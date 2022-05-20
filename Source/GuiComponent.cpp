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

    thumbnailOriginal.reset(new OriginalThumbnailComponent());
    addAndMakeVisible(thumbnailOriginal.get());

    convertBtn.reset (new TextButton ("convertBtn"));
    addAndMakeVisible (convertBtn.get());
    convertBtn->setButtonText (TRANS("Convert"));
    convertBtn->addListener (this);
    convertBtn->setColour (TextButton::buttonColourId, Colour (0xff2f31ba));

    convertBtn->setBounds (10, 320, 500, 20);

    thresholdSlider.reset (new juce::Slider ("thresholdSlider"));
    addAndMakeVisible (thresholdSlider.get());
    thresholdSlider->setRange (0, 10, 0.01);
    thresholdSlider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    thresholdSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    thresholdSlider->addListener (this);

    thresholdSlider->setBounds (50, 350, 190, 100);

    smoothnessSlider.reset (new juce::Slider ("smoothnessSlider"));
    addAndMakeVisible (smoothnessSlider.get());
    smoothnessSlider->setRange (0, 10, 0.01);
    smoothnessSlider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    smoothnessSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    smoothnessSlider->addListener (this);

    smoothnessSlider->setBounds (240, 350, 190, 100);

    downloadBtn.reset (new TextButton ("downloadBtn"));
    addAndMakeVisible (downloadBtn.get());
    downloadBtn->setButtonText (TRANS("Download"));
    downloadBtn->addListener (this);
    downloadBtn->setColour (TextButton::buttonColourId, Colours::blue);

    downloadBtn->setBounds (10, 800, 500, 20);

    //[UserPreSize]
    //[/UserPreSize]


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GuiComponent::~GuiComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    convertBtn = nullptr;

    smoothnessSlider = nullptr;
    thresholdSlider = nullptr;

    downloadBtn = nullptr;

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
        int x = 10, y = 210, width = 500, height = 100;
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

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GuiComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == thresholdSlider.get())
    {
        //[UserButtonCode_thresholdSlider] -- add your slider handler code here..

        std::cout << "thresholdSlider moved" << std::endl;
        std::cout << "value = " << std::to_string(thresholdSlider->getValue()) << std::endl;

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

void GuiComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == convertBtn.get())
    {
        //[UserButtonCode_convertBtn] -- add your button handler code here..

        std::cout << "convertBtn pressed" << std::endl;

        // _convertedThumbnail->loadConvertedFile();

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
  <SLIDER name="smoothness" id="8ffb4b136762fc90" memberName="smoothnessSlider"
          virtualName="" explicitFocusOrder="0" pos="57.1% 25.502% 176 96"
          bkgcol="ffe9edee" trackcol="ffffffff" rotarysliderfill="ffffffff"
          rotaryslideroutline="ff000000" min="0.0" max="10.0" int="0.01"
          style="Rotary" textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="Convert-button" id="e9200f066f3fc373" memberName="convertBtn"
              virtualName="" explicitFocusOrder="0" pos="200 480 150 22" bgColOff="ff2f31ba"
              buttonText="Convert" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="dumify-toggle" id="9a95dc488e8dd025" memberName="dumifyToggle"
                virtualName="" explicitFocusOrder="0" pos="67.919% 40.614% 96 24"
                buttonText="dumify++" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="threshold" id="4c12cd774ab38789" memberName="thresholdSlider"
          virtualName="" explicitFocusOrder="0" pos="118.182% 0% 176 96"
          posRelativeX="8ffb4b136762fc90" posRelativeY="8ffb4b136762fc90"
          bkgcol="ffffffff" trackcol="ffffffff" rotarysliderfill="ffffffff"
          rotaryslideroutline="ff000000" min="0.0" max="10.0" int="0.01"
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
  <TEXTBUTTON name="play" id="914cdc77d0517b4f" memberName="playOrigBtn" virtualName=""
              explicitFocusOrder="0" pos="504 160 64 22" bgColOff="ff068603"
              buttonText="Play" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="pause" id="76478d894edb4b85" memberName="pauseOrigBtn"
              virtualName="" explicitFocusOrder="0" pos="504 200 64 22" bgColOff="ffa90000"
              buttonText="Pause" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="play" id="aa3e6b34fe2bbd3" memberName="playConvertBtn"
              virtualName="" explicitFocusOrder="0" pos="507 602 61 22" bgColOff="ff068603"
              buttonText="Play" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="pause" id="6d50dbac2a08c940" memberName="pauseConvertBtn"
              virtualName="" explicitFocusOrder="0" pos="507 642 61 22" bgColOff="ffa90000"
              buttonText="Pause" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="downloadBtn" id="cb7d643fc41c37d5" memberName="downloadBtn"
              virtualName="" explicitFocusOrder="0" pos="200 728 150 22" bgColOff="ff0000ff"
              buttonText="Download" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

