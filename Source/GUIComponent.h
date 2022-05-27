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
                      public juce::Button::Listener
{
public:
    //==============================================================================
    GUIComponent ();
    ~GUIComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]


    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource.get() == nullptr)
            bufferToFill.clearActiveBufferRegion();
        else
            transportSource.getNextAudioBlock (bufferToFill);
    }

    void releaseResources() override
    {
        transportSource.releaseResources();
    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
            transportSourceChanged();
    }


    ////////////////////////////////////////////////////////////////////////////

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    void playButtonClicked()
    {
        //do somotehing here pls
    }

    void stopButtonClicked()
    {
        //do somotehing here pls
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

    void transportSourceChanged()
    {
        if (transportSource.isPlaying())
            changeState (Playing);
        else
            changeState (Stopped);
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

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;

    juce::AudioThumbnailCache thumbnailCache;
    SimpleThumbnailComponent thumbnailComp;
    SimplePositionOverlay positionOverlay;

    juce::File file;

    std::pair<juce::AudioSampleBuffer, juce::AudioFormatReader *> prerequisites;

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

    std::unique_ptr<TextButton> playBtn;
    std::unique_ptr<TextButton> pauseBtn;

    std::unique_ptr<TextButton> downloadBtn;

    bool status = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUIComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
