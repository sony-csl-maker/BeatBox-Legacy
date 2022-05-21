#include "ProcessorComponent.h"

//==============================================================================
ProcessorComponent::ProcessorComponent()
{
    startTimer (20);
}

ProcessorComponent::~ProcessorComponent()
{

}

//==============================================================================
void ProcessorComponent::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ProcessorComponent::resized()
{

}

void ProcessorComponent::timerCallback()
{

}

void ProcessorComponent::updateThreshold()
{

}

void ProcessorComponent::updateSmoothness()
{

}
