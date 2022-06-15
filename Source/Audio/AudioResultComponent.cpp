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

#include "AudioResultComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AudioResultComponent::AudioResultComponent() :
        state (Stopped),
        thumbnailCache (5),
        thumbnailComp (512, formatManager, thumbnailCache),
        positionOverlay (transportSource)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (&thumbnailComp);
    addAndMakeVisible (&positionOverlay);

    ////////////////////////////////////////////////////////////////////////////

    addAndMakeVisible(&playButton);
    playButton.setButtonText(TRANS("Play"));
    playButton.onClick = [this]
    { playButtonClicked(); };
    playButton.setEnabled(false);
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);

    playButton.setBounds(200, 650, 50, 30);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText(TRANS("Stop"));
    stopButton.onClick = [this]
    { stopButtonClicked(); };
    stopButton.setEnabled(false);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);

    stopButton.setBounds(270, 650, 50, 30);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener (this);

    setAudioChannels (2, 2);

    //[UserPreSize]
    //[/UserPreSize]

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

AudioResultComponent::~AudioResultComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    shutdownAudio();

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}


//==============================================================================
void AudioResultComponent::paint(Graphics &g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    // g.fillAll(juce::Colours::black); // A VOIR

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AudioResultComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    juce::Rectangle<int> thumbnailBounds (10, 670, getWidth() - 20, 100);
    thumbnailComp.setBounds (thumbnailBounds);
    positionOverlay.setBounds (thumbnailBounds);

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}
