/*
  ==============================================================================
    This file contains the basic startup code for a JUCE application.
  ==============================================================================
*/

#include <memory>
#include <iostream>

#include "GUIComponent.h"


//==============================================================================
GUIComponent::GUIComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    processorComponent.reset(new ProcessorComponent());
    addAndMakeVisible(processorComponent.get());

    thresholdIndex.reset(new SimpleThresholdIndex());
    addAndMakeVisible(thresholdIndex.get());
    thresholdIndex->setBounds(10, 100, 500, 100);

    peaksModelization.reset(new SimplePeakModelization());
    addAndMakeVisible(peaksModelization.get());
    peaksModelization->setBounds(10, 210, 500, 100);

    thresholdSlider.reset (new juce::Slider ("thresholdSlider"));
    addAndMakeVisible (thresholdSlider.get());
    thresholdSlider->setRange (0, 10, 0.01);
    thresholdSlider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    thresholdSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    thresholdSlider->addListener (this);

    thresholdSlider->setBounds (70, 330, 190, 100);
    thresholdSlider->setValue(9.5);

    smoothnessSlider.reset (new juce::Slider ("smoothnessSlider"));
    addAndMakeVisible (smoothnessSlider.get());
    smoothnessSlider->setRange (0, 10, 0.01);
    smoothnessSlider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    smoothnessSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    smoothnessSlider->addListener (this);

    smoothnessSlider->setBounds (250, 330, 190, 100);
    smoothnessSlider->setValue(0.17);

    simulateBtn.reset (new juce::TextButton ("simulateBtn"));
    addAndMakeVisible (simulateBtn.get());
    simulateBtn->setButtonText (TRANS("Simulate"));
    simulateBtn->addListener (this);
    simulateBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2f31ba));

    simulateBtn->setBounds (10, 470, 500, 20);

    convertBtn.reset (new juce::TextButton ("convertBtn"));
    addAndMakeVisible (convertBtn.get());
    convertBtn->setButtonText (TRANS("Convert"));
    convertBtn->addListener (this);
    convertBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2f31ba));

    convertBtn->setBounds (10, 510, 500, 20);

    downloadBtn.reset (new juce::TextButton ("downloadBtn"));
    addAndMakeVisible (downloadBtn.get());
    downloadBtn->setButtonText (TRANS("Download"));
    downloadBtn->addListener (this);
    downloadBtn->setColour (juce::TextButton::buttonColourId, juce::Colours::blue);

    downloadBtn->setBounds (10, 800, 500, 20);

    //[UserPreSize]
    //[/UserPreSize]


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GUIComponent::~GUIComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    simulateBtn = nullptr;
    convertBtn = nullptr;

    smoothnessSlider = nullptr;
    thresholdSlider = nullptr;

    downloadBtn = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GUIComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff333236));

    {
        int x = 10, y = 210, width = 500, height = 100;
        juce::Colour fillColour = juce::Colour (0xff2a78a5);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUIComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GUIComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == thresholdSlider.get())
    {
        //[UserButtonCode_thresholdSlider] -- add your slider handler code here..

        thresholdIndex->sendThresholdValue(thresholdSlider->getValue());

        //[/UserButtonCode_thresholdSlider]
    }

    if (sliderThatWasMoved == smoothnessSlider.get())
    {
        //[UserButtonCode_smoothnessSlider] -- add your slider handler code here..

        std::cout << "smoothnessSlider moved" << std::endl;
        std::cout << "value = " << std::to_string(smoothnessSlider->getValue()) << std::endl;

        //[/UserButtonCode_smoothnessSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GUIComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == simulateBtn.get())
    {
        //[UserButtonCode_simulateBtn] -- add your button handler code here..

        std::cout << "simulateBtn pressed" << std::endl;

        //[/UserButtonCode_simulateBtn]
    }

    if (buttonThatWasClicked == convertBtn.get())
    {
        //[UserButtonCode_convertBtn] -- add your button handler code here..

        std::cout << "convertBtn pressed" << std::endl;

        //[/UserButtonCode_convertBtn]
    }

    if (buttonThatWasClicked == downloadBtn.get())
    {
        //[UserButtonCode_downloadBtn] -- add your button handler code here..

        std::cout << "downloadBtn pressed" << std::endl;

        //[/UserButtonCode_downloadBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]
