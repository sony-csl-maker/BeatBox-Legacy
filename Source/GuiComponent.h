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
#include <JuceHeader.h>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GuiComponent  : public Component,
                      public Slider::Listener,
                      public Button::Listener
{
public:
    //==============================================================================
    GuiComponent ();
    ~GuiComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Slider> smoothNessSlider;
    std::unique_ptr<TextButton> convertBtn;
    std::unique_ptr<ToggleButton> dumifyBtn;
    std::unique_ptr<Slider> thresholdSlider;
    std::unique_ptr<Label> smoothnessLabel;
    std::unique_ptr<Label> thresholdLabel;
    std::unique_ptr<Label> snareLabel;
    std::unique_ptr<TextButton> FileBtn;
    std::unique_ptr<TextButton> playBtn;
    std::unique_ptr<TextButton> PauseBtn;
    std::unique_ptr<TextButton> playConvertBtn;
    std::unique_ptr<TextButton> PauseConvertBtn;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuiComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

