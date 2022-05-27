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

#include "ProcessorComponent.h"

#include "Thumbnails/OriginalThumbnailComponent.h"

#include "Thumbnails/Tools/SimpleThresholdIndex.h"
#include "Thumbnails/Tools/SimplePeakModelization.h"
#include "Thumbnails/Tools/SimpleThumbnailComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
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
    std::unique_ptr<ProcessorComponent> processor = std::make_unique<ProcessorComponent>();

    std::unique_ptr<OriginalThumbnailComponent> thumbnailOriginal;

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
