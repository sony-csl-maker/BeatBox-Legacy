#pragma once

#include <JuceHeader.h>

#include "Gist.h"

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

                // case Stopped: // Code Stopped here instead
                //     _stopButton.setEnabled (false);
                //     _playButton.setEnabled (true);
                //     _transportSource.setPosition (0.0);
                //     break;
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

                    int frameSize = 441;
                    int sampleRate = 44100;
                    Gist<float> gist(frameSize, sampleRate);

                    for (int index = 0; index < buffer.getNumSamples(); index += 1) {
                        if (index % frameSize == 0) {
                            std::vector<float> sample(_audioTimeSeries.begin() + prevSampleIndex, _audioTimeSeries.begin() + sampleIndex);
                            gist.processAudioFrame(sample);
                            _onsets.push_back(gist.energyDifference());
                            prevSampleIndex += frameSize;
                            sampleIndex += frameSize;
                        }
                    }
                    findPeaks(_onsets);

                    for (int index = 0; index < _peaksValues.size(); index += 1)
                        std::cout << _peaksValues.at(index) << std::endl;
                }
            } });
    }

    void findPeaks(std::vector<float> _onsets)
    {
        float last_peak = -1e10;

        for (int index = 0; index < _onsets.size() -1; index++) {
            if ((_onsets[index] > _onset[index + 1]) && (_onset[index] > _onset[index - 1]) && (index - last_peak > 0)) {
                _peaks.push_back(index);
                _peaksValues.push_back(_onset[index]);
                last_peak = index;
            }
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

    float _preEnergySum = 0.0;

    juce::AudioFormatManager _formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> _readerSource;
    juce::AudioTransportSource _transportSource;
    TransportState _state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
