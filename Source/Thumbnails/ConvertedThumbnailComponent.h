#pragma once

// Juce
#include <JuceHeader.h>
#include "Tools/SimplePositionOverlay.h"
#include "Tools/SimpleThumbnailComponent.h"

//==============================================================================
class ConvertedThumbnailComponent : public juce::AudioAppComponent,
                                    public juce::ChangeListener
{
public:
    ConvertedThumbnailComponent()
        : state(Stopped),
          thumbnailCache(5),
          thumbnailComp(512, formatManager, thumbnailCache),
          positionOverlay(transportSource)
    {
        addAndMakeVisible(&openButton);
        openButton.setButtonText("Open...");
        openButton.onClick = [this]
        { openButtonClicked(); };

        addAndMakeVisible(&playButton);
        playButton.setButtonText("Play");
        playButton.onClick = [this]
        { playButtonClicked(); };
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled(false);

        addAndMakeVisible(&stopButton);
        stopButton.setButtonText("Stop");
        stopButton.onClick = [this]
        { stopButtonClicked(); };
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled(false);

        addAndMakeVisible(&thumbnailComp);
        addAndMakeVisible(&positionOverlay);

        setSize(520, 880);

        formatManager.registerBasicFormats();
        transportSource.addChangeListener(this);

        setAudioChannels(2, 2);
    }

    ~ConvertedThumbnailComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override
    {
        if (readerSource.get() == nullptr)
            bufferToFill.clearActiveBufferRegion();
        else
            transportSource.getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    void resized() override
    {
        openButton.setBounds(10, 10, getWidth() - 20, 20);
        playButton.setBounds(10, 40, getWidth() - 20, 20);
        stopButton.setBounds(10, 70, getWidth() - 20, 20);

        juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, 100);
        thumbnailComp.setBounds(thumbnailBounds);
        positionOverlay.setBounds(thumbnailBounds);
    }

    void changeListenerCallback(juce::ChangeBroadcaster *source) override
    {
        if (source == &transportSource)
            transportSourceChanged();
    }

    juce::File getFileLoaded()
    {
        return (file);
    }

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    void changeState(TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
            case Stopped:
                stopButton.setEnabled(false);
                playButton.setEnabled(true);
                transportSource.setPosition(0.0);
                break;

            case Starting:
                playButton.setEnabled(false);
                transportSource.start();
                break;

            case Playing:
                stopButton.setEnabled(true);
                break;

            case Stopping:
                transportSource.stop();
                break;

            default:
                break;
            }
        }
    }

    void transportSourceChanged()
    {
        if (transportSource.isPlaying())
            changeState(Playing);
        else
            changeState(Stopped);
    }

    void openButtonClicked()
    {

        file = juce::File("JUCE/examples/CMake/BeatBox/Musics/silence.wav");

        if (file != File{})
        {
            auto *reader = formatManager.createReaderFor(file);

            std::string filename = file.getFileNameWithoutExtension().toStdString();

            if (reader != nullptr)
            {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                playButton.setEnabled(true);
                thumbnailComp.setFile(file);
                readerSource.reset(newSource.release());
            }
        }

        playButton.setEnabled(true);
    }

    void playButtonClicked()
    {
        changeState(Starting);
    }

    void stopButtonClicked()
    {
        changeState(Stopping);
    }

    //==========================================================================
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;

    juce::AudioThumbnailCache thumbnailCache;
    SimpleThumbnailComponent thumbnailComp;
    SimplePositionOverlay positionOverlay;

    juce::File file;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConvertedThumbnailComponent)
};
