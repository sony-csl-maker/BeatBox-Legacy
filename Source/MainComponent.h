#pragma once

// Juce
#include <JuceHeader.h>

// std
#include <utility>

// gist
#include "Gist.h"

// libtorch
#include <torch/script.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

//==============================================================================
class MainComponent : public juce::AudioAppComponent, public juce::ChangeListener, public juce::Timer
{
public:
    MainComponent();

    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics &g) override;

    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    void timerCallback() override;

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
        Pause
    };

    void changeState(TransportState newState)
    {
        if (_state != newState)
        {
            _state = newState;

            switch (_state)
            {
            case Stopped: // Is Paused in reality
                _stopButton.setEnabled(false);
                _playButton.setEnabled(true);
                break;

            case Starting:
                _playButton.setEnabled(false);
                _transportSource.start();
                break;

            case Playing:
                _stopButton.setEnabled(true);
                break;

            case Stopping:
                _transportSource.stop();
                break;
            }
        }
    }

    void openButtonClicked()
    {
        _fileChooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
                                                           juce::File{},
                                                           "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        _fileChooser->launchAsync(chooserFlags, [this](const juce::FileChooser &fc)
                                  {
            auto file = fc.getResult();

            if (file != juce::File{})
            {

                auto* reader = _formatManager.createReaderFor (file);

                juce::AudioSampleBuffer buffer(reader->numChannels, reader->lengthInSamples);

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                    _transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                    _playButton.setEnabled (true);
                    _readerSource.reset (newSource.release());

                    buffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples);
                    reader->read (&buffer, 0, (int) reader->lengthInSamples, 0, true, true);

                    DBG(buffer.getNumSamples());

                    for (int index = 0; index < buffer.getNumSamples(); index += 1)
                        _audioTimeSeries.push_back(buffer.getSample(0, index));


                    int prevSampleIndex = 0;
                    int sampleIndex = (int)reader->sampleRate / 100;

                    try {
                        _encoder = torch::jit::load(_encoderPath);
                        _decoder = torch::jit::load(_decoderPath); //a regarder Torch::device / TorchOption
                    } catch (const c10::Error& e) {
                        std::cerr << "Errors(s): " << e.what() << "\n";
                        return (1);
                    }

                    int frameSize = (int)reader->sampleRate / 100;
                    int sampleRate = 44100;
                    Gist<float> gist(frameSize, sampleRate);

                    for (int index = 0; index < buffer.getNumSamples(); index += 1) {

                        if (index % (int)reader->sampleRate / 100 == 0) {
                            std::vector<float> sample(_audioTimeSeries.begin() + prevSampleIndex, _audioTimeSeries.begin() + sampleIndex);
                            gist.processAudioFrame(sample);
                            _onsets.push_back(gist.energyDifference());
                            prevSampleIndex += (int)reader->sampleRate / 100;
                            sampleIndex += (int)reader->sampleRate / 100;
                        }
                    }


                    findPeaks(_onsets);
                    for (int index = 0; index < _peaks.size() - 1; index += 1)
                        _peaks[index] *= 441;
                    findStartEndOnset(_audioTimeSeries, _peaks);
                    transferTrack(_startEnd);

                    for (auto it : _samplesTab) {
                        std::cout << "Sample :" << it.size() << std::endl;
                    }

                    std::vector<float> v(transferSample(_samplesTab[0]).data_ptr<float>(), transferSample(_samplesTab[0]).data_ptr<float>() + transferSample(_samplesTab[0]).numel());
                    for (auto a : v) std::cout << a << std::endl;

                }
            } });
    }

    at::Tensor transferSample(const std::vector<float> &sample)
    {
        auto encoderTensorOptions = torch::TensorOptions()
                                        .dtype(torch::kFloat)
                                        .device(torch::kCPU);

        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(torch::from_blob((void *)sample.data(), {(int64_t)1, (int64_t)sample.size()}, encoderTensorOptions));

        at::Tensor latent = _encoder.forward(inputs).toTensor().detach();

        std::vector<torch::jit::IValue> outputs;
        outputs.push_back(latent);
        return (_decoder.forward(outputs).toTensor().detach());
    }

    void encode(Array<float> audioBuffer, const unsigned int audioLength)
    {
        torch::Tensor tensor_wav = torch::from_blob(audioBuffer.data(), {1, audioLength});

        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(tensor_wav);

        torch::NoGradGuard no_grad;
        torch::Tensor encoderOutput = _encoder.forward(inputs).toTensor();

        float *valuePtr = encoderOutput.data_ptr<float>();
        Array<float> arrayValues(valuePtr, numberOfDimensions + numberOfClasses);

        Array<float> newZ;
        newZ.resize(numberOfDimensions);

        for (int idx = 0; idx < numberOfDimensions; idx++)
        {
            newZ.set(idx, arrayValues[idx]);

            Array<float> normalizedClasses;
            normalizedClasses.resize(numberOfClasses);

            for (int idx = 0; idx < numberOfClasses; idx++)
            {
                normalizedClasses.set(idx, arrayValues[idx + numberOfDimensions]);
            }
        }
    }

    void decode(Array<float> z_c_array_ptr, unsigned int numberOfDimensions, unsigned int numberOfClasses)
    {
        torch::Tensor tensor_z_c = torch::from_blob(z_c_array_ptr, {1, numberOfDimensions + numberOfClasses}).clone();

        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(tensor_z_c);

        torch::NoGradGuard no_grad;
        torch::Tensor decoderOutput = _decoder.forward(inputs).toTensor();

        float *value = decoderOutput.data_ptr<float>();
        int sizeWav = 24575;
        Array<float> arrayWav(value, sizeWav);
    }

    void findPeaks(std::vector<float> onsets)
    {
        float last_peak = -1e10;

        for (int index = 0; index < onsets.size() - 1; index++)
        {
            if ((_onsets[index] > onsets[index + 1]) && (onsets[index] > onsets[index - 1]) && (index - last_peak > 0))
            {
                _peaks.push_back(index);
                _peaksValues.push_back(onsets[index]);
                last_peak = index;
            }
        }
    }

    void findStartEndOnset(std::vector<float> audio, std::vector<float> onsetPeaks)
    {
        int length = 24575;

        for (int index = 0; index < onsetPeaks.size() - 1; index++)
        {
            if ((onsetPeaks[index] + length) < audio.size())
            {
                _startEnd.push_back({onsetPeaks[index], onsetPeaks[index] + length});
            }
        }
    }

    void transferTrack(std::vector<std::pair<float, float>> startEnd)
    {
        for (auto it : startEnd)
        {
            std::vector<float> sample(_audioTimeSeries.begin() + it.first, _audioTimeSeries.begin() + it.second);
            _samplesTab.push_back(sample);
        }
    }

    void playButtonClicked()
    {
        changeState(Starting);
    }

    void stopButtonClicked()
    {
        changeState(Stopping);
    }

    void pauseButtonClicked()
    {
        changeState(Pause);
    }

    //==========================================================================
    juce::TextButton _openButton;
    juce::TextButton _playButton;
    juce::TextButton _stopButton;
    juce::TextButton _pauseButton; // Need to add the button to pause music
    juce::Label _currentPositionLabel;

    std::unique_ptr<juce::FileChooser> _fileChooser;

    juce::AudioSampleBuffer _buffer;
    std::vector<float> _audioTimeSeries;
    std::vector<float> _onsets;
    std::vector<float> _peaks;
    std::vector<float> _peaksValues;

    std::vector<std::pair<float, float>> _startEnd;

    std::vector<std::vector<float>> _samplesTab;

    std::string _encoderPath;
    std::string _decoderPath;
    torch::jit::script::Module _encoder;
    torch::jit::script::Module _decoder;

    float _preEnergySum = 0.0;

    juce::AudioFormatManager _formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> _readerSource;
    juce::AudioTransportSource _transportSource;
    TransportState _state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
