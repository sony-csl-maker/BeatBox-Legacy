#include "ProcessorComponent.h"

//==============================================================================
ProcessorComponent::ProcessorComponent()
    : _state(Stopped)
{
    addAndMakeVisible(&_openButton);
    _openButton.setButtonText("Open from filesystem ...");
    _openButton.onClick = [this]
    { openButtonClicked(); };

    addAndMakeVisible(&_playButton);
    _playButton.setButtonText("Play");
    _playButton.onClick = [this]
    { playButtonClicked(); };
    _playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    _playButton.setEnabled(false);

    addAndMakeVisible(&_stopButton);
    _stopButton.setButtonText("Stop");
    _stopButton.onClick = [this]
    { stopButtonClicked(); };
    _stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    _stopButton.setEnabled(false);

    addAndMakeVisible(&_currentPositionLabel);
    _currentPositionLabel.setText("Stopped", juce::dontSendNotification);

    _formatManager.registerBasicFormats();
    _transportSource.addChangeListener(this);

    setAudioChannels(2, 2);
    startTimer (20);
}

ProcessorComponent::~ProcessorComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void ProcessorComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    _transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void ProcessorComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    if (_readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    _transportSource.getNextAudioBlock(bufferToFill);
}

void ProcessorComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    _transportSource.releaseResources();
}

//==============================================================================
void ProcessorComponent::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ProcessorComponent::resized()
{
    // This is called when the ProcessorComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    _openButton.setBounds(10, 10, getWidth() - 20, 20);
    _playButton.setBounds(10, 40, getWidth() - 20, 20);
    _stopButton.setBounds(10, 70, getWidth() - 20, 20);
    _currentPositionLabel.setBounds(10, 130, getWidth() - 20, 20);
}

void ProcessorComponent::timerCallback()
{
    if (_transportSource.isPlaying())
    {
        juce::RelativeTime position(_transportSource.getCurrentPosition());

        auto minutes = ((int)position.inMinutes()) % 60;
        auto seconds = ((int)position.inSeconds()) % 60;
        auto millis = ((int)position.inMilliseconds()) % 1000;

        auto positionString = juce::String::formatted("%02d:%02d:%03d", minutes, seconds, millis);

        _currentPositionLabel.setText(positionString, juce::dontSendNotification);
    }
    else
    {
        _currentPositionLabel.setText("Paused", juce::dontSendNotification);
    }
}

void ProcessorComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &_transportSource)
    {
        if (_transportSource.isPlaying())
            changeState(Playing);
        else
            changeState(Stopped);
    }
}
