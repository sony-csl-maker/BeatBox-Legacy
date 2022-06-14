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
        positionOverlay (transportSource),
        thumbnailResultComp(512, formatManager, thumbnailCache),
        onsetThumbnail(512, formatManager, thumbnailCache)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    // thumbnailOriginal.reset(new OriginalThumbnailComponent());
    // addAndMakeVisible(thumbnailOriginal.get());

    startTimer(100);

    // getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::antiquewhite);
    // setLookAndFeel(&otherLookAndFeel);


    addAndMakeVisible (&openButtonT);
    openButtonT.setButtonText ("Load a file");
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

    drumifyToggle.reset (new juce::ToggleButton ("drumify"));
    addAndMakeVisible (drumifyToggle.get());
    drumifyToggle->addListener (this);
    drumifyToggle->setBounds(230, 420, 150, 24);


    // addAndMakeVisible (&thumbnailComp);
    // addAndMakeVisible (&positionOverlay);

    addAndMakeVisible(&thumbnailResultComp);

    addAndMakeVisible(&onsetThumbnail);



    ////////////////////////////////////////////////////////////////////////////

    thresholdIndex.reset(new SimpleThresholdIndex());
    addAndMakeVisible(thresholdIndex.get());
    thresholdIndex->setBounds(10, 70, 500, 100);

    peaksModelization.reset(new SimplePeakModelization());
    // addAndMakeVisible(peaksModelization.get());
    peaksModelization->setBounds(10, 175, 500, 40);

    thresholdSlider.reset(new juce::Slider("thresholdSlider"));
    addAndMakeVisible(thresholdSlider.get());
    thresholdSlider->setRange(0, 10, 0.01);
    thresholdSlider->setTooltip(TRANS("Use it to ignore peaks under a certain energy"));
    thresholdSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    thresholdSlider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    thresholdSlider->setChangeNotificationOnlyOnRelease(true);
    thresholdSlider->addListener(this);

    thresholdSlider->setBounds(70, 310, 190, 100);
    thresholdSlider->setValue(5);
    thresholdSlider->setLookAndFeel(&otherLookAndFeel);

    smoothnessSlider.reset(new juce::Slider("smoothnessSlider"));
    addAndMakeVisible(smoothnessSlider.get());
    smoothnessSlider->setRange(0, 100, 1);
    smoothnessSlider->setTooltip(TRANS("Use it to set the range used to determine the maximum value of each peaks pack"));
    smoothnessSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    smoothnessSlider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    smoothnessSlider->setChangeNotificationOnlyOnRelease(true);
    smoothnessSlider->addListener(this);

    smoothnessSlider->setBounds(250, 310, 190, 100);
    smoothnessSlider->setValue(50);
    smoothnessSlider->setLookAndFeel(&otherLookAndFeel);

    convertBtn.reset(new juce::TextButton("convertBtn"));
    addAndMakeVisible(convertBtn.get());
    convertBtn->setButtonText(TRANS("Convert"));
    convertBtn->addListener(this);
    convertBtn->setEnabled(true);
    convertBtn->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2f31ba));

    convertBtn->setBounds(180, 470, 520 - 360, 30);

    addAndMakeVisible(&playBtn);
    playBtn.setButtonText(TRANS("Play"));
    playBtn.onClick = [this]
    { playButtonClicked(); };
    playBtn.setEnabled(false);
    playBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::green);

    playBtn.setBounds(200, 650, 50, 30);

    addAndMakeVisible(&stopBtn);
    stopBtn.setButtonText(TRANS("Stop"));
    stopBtn.onClick = [this]
    { stopButtonClicked(); };
    stopBtn.setEnabled(false);
    stopBtn.setColour(juce::TextButton::buttonColourId, juce::Colours::red);

    stopBtn.setBounds(270, 650, 50, 30);

    downloadBtn.reset(new juce::TextButton("downloadBtn"));
    addAndMakeVisible(downloadBtn.get());
    downloadBtn->setButtonText(TRANS("Download"));
    downloadBtn->addListener(this);
    downloadBtn->setEnabled(false);
    downloadBtn->setColour(juce::TextButton::buttonColourId, juce::Colours::orange);

    downloadBtn->setBounds(180, 750, 520 - 360, 30);

    setSize (520, 880);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener (this);
    resultTransportSource.addChangeListener(this);

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

    downloadBtn = nullptr;

    drumifyToggle = nullptr;

    shutdownAudio();

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}


