#pragma once

// Juce
#include <JuceHeader.h>

// std
#include <utility>

// gist
#include "Gist.h"

// libtorch
#include <torch/script.h>

// BeatBoxComponent
#include "BeatBoxComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

//==============================================================================
class ProcessorComponent : public juce::AudioAppComponent, public juce::ChangeListener, public juce::Timer
{
public:
    ProcessorComponent();

    ~ProcessorComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics &g) override;

    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    void timerCallback() override;

    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
    };

    void changeState(TransportState newState)
    {
        if (_state != newState)
        {
            _state = newState;

            switch (_state)
            {
            case Stopped:
                _stopButton.setEnabled(false);
                _playButton.setEnabled(true);
                _transportSource.setPosition(0.0);
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

            default:
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
            _loadedFile = fc.getResult();

            if (_loadedFile != juce::File{})
            {
                _reader = _formatManager.createReaderFor (_loadedFile);
                std::string filename = _loadedFile.getFileNameWithoutExtension().toStdString();

                _beatBox->setFilename(filename);

                juce::AudioSampleBuffer buffer(1, _reader->lengthInSamples);
                _buffer = buffer;

                if (_reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (_reader, true);
                    _transportSource.setSource (newSource.get(), 0, nullptr, _reader->sampleRate);
                    _playButton.setEnabled (true);
                    _readerSource.reset (newSource.release());

                    _buffer.setSize ((int) _reader->numChannels, (int) _reader->lengthInSamples);
                    _reader->read (&_buffer, 0, (int) _reader->lengthInSamples, 0, true, true);
                }
            } });
        return;
    }

    void loadTrackToConvert()
    {
        _beatBox->setTorchModules();

        _beatBox->fillAudioTimeSeries(_buffer);
    }

    void processTrackToConvert()
    {
        _beatBox->onsetDetection(_reader, _buffer);

        _beatBox->findPeaks();
    }

    void convertAndTrasnferTrack()
    {
        _beatBox->findStartEndOnset();

        _beatBox->transferTrack();
    }

    void saveTransferredFile()
    {
        _beatBox->saveTransferredFile();
    }

    void playConvertedTrack()
    {
        // do something here please
    }

    juce::File sendFileLoaded()
    {
        return (_loadedFile);
    }

    void playButtonClicked()
    {
        changeState(Starting);
    }

    void stopButtonClicked()
    {
        changeState(Stopping);
    }

private:
    //==========================================================================
    juce::TextButton _openButton;
    juce::TextButton _playButton;
    juce::TextButton _stopButton;
    juce::Label _currentPositionLabel;

    std::unique_ptr<juce::FileChooser> _fileChooser;

    // Original File
    juce::AudioFormatManager _formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> _readerSource;
    juce::AudioTransportSource _transportSource;
    TransportState _state;

    juce::File _loadedFile;

    juce::AudioFormatReader *_reader;
    juce::AudioSampleBuffer _buffer;

    juce::AudioFormatReader *_readerTransformed;
    juce::AudioSampleBuffer _bufferTransformed;

    std::unique_ptr<BeatBoxComponent> _beatBox = std::make_unique<BeatBoxComponent>();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorComponent)
};
