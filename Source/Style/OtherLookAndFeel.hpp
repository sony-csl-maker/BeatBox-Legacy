/*
** EPITECH PROJECT, 2022
** SONYCSL_REPO
** File description:
** OtherLookAndFeel
*/

#ifndef OTHERLOOKANDFEEL_HPP_
#define OTHERLOOKANDFEEL_HPP_

#pragma once

#include <JuceHeader.h>

class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel()
    {
        setColour (juce::Slider::thumbColourId, juce::Colours::red);
    }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float) width  * 0.5f;
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour (juce::Colours::orange);
        g.fillEllipse (rx, ry, rw, rw);

        // outline
        g.setColour (juce::Colours::yellow);
        g.drawEllipse (rx, ry, rw, rw, 3.0f);

        juce::Path p;
        auto pointerLength = radius * 0.93f;
        auto pointerThickness = 2.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));

        // pointer
        g.setColour (juce::Colours::yellow);
        g.fillPath (p);
    }

    // void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
    //                            bool, bool isButtonDown) override
    // {
    //     auto radius = (float) juce::jmin (30 / 2, 30 / 2) - 4.0f;
    //     auto centreX = (float) 100 + (float) 30  * 0.5f;
    //     auto centreY = (float) 100 + (float) 30 * 0.5f;
    //     auto rx = centreX - radius;
    //     auto ry = centreY - radius;
    //     auto rw = radius * 2.0f;
    //     // auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    //     // fill
    //     g.setColour (backgroundColour);
    //     g.fillEllipse (rx, ry, rw, rw);
    //     button.setColour(juce::TextButton::buttonColourId, backgroundColour);
    //     g.setColour (juce::Colours::yellow);
    //     g.drawEllipse (rx, ry, rw, rw, 3.0f);
    // }
};


#endif /* !OTHERLOOKANDFEEL_HPP_ */
