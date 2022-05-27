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

void ProcessorComponent::processPeaks(float value)
{
    std::cout << "ProcessorComponent::processPeaks()" << std::endl;

    float last_peak = -1e10;
    smoothness = value;

    for (long unsigned int index = 0; index < onsets.size() - 1; index++)
    {
        if ((std::distance(onsets.begin() - smoothness, std::max_element(onsets.begin() - smoothness, onsets.end()) + smoothness) > (long int)index && (index - last_peak > 0)))
        {
            peaksIndex.push_back(index);
            peaksValues.push_back(onsets[index]);
            last_peak = index;
        }
    }

    for (unsigned int index = 0; index < peaksIndex.size() - 1; index += 1)
        peaksIndex[index] *= 441;

    peaksProcessed = true;
}

void ProcessorComponent::extractPeaks()
{
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
    return (arrayValues);
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

void ProcessorComponent::processAudioTrack()
{
    std::vector<Array<float>> tempSampleTab;

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

    juce::File file("JUCE/examples/CMake/BeatBox/Musics/" + filename + "-transferred" + ".wav");
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