//==============================================================================
void GUIComponent::paint(Graphics &g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colours::black);

    {
        float x = 0, y = 0, width = 520.0f, height = 480.0f;
        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle (x, y, width, height, 10.000f, 5.000f);
    }
    {
        float x = 0, y = 490, width = 520.0f, height = 300.0f;
        g.drawRoundedRectangle (x, y, width, height, 10.000f, 5.000f);
    }


    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUIComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    openButtonT.setBounds (180, 10, getWidth() - 360, 30);
    playButtonT.setBounds(200, 250, 50, 30);
    stopButtonT.setBounds(270, 250, 50, 30);


    juce::Rectangle<int> thumbnailBounds (10, 70, getWidth() - 20, 100);
    thumbnailComp.setBounds (thumbnailBounds);
    positionOverlay.setBounds (thumbnailBounds);

    juce::Rectangle<int> onsetThumbnailBounds (10, 190, getWidth() - 20, 100);
    onsetThumbnail.setBounds (onsetThumbnailBounds);

    juce::Rectangle<int> thumbnailResultBounds (10, 530, getWidth() - 20, 100);
    thumbnailResultComp.setBounds (thumbnailResultBounds);

    // positionOverlay.setBounds (thumbnailBounds);

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

        if (processor->isFileLoaded()) {
            processor->processOnsets();
            processor->processPeaks(smoothnessSlider->getValue(), thresholdSlider->getValue());
            processor->extractPeaks();
        }

        //[/UserButtonCode_thresholdSlider]
    }

    if (sliderThatWasMoved == smoothnessSlider.get())
    {
        //[UserButtonCode_smoothnessSlider] -- add your slider handler code here..

        if (processor->isFileLoaded()) {
            processor->processOnsets();
            processor->processPeaks(smoothnessSlider->getValue(), thresholdSlider->getValue());
            processor->extractPeaks();
        }

        //[/UserButtonCode_smoothnessSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GUIComponent::buttonClicked(Button *buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == drumifyToggle.get())
    {
        //[UserButtonCode_juce__toggleButton] -- add your button handler code here..

        std::cout << "toggle drumify pressed" << std::endl;
        processor->toggleDrumify();

        //[/UserButtonCode_juce__toggleButton]
    }

    if (buttonThatWasClicked == convertBtn.get())
    {
        //[UserButtonCode_convertBtn] -- add your button handler code here..

        std::cout << "convertBtn pressed" << std::endl;
        processor->processAudioTrack();

        downloadBtn->setEnabled(true);
        thumbnailResultComp.setSource(processor->getConvertBuffer());
        playBtn.setEnabled(true);

        pathToFile = processor->getFilename();

        processor->setFilename(".temp/" + pathToFile);
        processor->downloadProcessedFile();

        openAudioTrack();

        //[/UserButtonCode_convertBtn]
    }

    if (buttonThatWasClicked == downloadBtn.get())
    {
        //[UserButtonCode_downloadBtn] -- add your button handler code here..

        std::cout << "downloadBtn pressed" << std::endl;

        processor->setFilename(pathToFile);
        processor->downloadProcessedFile();

        //[/UserButtonCode_downloadBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void GUIComponent::timerCallback()
{
    thresholdIndex->sendThresholdValue(thresholdSlider->getValue());
    peaksModelization->sendPeaksIndex(processor->getPeaksIndex());
}