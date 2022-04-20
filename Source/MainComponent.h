#pragma once

#include <JuceHeader.h>

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

                    // _prevMagnitudeSpectrum_spectralDifference.resize((int)reader->sampleRate);
                    // for (int index = 0; index < (int)reader->sampleRate / 100; index += 1) {
                    //     _prevMagnitudeSpectrum_spectralDifference[index] = 0.0;
                    // }

                    for (int index = 0; index < buffer.getNumSamples(); index += 1) {

                        if (index % (int)reader->sampleRate / 100 == 0) {
                            std::vector<float> sample(_audioTimeSeries.begin() + prevSampleIndex, _audioTimeSeries.begin() + sampleIndex);
                            _onsets.push_back(OnsetDetectionFunction(sample));
                            prevSampleIndex += (int)reader->sampleRate / 100;
                            sampleIndex += (int)reader->sampleRate / 100;
                        }
                    }

                    for (int index = 0; index < _onsets.size(); index += 1)
                        std::cout << _onsets.at(index) << std::endl;
                }
            } });
    }

    float OnsetDetectionFunction(const std::vector<float> buffer)
    {
        float sum;
        float difference;

        sum = 0;

        for (size_t i = 0; i < buffer.size(); i += 1) {
            sum = sum + (buffer[i] * buffer[i]);
        }

        difference = sum - _preEnergySum;

        _preEnergySum = sum;

        return ((difference > 0) ? difference : 0.0);
    }

    // float OnsetDetectionFunction(const std::vector<float> magnitudeSpectrum)
    // {
    //     float sum = 0; // initialise sum to zero

    //     for (size_t i = 0; i < magnitudeSpectrum.size(); i++)
    //     {
    //         // calculate difference
    //         float diff = magnitudeSpectrum[i] - _prevMagnitudeSpectrum_spectralDifference[i];

    //         // ensure all difference values are positive
    //         if (diff < 0)
    //         {
    //             diff = diff * -1;
    //         }

    //         // add difference to sum
    //         sum = sum + diff;

    //         // store the sample for next time
    //         _prevMagnitudeSpectrum_spectralDifference[i] = magnitudeSpectrum[i];
    //     }

    //     return sum;
    // }

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

    float _preEnergySum = 0.0;
    std::vector<float> _prevMagnitudeSpectrum_spectralDifference;

    juce::AudioFormatManager _formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> _readerSource;
    juce::AudioTransportSource _transportSource;
    TransportState _state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
