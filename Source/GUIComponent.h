/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <unistd.h>

#include <JuceHeader.h>

#include "ProcessorComponent.h"

#include "Thumbnails/Tools/SimplePeakModelization.h"
#include "Thumbnails/Tools/SimplePositionOverlay.h"
#include "Thumbnails/Tools/SimpleThresholdIndex.h"
#include "Thumbnails/Tools/SimpleThumbnailComponent.h"
#include "Style/OtherLookAndFeel.hpp"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GUIComponent  : public juce::AudioAppComponent,
                      public juce::ChangeListener,
                      public juce::Slider::Listener,
                      public juce::Button::Listener,
                      public juce::Timer
{
public:
    //==============================================================================
    GUIComponent ();
    ~GUIComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void timerCallback() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
        resultTransportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource.get() == nullptr)
            bufferToFill.clearActiveBufferRegion();
        else {
            transportSource.getNextAudioBlock (bufferToFill);
            resultTransportSource.getNextAudioBlock (bufferToFill);
        }
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
        resultTransportSource.releaseResources();
    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
            transportSourceChanged();
        if (source == &resultTransportSource)
            resultTransportSourceChanged();
    }


    ////////////////////////////////////////////////////////////////////////////

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    void playButtonClicked()
    {
        changeResultState(Starting);
    }

    void stopButtonClicked()
    {
        changeResultState(Stopping);
    }

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    // GET Informations from processor

    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
                case Stopped:
                    stopButtonT.setEnabled (false);
                    playButtonT.setEnabled (true);
                    transportSource.setPosition (0.0);
                    break;

                case Starting:
                    playButtonT.setEnabled (false);
                    transportSource.start();
                    break;

                case Playing:
                    stopButtonT.setEnabled (true);
                    break;

                case Stopping:
                    transportSource.stop();
                    break;

                default:
                    break;
            }
        }
    }

    void changeResultState (TransportState newState)
    {
        if (resultState != newState)
        {
            resultState = newState;

            switch (resultState)
            {
                case Stopped:
                    stopBtn.setEnabled (false);
                    playBtn.setEnabled (true);
                    resultTransportSource.setPosition (0.0);
                    break;

                case Starting:
                    playBtn.setEnabled (false);
                    resultTransportSource.start();
                    break;

                case Playing:
                    stopBtn.setEnabled (true);
                    break;

                case Stopping:
                    resultTransportSource.stop();
                    break;

                default:
                    break;
            }
        }
    }

    void transportSourceChanged()
    {
        if (transportSource.isPlaying())
            changeState (Playing);
        else
            changeState (Stopped);
    }

    void resultTransportSourceChanged()
    {
        if (resultTransportSource.isPlaying())
            changeResultState (Playing);
        else
            changeResultState (Stopped);
    }

    void openAudioTrack()
    {
        juce::File resultFile(processor->getPath() + "/examples/CMake/BeatBox/Musics/.temp/" + pathToFile);

        auto *reader = formatManager.createReaderFor (resultFile);
        juce::AudioSampleBuffer buffer(1, reader->lengthInSamples);

        std::string filename = file.getFileNameWithoutExtension().toStdString();
        processor->setFilename(filename);

        if (reader != nullptr)
        {
            auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
            resultTransportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
            playBtn.setEnabled (true);
            thumbnailComp.setFile (resultFile);
            readerSource.reset (newSource.release());

            buffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples);
            reader->read (&buffer, 0, (int) reader->lengthInSamples, 0, true, true);

            prerequisites.first = buffer;
            prerequisites.second = reader;
            processor->sendData(prerequisites);
            processor->loadFile();
            processor->processOnsets();
            processor->processPeaks(smoothnessSlider->getValue(), thresholdSlider->getValue());                    processor->transferTrack();
            peaksModelization->sendLengthOfTrack((long unsigned int)reader->lengthInSamples);
        }
    }

    void openButtonClicked()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                       juce::File{},
                                                       "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const FileChooser& fc)
        {
            file = fc.getResult();

            if (file != File{})
            {
                auto *reader = formatManager.createReaderFor (file);
                juce::AudioSampleBuffer buffer(1, reader->lengthInSamples);

                std::string filename = file.getFileNameWithoutExtension().toStdString();
                processor->setFilename(filename);

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                    transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                    playButtonT.setEnabled (true);
                    thumbnailComp.setFile (file);
                    readerSource.reset (newSource.release());

                    buffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples);
                    reader->read (&buffer, 0, (int) reader->lengthInSamples, 0, true, true);

                    prerequisites.first = buffer;
                    prerequisites.second = reader;
                    processor->sendData(prerequisites);
                    processor->loadFile();
                    processor->processOnsets();
                    processor->processPeaks(smoothnessSlider->getValue(), thresholdSlider->getValue());                    processor->transferTrack();
                    peaksModelization->sendLengthOfTrack((long unsigned int)reader->lengthInSamples);
                }
            }
        });
    }

    void playButtonTClicked()
    {
        changeState (Starting);
    }

    void stopButtonTClicked()
    {
        changeState (Stopping);
    }


    juce::TextButton openButtonT;
    juce::TextButton playButtonT;
    juce::TextButton stopButtonT;

    std::string pathToFile;

    juce::TextButton playBtn;
    juce::TextButton stopBtn;

    std::unique_ptr<juce::ToggleButton> drumifyToggle;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;

    juce::AudioTransportSource resultTransportSource;
    TransportState resultState;

    juce::AudioThumbnailCache thumbnailCache;
    SimpleThumbnailComponent thumbnailComp;
    SimplePositionOverlay positionOverlay;

    SimpleThumbnailComponent thumbnailResultComp;

    juce::File file;

    std::pair<juce::AudioSampleBuffer, juce::AudioFormatReader *> prerequisites;

    juce::AudioBuffer<float> _convertedBuffer;

    // std::unique_ptr<ProcessorComponent> processor = std::make_unique<ProcessorComponent>();

    ////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<ProcessorComponent> processor = std::make_unique<ProcessorComponent>();

    // std::unique_ptr<OriginalThumbnailComponent> thumbnailOriginal;

    std::unique_ptr<SimpleThresholdIndex> thresholdIndex;
    std::unique_ptr<SimplePeakModelization> peaksModelization;
    //[/UserVariables]

    //==============================================================================

    std::unique_ptr<Slider> smoothnessSlider;
    std::unique_ptr<Slider> thresholdSlider;

    std::unique_ptr<TextButton> convertBtn;

    std::unique_ptr<TextButton> downloadBtn;

    OtherLookAndFeel otherLookAndFeel;

    bool status = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUIComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
