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

//[Headers] You can add your own extra header files here...
#include <memory>
#include <iostream>
//[/Headers]

#include "GUIComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GUIComponent::GUIComponent() :
        state (Stopped),
        thumbnailCache (5),
        thumbnailComp (512, formatManager, thumbnailCache),
        positionOverlay (transportSource)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    // thumbnailOriginal.reset(new OriginalThumbnailComponent());
    // addAndMakeVisible(thumbnailOriginal.get());

    addAndMakeVisible (&openButtonT);
    openButtonT.setButtonText ("Open...");
    openButtonT.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible (&playButtonT);
    playButtonT.setButtonText ("Play");
    playButtonT.onClick = [this] { playButtonTClicked(); };
    playButtonT.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
    playButtonT.setEnabled (false);

    addAndMakeVisible (&stopButtonT);
    stopButtonT.setButtonText ("Stop");
    stopButtonT.onClick = [this] { stopButtonTClicked(); };
    stopButtonT.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
    stopButtonT.setEnabled (false);

    addAndMakeVisible (&thumbnailComp);
    addAndMakeVisible (&positionOverlay);


    ////////////////////////////////////////////////////////////////////////////

    thresholdIndex.reset(new SimpleThresholdIndex());
    addAndMakeVisible(thresholdIndex.get());
    thresholdIndex->setBounds(10, 100, 500, 100);

    peaksModelization.reset(new SimplePeakModelization());
    addAndMakeVisible(peaksModelization.get());
    peaksModelization->setBounds(10, 210, 500, 100);

    thresholdSlider.reset(new juce::Slider("thresholdSlider"));
    addAndMakeVisible(thresholdSlider.get());
    thresholdSlider->setRange(0, 10, 0.01);
    thresholdSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    thresholdSlider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    thresholdSlider->addListener(this);

    thresholdSlider->setBounds(70, 330, 190, 100);
    thresholdSlider->setValue(9.5);

    smoothnessSlider.reset(new juce::Slider("smoothnessSlider"));
    addAndMakeVisible(smoothnessSlider.get());
    smoothnessSlider->setRange(0, 100, 0.01);
    smoothnessSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    smoothnessSlider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    smoothnessSlider->addListener(this);

    smoothnessSlider->setBounds(250, 330, 190, 100);
    smoothnessSlider->setValue(10.0);

    convertBtn.reset(new juce::TextButton("convertBtn"));
    addAndMakeVisible(convertBtn.get());
    convertBtn->setButtonText(TRANS("Convert"));
    convertBtn->addListener(this);
    convertBtn->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2f31ba));

    convertBtn->setBounds(10, 510, 500, 20);

    playBtn.reset(new juce::TextButton("playBtn"));
    addAndMakeVisible(playBtn.get());
    playBtn->setButtonText(TRANS("Play"));
    playBtn->onClick = [this]
    { playButtonClicked(); };
    playBtn->setEnabled(false);
    playBtn->setColour(juce::TextButton::buttonColourId, juce::Colours::green);

    playBtn->setBounds(10, 550, 500, 20);

    pauseBtn.reset(new juce::TextButton("pauseBtn"));
    addAndMakeVisible(pauseBtn.get());
    pauseBtn->setButtonText(TRANS("Pause"));
    pauseBtn->onClick = [this]
    { playButtonClicked(); };
    pauseBtn->setEnabled(false);
    pauseBtn->setColour(juce::TextButton::buttonColourId, juce::Colours::red);

    pauseBtn->setBounds(10, 580, 500, 20);

    downloadBtn.reset(new juce::TextButton("downloadBtn"));
    addAndMakeVisible(downloadBtn.get());
    downloadBtn->setButtonText(TRANS("Download"));
    downloadBtn->addListener(this);
    downloadBtn->setColour(juce::TextButton::buttonColourId, juce::Colours::blue);

    downloadBtn->setBounds(10, 750, 500, 20);

    setSize (520, 880);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener (this);

    setAudioChannels (2, 2);

    //[UserPreSize]
    //[/UserPreSize]

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GUIComponent::~GUIComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    convertBtn = nullptr;

    smoothnessSlider = nullptr;
    thresholdSlider = nullptr;

    playBtn = nullptr;
    pauseBtn = nullptr;

    downloadBtn = nullptr;

    shutdownAudio();

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}


//==============================================================================
void GUIComponent::paint(Graphics &g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(Colour(0xff333236));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUIComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    openButtonT.setBounds (10, 10, getWidth() - 20, 20);
    playButtonT.setBounds (10, 40, getWidth() - 20, 20);
    stopButtonT.setBounds (10, 70, getWidth() - 20, 20);

    juce::Rectangle<int> thumbnailBounds (10, 100, getWidth() - 20, 100);
    thumbnailComp.setBounds (thumbnailBounds);
    positionOverlay.setBounds (thumbnailBounds);

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GUIComponent::sliderValueChanged(Slider *sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == thresholdSlider.get())
    {
        //[UserButtonCode_thresholdSlider] -- add your slider handler code here..

        if (processor->isFileLoaded())
            processor->processOnsets();

        thresholdIndex->sendThresholdValue(thresholdSlider->getValue());

        if (processor->isFileLoaded())
            processor->processPeaks(smoothnessSlider->getValue());
        if (processor->isFileLoaded())
            processor->extractPeaks();

        sleep(2);

        //[/UserButtonCode_thresholdSlider]
    }

    if (sliderThatWasMoved == smoothnessSlider.get())
    {
        //[UserButtonCode_smoothnessSlider] -- add your slider handler code here..

        if (processor->isFileLoaded())
            processor->processOnsets();

        if (processor->isFileLoaded())
            processor->processPeaks(smoothnessSlider->getValue());

        // peaksModelization->sendSmoothnessValue(smoothnessSlider->getValue());

        if (processor->isFileLoaded())
            processor->extractPeaks();

        sleep(2);

        //[/UserButtonCode_smoothnessSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GUIComponent::buttonClicked(Button *buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == convertBtn.get())
    {
        //[UserButtonCode_convertBtn] -- add your button handler code here..

        if (status) {
            std::cout << "convertBtn pressed" << std::endl;
            processor->processAudioTrack();
        }

        //[/UserButtonCode_convertBtn]
    }

    if (buttonThatWasClicked == downloadBtn.get())
    {
        //[UserButtonCode_downloadBtn] -- add your button handler code here..

        if (status) {
            std::cout << "downloadBtn pressed" << std::endl;
            processor->downloadProcessedFile();
        }

        //[/UserButtonCode_downloadBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}
