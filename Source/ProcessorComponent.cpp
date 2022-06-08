#include "ProcessorComponent.h"

//==============================================================================
ProcessorComponent::ProcessorComponent()
{
    startTimer(50);
}

ProcessorComponent::~ProcessorComponent()
{
}

void ProcessorComponent::timerCallback()
{

}

//==============================================================================
void ProcessorComponent::loadFile()
{
    juce::AudioSampleBuffer buffer = _data.first;

    audioTimeSeries.clear();

    std::cout << "ProcessorComponent::loadFile()" << std::endl;

    for (int index = 0; index < buffer.getNumSamples(); index += 1)
        audioTimeSeries.push_back(buffer.getSample(0, index));

    fileWasLoaded = true;

    loadModel();
}

void ProcessorComponent::loadModel()
{
    std::cout << "ProcessorComponent::loadModel()" << std::endl;

    try
    {
        encoder = torch::jit::load(encoderPath);
        decoder = torch::jit::load(decoderPath);
    }
    catch (const c10::Error &e)
    {
        std::cerr << "Errors(s): " << e.what() << "\n";
        exit(1);
    }
}

void ProcessorComponent::processOnsets()
{
    std::cout << "ProcessorComponent::processOnsets()" << std::endl;
    onsets.clear();

    juce::AudioSampleBuffer buffer = _data.first;
    juce::AudioFormatReader *reader = _data.second;

    int prevSampleIndex = 0;
    int sampleIndex = (int)reader->sampleRate / 100;

    int frameSize = (int)reader->sampleRate / 100;
    int sampleRate = 44100;
    Gist<float> gist(frameSize, sampleRate);

    for (int index = 0; index < buffer.getNumSamples(); index += 1)
    {
        if (index % (int)reader->sampleRate / 100 == 0)
        {
            std::vector<float> sample(audioTimeSeries.begin() + prevSampleIndex, audioTimeSeries.begin() + sampleIndex);
            gist.processAudioFrame(sample);
            onsets.push_back(gist.energyDifference());
            prevSampleIndex += (int)reader->sampleRate / 100;
            sampleIndex += (int)reader->sampleRate / 100;
        }
    }
    onsetsProcessed = true;
}

void ProcessorComponent::processPeaks(float smoothnessValue, float tresholdValue)
{
    std::cout << "ProcessorComponent::processPeaks() with value of " << smoothnessValue << "and treshold of " << tresholdValue << std::endl;
    peaksProcessed = false;
    peaksValues.clear();
    peaksIndex.clear();

    float last_peak = -1e10;
    smoothness = smoothnessValue;

    for (long unsigned int index = 0; index < onsets.size() - 1; index++)
    {
        if ((onsets[index] > onsets[index + 1]) && (onsets[index] > onsets[index - 1]) && (index - last_peak > 0))
        {
            if (onsets[index] >= tresholdValue) {
                peaksIndex.push_back(index);
                peaksValues.push_back(onsets[index]);
                last_peak = index;
            }
        }
    }

    for (unsigned int index = 0; index < peaksIndex.size() - 1; index += 1)
        peaksIndex[index] *= 441;

    peaksProcessed = true;
}

void ProcessorComponent::extractPeaks()
{
    std::cout << "ProcessorComponent::extractPeaks()" << std::endl;
    startEnd.clear();
    long unsigned int length = 24575;

    for (long unsigned int index = 0; index < peaksIndex.size() - 1; index++)
    {
        if ((peaksIndex[index] + length) < audioTimeSeries.size())
        {
            startEnd.push_back({peaksIndex[index], peaksIndex[index] + length});
        }
    }
}

juce::Array<float> ProcessorComponent::encodeSample(Array<float> audioBuffer, const int audioLength)
{
    torch::Tensor tensor_wav = torch::from_blob(audioBuffer.data(), {1, audioLength});

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(tensor_wav);

    torch::NoGradGuard no_grad;
    torch::Tensor encoderOutput = encoder.forward(inputs).toTensor();

    float *valuePtr = encoderOutput.data_ptr<float>();
    Array<float> arrayValues(valuePtr, numberOfDimensions + numberOfClasses);

    newZ.resize(numberOfDimensions);

    for (int idx = 0; idx < numberOfDimensions; idx++)
    {
        newZ.set(idx, arrayValues[idx]);
    }

    normalizedClasses.resize(numberOfClasses);

    for (int index = 0; index < numberOfClasses; index++)
    {
        normalizedClasses.set(index, arrayValues[index + numberOfDimensions]);
    }
    if (drumifyState) {
        arrayValues = drumify(arrayValues);
    }
    for (auto kick : normalizedClasses)
        std::cout << "normalizedClass: " << kick << std::endl;
    return (arrayValues);
}

juce::Array<float> ProcessorComponent::drumify(juce::Array<float> &audioBuffer)
{
    int tempIndex = -1;

    for (int index = 0; index < numberOfClasses; index++) {
        if (normalizedClasses[index] > 0.5) {
            std::cout << "need drumify: " << index << std::endl;
            tempIndex = index;
        }
    }
    if (tempIndex != -1) {
        for (int indexSec = 0; indexSec < numberOfClasses; indexSec++) {
            if (indexSec == tempIndex) {
                audioBuffer.set(numberOfDimensions + indexSec, 1);
            } else {
                audioBuffer.set(numberOfDimensions + tempIndex, 0);
            }
        }
    }
    return (audioBuffer);

}

