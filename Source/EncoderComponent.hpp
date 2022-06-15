/*
** EPITECH PROJECT, 2022
** SONYCSL_REPO
** File description:
** EncoderComponent
*/

#pragma once

// Juce
#include <JuceHeader.h>

class EncoderComponent : public juce::ThreadWithProgressWindow
{
    public:
        EncoderComponent() : ThreadWithProgressWindow("busy ...", true, true)
        {
        }

        void run()
        {
            for (int i = 0; i < 1000000; ++i)
            {
                // must check this as often as possible, because this is
                // how we know if the user's pressed 'cancel'
                if (threadShouldExit())
                    break;

                // this will update the progress bar on the dialog box
                setProgress (i / (double) 100);


                //   ... do the business here...
            }
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EncoderComponent)
};