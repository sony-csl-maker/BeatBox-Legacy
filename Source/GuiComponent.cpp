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

#include "GuiComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GuiComponent::GuiComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    smoothNessSlider.reset (new Slider ("smoothness"));
    addAndMakeVisible (smoothNessSlider.get());
    smoothNessSlider->setRange (0, 10, 0);
    smoothNessSlider->setSliderStyle (Slider::Rotary);
    smoothNessSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    smoothNessSlider->setColour (Slider::backgroundColourId, Colour (0xffe9edee));
    smoothNessSlider->setColour (Slider::trackColourId, Colours::white);
    smoothNessSlider->setColour (Slider::rotarySliderFillColourId, Colours::white);
    smoothNessSlider->setColour (Slider::rotarySliderOutlineColourId, Colours::black);
    smoothNessSlider->addListener (this);

    convertBtn.reset (new TextButton ("Convert-button"));
    addAndMakeVisible (convertBtn.get());
    convertBtn->setButtonText (TRANS("Convert"));
    convertBtn->addListener (this);
    convertBtn->setColour (TextButton::buttonColourId, Colour (0xff2f31ba));

    convertBtn->setBounds (200, 480, 150, 22);

    dumifyBtn.reset (new ToggleButton ("dumify-toggle"));
    addAndMakeVisible (dumifyBtn.get());
    dumifyBtn->setButtonText (TRANS("dumify++"));
    dumifyBtn->addListener (this);

    thresholdSlider.reset (new Slider ("threshold"));
    addAndMakeVisible (thresholdSlider.get());
    thresholdSlider->setRange (0, 10, 0);
    thresholdSlider->setSliderStyle (Slider::Rotary);
    thresholdSlider->setTextBoxStyle (Slider::TextBoxRight, false, 80, 20);
    thresholdSlider->setColour (Slider::backgroundColourId, Colours::white);
    thresholdSlider->setColour (Slider::trackColourId, Colours::white);
    thresholdSlider->setColour (Slider::rotarySliderFillColourId, Colours::white);
    thresholdSlider->setColour (Slider::rotarySliderOutlineColourId, Colours::black);
    thresholdSlider->addListener (this);

    smoothnessLabel.reset (new Label ("smoothnessl",
                                      TRANS("Smoothness")));
    addAndMakeVisible (smoothnessLabel.get());
    smoothnessLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    smoothnessLabel->setJustificationType (Justification::centredLeft);
    smoothnessLabel->setEditable (false, false, false);
    smoothnessLabel->setColour (TextEditor::textColourId, Colours::black);
    smoothnessLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    thresholdLabel.reset (new Label ("threshold",
                                     TRANS("Threshold")));
    addAndMakeVisible (thresholdLabel.get());
    thresholdLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    thresholdLabel->setJustificationType (Justification::centredLeft);
    thresholdLabel->setEditable (false, false, false);
    thresholdLabel->setColour (TextEditor::textColourId, Colours::black);
    thresholdLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    snareLabel.reset (new Label ("snare",
                                 TRANS("Snare")));
    addAndMakeVisible (snareLabel.get());
    snareLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    snareLabel->setJustificationType (Justification::centred);
    snareLabel->setEditable (false, false, false);
    snareLabel->setColour (TextEditor::textColourId, Colours::black);
    snareLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    snareLabel->setBounds (-104, -40, 88, 24);

    FileBtn.reset (new TextButton ("File"));
    addAndMakeVisible (FileBtn.get());
    FileBtn->setButtonText (TRANS("Open..."));
    FileBtn->addListener (this);
    FileBtn->setColour (TextButton::buttonColourId, Colour (0xff3b3b3b));

    FileBtn->setBounds (200, 56, 150, 22);

    playBtn.reset (new TextButton ("play"));
    addAndMakeVisible (playBtn.get());
    playBtn->setButtonText (TRANS("Play"));
    playBtn->addListener (this);
    playBtn->setColour (TextButton::buttonColourId, Colour (0xff068603));

    playBtn->setBounds (504, 160, 64, 22);

    PauseBtn.reset (new TextButton ("pause"));
    addAndMakeVisible (PauseBtn.get());
    PauseBtn->setButtonText (TRANS("Pause"));
    PauseBtn->addListener (this);
    PauseBtn->setColour (TextButton::buttonColourId, Colour (0xffa90000));

    PauseBtn->setBounds (504, 200, 64, 22);

    playConvertBtn.reset (new TextButton ("play"));
    addAndMakeVisible (playConvertBtn.get());
    playConvertBtn->setButtonText (TRANS("Play"));
    playConvertBtn->addListener (this);
    playConvertBtn->setColour (TextButton::buttonColourId, Colour (0xff068603));

    playConvertBtn->setBounds (507, 602, 61, 22);

    PauseConvertBtn.reset (new TextButton ("pause"));
    addAndMakeVisible (PauseConvertBtn.get());
    PauseConvertBtn->setButtonText (TRANS("Pause"));
    PauseConvertBtn->addListener (this);
    PauseConvertBtn->setColour (TextButton::buttonColourId, Colour (0xffa90000));

    PauseConvertBtn->setBounds (507, 642, 61, 22);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1400, 800);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GuiComponent::~GuiComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    smoothNessSlider = nullptr;
    convertBtn = nullptr;
    dumifyBtn = nullptr;
    thresholdSlider = nullptr;
    smoothnessLabel = nullptr;
    thresholdLabel = nullptr;
    snareLabel = nullptr;
    FileBtn = nullptr;
    playBtn = nullptr;
    PauseBtn = nullptr;
    playConvertBtn = nullptr;
    PauseConvertBtn = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GuiComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff333236));

    {
        int x = 76, y = 148, width = 392, height = 92;
        Colour fillColour = Colour (0xff2a78a5);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 76, y = 588, width = 392, height = 92;
        Colour fillColour = Colour (0xff2a78a5);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 76, y = 308, width = 392, height = 92;
        Colour fillColour = Colour (0xff2a78a5);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GuiComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    smoothNessSlider->setBounds (proportionOfWidth (0.5710f), proportionOfHeight (0.2550f), 176, 96);
    dumifyBtn->setBounds (proportionOfWidth (0.6792f), proportionOfHeight (0.4061f), 96, 24);
    thresholdSlider->setBounds (proportionOfWidth (0.5710f) + roundToInt (176 * 1.1818f), proportionOfHeight (0.2550f) + roundToInt (96 * 0.0000f), 176, 96);
    smoothnessLabel->setBounds (proportionOfWidth (0.5710f) + 80, proportionOfHeight (0.2550f) + -40, 88, 24);
    thresholdLabel->setBounds ((proportionOfWidth (0.5710f) + roundToInt (176 * 1.1818f)) + 8, (proportionOfHeight (0.2550f) + roundToInt (96 * 0.0000f)) + -40, 88, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GuiComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == smoothNessSlider.get())
    {
        //[UserSliderCode_smoothNessSlider] -- add your slider handling code here..
        //[/UserSliderCode_smoothNessSlider]
    }
    else if (sliderThatWasMoved == thresholdSlider.get())
    {
        //[UserSliderCode_thresholdSlider] -- add your slider handling code here..
        //[/UserSliderCode_thresholdSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GuiComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == convertBtn.get())
    {
        //[UserButtonCode_convertBtn] -- add your button handler code here..
        //[/UserButtonCode_convertBtn]
    }
    else if (buttonThatWasClicked == dumifyBtn.get())
    {
        //[UserButtonCode_dumifyBtn] -- add your button handler code here..
        //[/UserButtonCode_dumifyBtn]
    }
    else if (buttonThatWasClicked == FileBtn.get())
    {
        //[UserButtonCode_FileBtn] -- add your button handler code here..
        //[/UserButtonCode_FileBtn]
    }
    else if (buttonThatWasClicked == playBtn.get())
    {
        //[UserButtonCode_playBtn] -- add your button handler code here..
        //[/UserButtonCode_playBtn]
    }
    else if (buttonThatWasClicked == PauseBtn.get())
    {
        //[UserButtonCode_PauseBtn] -- add your button handler code here..
        //[/UserButtonCode_PauseBtn]
    }
    else if (buttonThatWasClicked == playConvertBtn.get())
    {
        //[UserButtonCode_playConvertBtn] -- add your button handler code here..
        //[/UserButtonCode_playConvertBtn]
    }
    else if (buttonThatWasClicked == PauseConvertBtn.get())
    {
        //[UserButtonCode_PauseConvertBtn] -- add your button handler code here..
        //[/UserButtonCode_PauseConvertBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GuiComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="1400" initialHeight="800">
  <BACKGROUND backgroundColour="ff333236">
    <RECT pos="76 148 392 92" fill="solid: ff2a78a5" hasStroke="0"/>
    <RECT pos="76 588 392 92" fill="solid: ff2a78a5" hasStroke="0"/>
    <RECT pos="76 308 392 92" fill="solid: ff2a78a5" hasStroke="0"/>
  </BACKGROUND>
  <SLIDER name="smoothness" id="8ffb4b136762fc90" memberName="smoothNessSlider"
          virtualName="" explicitFocusOrder="0" pos="57.1% 25.502% 176 96"
          bkgcol="ffe9edee" trackcol="ffffffff" rotarysliderfill="ffffffff"
          rotaryslideroutline="ff000000" min="0.0" max="10.0" int="0.0"
          style="Rotary" textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="Convert-button" id="e9200f066f3fc373" memberName="convertBtn"
              virtualName="" explicitFocusOrder="0" pos="200 480 150 22" bgColOff="ff2f31ba"
              buttonText="Convert" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="dumify-toggle" id="9a95dc488e8dd025" memberName="dumifyBtn"
                virtualName="" explicitFocusOrder="0" pos="67.919% 40.614% 96 24"
                buttonText="dumify++" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="threshold" id="4c12cd774ab38789" memberName="thresholdSlider"
          virtualName="" explicitFocusOrder="0" pos="118.182% 0% 176 96"
          posRelativeX="8ffb4b136762fc90" posRelativeY="8ffb4b136762fc90"
          bkgcol="ffffffff" trackcol="ffffffff" rotarysliderfill="ffffffff"
          rotaryslideroutline="ff000000" min="0.0" max="10.0" int="0.0"
          style="Rotary" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="smoothnessl" id="8996091298f6f4bb" memberName="smoothnessLabel"
         virtualName="" explicitFocusOrder="0" pos="80 -40 88 24" posRelativeX="8ffb4b136762fc90"
         posRelativeY="8ffb4b136762fc90" edTextCol="ff000000" edBkgCol="0"
         labelText="Smoothness" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="1" italic="0" justification="33" typefaceStyle="Bold"/>
  <LABEL name="threshold" id="13d86ca6ed8560c6" memberName="thresholdLabel"
         virtualName="" explicitFocusOrder="0" pos="8 -40 88 24" posRelativeX="4c12cd774ab38789"
         posRelativeY="4c12cd774ab38789" edTextCol="ff000000" edBkgCol="0"
         labelText="Threshold" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="1" italic="0" justification="33" typefaceStyle="Bold"/>
  <LABEL name="snare" id="5ab7062d7d3463cd" memberName="snareLabel" virtualName=""
         explicitFocusOrder="0" pos="-104 -40 88 24" posRelativeX="4379536476d3a333"
         posRelativeY="4379536476d3a333" edTextCol="ff000000" edBkgCol="0"
         labelText="Snare" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="1" italic="0" justification="36" typefaceStyle="Bold"/>
  <TEXTBUTTON name="File" id="1957e99fe10396a7" memberName="FileBtn" virtualName=""
              explicitFocusOrder="0" pos="200 56 150 22" bgColOff="ff3b3b3b"
              buttonText="Open..." connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="play" id="914cdc77d0517b4f" memberName="playBtn" virtualName=""
              explicitFocusOrder="0" pos="504 160 64 22" bgColOff="ff068603"
              buttonText="Play" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="pause" id="76478d894edb4b85" memberName="PauseBtn" virtualName=""
              explicitFocusOrder="0" pos="504 200 64 22" bgColOff="ffa90000"
              buttonText="Pause" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="play" id="aa3e6b34fe2bbd3" memberName="playConvertBtn"
              virtualName="" explicitFocusOrder="0" pos="507 602 61 22" bgColOff="ff068603"
              buttonText="Play" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="pause" id="6d50dbac2a08c940" memberName="PauseConvertBtn"
              virtualName="" explicitFocusOrder="0" pos="507 642 61 22" bgColOff="ffa90000"
              buttonText="Pause" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