juce::Array<float> ProcessorComponent::decodeSample(juce::Array<float> z_c_array_ptr)
{
    torch::Tensor tensor_z_c = torch::from_blob(z_c_array_ptr.data(), {1, numberOfDimensions + numberOfClasses}).clone();

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(tensor_z_c);

    torch::NoGradGuard no_grad;
    torch::Tensor decoderOutput = decoder.forward(inputs).toTensor();

    float *value = decoderOutput.data_ptr<float>();
    int sizeWav = 24575;
    Array<float> arrayWav(value, sizeWav);

    std::cout << "Decodede Sample number " << std::to_string(_index) << ": " << arrayWav.size() << std::endl;
    _index += 1;

    return (arrayWav);
}

juce::Array<float> ProcessorComponent::processSamples(std::vector<float> sample)
{
    juce::Array<float> transform;

    for (auto it : sample)
        transform.add(it);

    juce::Array<float> encodedSample = encodeSample(transform, 24575);

    return (decodeSample(encodedSample));
}

void ProcessorComponent::transferTrack()
{
    int index = 0;

    for (auto it : startEnd)
    {
        std::vector<float> sample(audioTimeSeries.begin() + it.first, audioTimeSeries.begin() + it.second);
        std::cout << "[TransferTrack] NON ENCODED : for sample " << std::to_string(index) << " " << it.first << " - " << it.second << std::endl;
        samplesTab.push_back(sample); // 24575
        index += 1;
    }
}

void ProcessorComponent::processAudioTrack()
{
    transferTrack();

    std::vector<Array<float>> tempSampleTab;

    // the problem lies on samplesTab being empty :/ check from previous code if it's the case

    for (size_t sampleIndex = 0; sampleIndex < samplesTab.size(); sampleIndex += 1)
        tempSampleTab.push_back(processSamples(samplesTab[sampleIndex]));

    encodedAudioTimeSeries.resize(audioTimeSeries.size());
    std::fill(encodedAudioTimeSeries.begin(), encodedAudioTimeSeries.end(), 0.0f);

    for (long unsigned int index = 0; index < startEnd.size(); index += 1)
    {
        for (long unsigned int sampleIndex = 0; sampleIndex < 24575; sampleIndex += 1)
        {
            encodedAudioTimeSeries[startEnd.at(index).first + sampleIndex] = tempSampleTab[index][sampleIndex];
        }
    }

    AudioBuffer<float> buffer(1, encodedAudioTimeSeries.size());

    for (long unsigned int index = 0; index < encodedAudioTimeSeries.size(); index += 1)
        buffer.setSample(0, index, encodedAudioTimeSeries[index]);

    _convertedBuffer = buffer;
}

void ProcessorComponent::downloadOriginalFile()
{
    juce::File file("JUCE/examples/CMake/BeatBox/Musics/" + filename + "-orig" + ".wav");

    Array<float> array;

    for (auto it : audioTimeSeries)
        array.add(it);

    AudioBuffer<float> buffer(2, audioTimeSeries.size());

    for (int index = 0; index < array.size(); index += 1)
        buffer.setSample(0, index, array[index]);

    juce::WavAudioFormat format;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    writer.reset(format.createWriterFor(new juce::FileOutputStream(file),
                                        44100.0,
                                        1,
                                        16,
                                        {},
                                        0));
    if (writer != nullptr)
    {
        std::cout << "Writing Origin file..." << std::endl;
        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }
}

void ProcessorComponent::downloadProcessedFile()
{
    std::cout << "Downloading Processed file..." << std::endl;

    juce::File file("SONYCSL_REPO/examples/CMake/BeatBox/Musics/" + filename + "-transferred" + ".wav");
    Array<float> array;

    processAudioTrack();

    for (auto it : encodedAudioTimeSeries)
        array.add(it);

    AudioBuffer<float> buffer(1, encodedAudioTimeSeries.size());

    for (int index = 0; index < array.size(); index += 1)
        buffer.setSample(0, index, array[index]);

    juce::WavAudioFormat format;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    writer.reset(format.createWriterFor(new juce::FileOutputStream(file),
                                        44100.0,
                                        1,
                                        16,
                                        {},
                                        0));
    if (writer != nullptr)
    {
        std::cout << "Writing Transferred file..." << std::endl;
        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }
}

void ProcessorComponent::downloadSamplesFile(long unsigned int sampleIndex)
{
    juce::File file("JUCE/examples/CMake/BeatBox/Musics/" + filename + "-sample" + std::to_string(sampleIndex) + ".wav");
    Array<float> array(processSamples(samplesTab.at(sampleIndex)));
    AudioBuffer<float> buffer(2, 24575);

    for (int index = 0; index < array.size(); index += 1)
        buffer.setSample(0, index, array[index]);

    juce::WavAudioFormat format;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    writer.reset(format.createWriterFor(new juce::FileOutputStream(file),
                                        44100.0,
                                        1,
                                        16,
                                        {},
                                        0));
    if (writer != nullptr)
    {
        std::cout << "Writing Samples file..." << std::endl;
        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }
}
