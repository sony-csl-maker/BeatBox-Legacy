/*
  ==============================================================================
    This file contains the basic startup code for a JUCE application.
  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "ProcessorComponent.h"

#include "ThumbnailTools/SimpleThresholdIndex.h"
#include "ThumbnailTools/SimplePeakModelization.h"
//[/Headers]



//==============================================================================
class GUIComponent  : public juce::Component,
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

    void paint (juce::Graphics& g);
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[Modules]   -- You can add your own custom modules in this section.
    std::unique_ptr<ProcessorComponent> processorComponent;

    std::unique_ptr<SimpleThresholdIndex> thresholdIndex;
    std::unique_ptr<SimplePeakModelization> peaksModelization;
    //[/Modules]

    //[Variables]   -- You can add your own custom variables in this section.
    bool fileIsLoaded = false;


    //[/Variables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> convertBtn;
    std::unique_ptr<juce::TextButton> simulateBtn;

    std::unique_ptr<juce::ToggleButton> dumifyToggle;

    std::unique_ptr<juce::Slider> smoothnessSlider;
    std::unique_ptr<juce::Slider> thresholdSlider;

    std::unique_ptr<juce::Label> smoothnessLabel;
    std::unique_ptr<juce::Label> thresholdLabel;
    std::unique_ptr<juce::Label> snareLabel;

    std::unique_ptr<juce::TextButton> fileBtn;
    std::unique_ptr<juce::TextButton> playOrigBtn;
    std::unique_ptr<juce::TextButton> pauseOrigBtn;
    std::unique_ptr<juce::TextButton> playConvertBtn;
    std::unique_ptr<juce::TextButton> pauseConvertBtn;
    std::unique_ptr<juce::TextButton> downloadBtn;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUIComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

