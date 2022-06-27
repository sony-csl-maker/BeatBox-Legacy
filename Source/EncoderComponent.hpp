/*
** EPITECH PROJECT, 2022
** SONYCSL_REPO
** File description:
** EncoderComponent
*/

#pragma once

// Juce
#include <JuceHeader.h>
#include "ProcessorComponent.h"

class EncoderComponent : public juce::ThreadWithProgressWindow
{
    public:
        EncoderComponent(std::function<void(long unsigned int)> callBack, long unsigned int size) : ThreadWithProgressWindow("busy ...", true, true)
        {
            _callBack = callBack;
            _size = size;
        }

        void run()
        {
            for (long unsigned int index = 0; index < _size; index++) {
                if (threadShouldExit())
                    break;
                setProgress(index / 100);
                _callBack(index);
            }
        }

    private:
        std::function<void(long unsigned int)> _callBack;
        long unsigned int _size;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncoderComponent)
};