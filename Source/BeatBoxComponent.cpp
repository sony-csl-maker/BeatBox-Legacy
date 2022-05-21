/*
  ==============================================================================
    This file contains the basic startup code for a JUCE application.
  ==============================================================================
*/

#include "BeatBoxComponent.h"

BeatBoxComponent::BeatBoxComponent()
{
}

BeatBoxComponent::~BeatBoxComponent()
{
}

void BeatBoxComponent::saveOriginFile()
{
    juce::File file("JUCE/examples/CMake/BeatBox/Musics/" + _filename + "-orig" + ".wav");

    juce::Array<float> array;

    for (auto it : _audioTimeSeries)
        array.add(it);

    AudioBuffer<float> buffer(2, _audioTimeSeries.size());

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

void BeatBoxComponent::saveTransferredFile()
{
    juce::File file("JUCE/examples/CMake/BeatBox/Musics/" + _filename + "-transferred" + ".wav");
    juce::Array<float> array;

    fillEncodedSamples();

    for (auto it : _encodedAudioTimeSeries)
        array.add(it);

    AudioBuffer<float> buffer(1, _encodedAudioTimeSeries.size());

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

void BeatBoxComponent::saveSamplesFile(long unsigned int sampleIndex)
{
    juce::File file("JUCE/examples/CMake/BeatBox/Musics/" + _filename + "-sample" + std::to_string(sampleIndex) + ".wav");
    juce::Array<float> array(transferSample(_samplesTab.at(sampleIndex)));
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

void BeatBoxComponent::fillEncodedSamples()
{
    std::vector<juce::Array<float>> encodedSamplesTab;

    for (size_t sampleIndex = 0; sampleIndex < _samplesTab.size(); sampleIndex += 1)
        encodedSamplesTab.push_back(transferSample(_samplesTab[sampleIndex]));

    _encodedAudioTimeSeries.resize(_audioTimeSeries.size());
    std::fill(_encodedAudioTimeSeries.begin(), _encodedAudioTimeSeries.end(), 0.0f);

    for (long unsigned int index = 0; index < _startEnd.size(); index += 1)
    {
        for (long unsigned int sampleIndex = 0; sampleIndex < 24575; sampleIndex += 1)
        {
            _encodedAudioTimeSeries[_startEnd.at(index).first + sampleIndex] = encodedSamplesTab[index][sampleIndex];
        }
    }
}

juce::Array<float> BeatBoxComponent::transferSample(std::vector<float> sample)
{
    juce::Array<float> transform;

    for (auto it : sample)
        transform.add(it);

    juce::Array<float> encodedSample = encodeSample(transform, 24575);

    return (decodeSample(encodedSample));
}

juce::Array<float> BeatBoxComponent::encodeSample(juce::Array<float> audioBuffer, const int audioLength)
{
    torch::Tensor tensor_wav = torch::from_blob(audioBuffer.data(), {1, audioLength});

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(tensor_wav);

    torch::NoGradGuard no_grad;
    torch::Tensor encoderOutput = _encoder.forward(inputs).toTensor();

    float *valuePtr = encoderOutput.data_ptr<float>();
    juce::Array<float> arrayValues(valuePtr, _numberOfDimensions + _numberOfClasses);

    _newZ.resize(_numberOfDimensions);

    for (int idx = 0; idx < _numberOfDimensions; idx++)
    {
        _newZ.set(idx, arrayValues[idx]);
    }

    _normalizedClasses.resize(_numberOfClasses);

    for (int index = 0; index < _numberOfClasses; index++)
    {
        _normalizedClasses.set(index, arrayValues[index + _numberOfDimensions]);
    }
    return (arrayValues);
}

juce::Array<float> BeatBoxComponent::decodeSample(juce::Array<float> z_c_array_ptr)
{
    torch::Tensor tensor_z_c = torch::from_blob(z_c_array_ptr.data(), {1, _numberOfDimensions + _numberOfClasses}).clone();

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(tensor_z_c);

    torch::NoGradGuard no_grad;
    torch::Tensor decoderOutput = _decoder.forward(inputs).toTensor();

    float *value = decoderOutput.data_ptr<float>();
    int sizeWav = 24575;
    juce::Array<float> arrayWav(value, sizeWav);

    std::cout << "Decodede Sample number " << std::to_string(_index) << ": " << arrayWav.size() << std::endl;
    _index += 1;

    return (arrayWav);
}

void BeatBoxComponent::findPeaks()
{
    float last_peak = -1e10;

    for (long unsigned int index = 0; index < _onsets.size() - 1; index++)
    {
        if ((std::distance(_onsets.begin() - _smoothness, std::max_element(_onsets.begin() - _smoothness, _onsets.end()) + _smoothness) > (long int)index && (index - last_peak > 0)))
        {
            _peaks.push_back(index);
            _peaksValues.push_back(_onsets[index]);
            last_peak = index;
        }
    }

    for (unsigned int index = 0; index < _peaks.size() - 1; index += 1)
        _peaks[index] *= 441;
}

void BeatBoxComponent::findPeaksWithSmoothness(int smoothness)
{
    float last_peak = -1e10;
    _smoothness = smoothness;

    for (long unsigned int index = 0; index < _onsets.size() - 1; index++)
    {
        if ((std::distance(_onsets.begin() - _smoothness, std::max_element(_onsets.begin() - _smoothness, _onsets.end()) + _smoothness) > (long int)index && (index - last_peak > 0)))
        {
            _peaks.push_back(index);
            _peaksValues.push_back(_onsets[index]);
            last_peak = index;
        }
    }

    for (unsigned int index = 0; index < _peaks.size() - 1; index += 1)
        _peaks[index] *= 441;
}

void BeatBoxComponent::findStartEndOnset()
{
    long unsigned int length = 24575;

    for (long unsigned int index = 0; index < _peaks.size() - 1; index++)
    {
        if ((_peaks[index] + length) < _audioTimeSeries.size())
        {
            _startEnd.push_back({_peaks[index], _peaks[index] + length});
        }
    }
}

void BeatBoxComponent::transferTrack()
{
    int index = 0;

    for (auto it : _startEnd)
    {
        std::vector<float> sample(_audioTimeSeries.begin() + it.first, _audioTimeSeries.begin() + it.second);
        std::cout << "[TransferTrack] NON ENCODED : for sample " << std::to_string(index) << " " << it.first << " - " << it.second << std::endl;
        _samplesTab.push_back(sample); // 24575
        index += 1;
    }
}

void BeatBoxComponent::fillAudioTimeSeries(juce::AudioSampleBuffer buffer)
{
    for (int index = 0; index < buffer.getNumSamples(); index += 1)
        _audioTimeSeries.push_back(buffer.getSample(0, index));
}

void BeatBoxComponent::onsetDetection(juce::AudioFormatReader *reader, juce::AudioSampleBuffer buffer)
{

    int prevSampleIndex = 0;
    int sampleIndex = (int)reader->sampleRate / 100;

    int frameSize = (int)reader->sampleRate / 100;
    int sampleRate = 44100;
    Gist<float> gist(frameSize, sampleRate);

    for (int index = 0; index < buffer.getNumSamples(); index += 1)
    {

        if (index % (int)reader->sampleRate / 100 == 0)
        {
            std::vector<float> sample(_audioTimeSeries.begin() + prevSampleIndex, _audioTimeSeries.begin() + sampleIndex);
            gist.processAudioFrame(sample);
            _onsets.push_back(gist.energyDifference());
            prevSampleIndex += (int)reader->sampleRate / 100;
            sampleIndex += (int)reader->sampleRate / 100;
        }
    }
}

void BeatBoxComponent::setTorchModules()
{
    try
    {
        _encoder = torch::jit::load(_encoderPath);
        _decoder = torch::jit::load(_decoderPath);
    }
    catch (const c10::Error &e)
    {
        std::cerr << "Errors(s): " << e.what() << "\n";
        exit(1);
    }
}

void BeatBoxComponent::setFilename(std::string filename)
{
    _filename = filename;
}
