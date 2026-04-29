#pragma once

#include "BinaryData.h"
#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
class WaveformDisplayComponent : public juce::Component
{
public:
    WaveformDisplayComponent() {}

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        // Draw the background of the box
        g.setColour (juce::Colours::black);
        g.fillRect (bounds);

        // Draw a dummy sine waveform
        g.setColour (juce::Colours::cyan);
        juce::Path p;
        auto width = bounds.getWidth();
        auto height = bounds.getHeight();
        auto centerY = height / 2.0f;

        p.startNewSubPath (0.0f, centerY);
        for (float x = 0.0f; x < width; x += 1.0f)
        {
            float phase = (x / width) * juce::MathConstants<float>::twoPi * 2.0f; // 2 cycles
            float y = centerY - std::sin (phase) * (height * 0.4f);
            p.lineTo (x, y);
        }

        g.strokePath (p, juce::PathStrokeType (2.0f));
    }
};

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void visibilityChanged() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::MidiKeyboardComponent keyboardComponent;
    juce::GroupComponent osc1Group;
    WaveformDisplayComponent waveform1Display;
    juce::ComboBox waveform1Selector;
    juce::GroupComponent osc2Group;
    WaveformDisplayComponent waveform2Display;
    juce::ComboBox waveform2Selector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1WaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2WaveAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
