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

    int waveType = 0; // 0: Sine, 1: Triangle, 2: Square

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        // Draw the background of the box
        g.setColour (juce::Colours::black);
        g.fillRect (bounds);

        // Draw the waveform
        g.setColour (juce::Colours::cyan);
        juce::Path p;
        auto width = bounds.getWidth();
        auto height = bounds.getHeight();
        auto centerY = height / 2.0f;

        p.startNewSubPath (0.0f, centerY);
        for (float x = 0.0f; x < width; x += 1.0f)
        {
            float phase = (x / width) * juce::MathConstants<float>::twoPi * 2.0f; // 2 cycles
            float y = centerY;

            if (waveType == 0) // Sine
            {
                y -= std::sin (phase) * (height * 0.4f);
            }
            else if (waveType == 1) // Triangle
            {
                float normalizedPhase = phase / juce::MathConstants<float>::twoPi;
                float tri = (float) (2.0 * std::abs (2.0 * (normalizedPhase - std::floor (normalizedPhase + 0.5))) - 1.0);
                y -= tri * (height * 0.4f);
            }
            else if (waveType == 2) // Square
            {
                float sq = std::sin (phase) < 0.0f ? -1.0f : 1.0f;
                y -= sq * (height * 0.4f);
            }

            p.lineTo (x, y);
        }

        g.strokePath (p, juce::PathStrokeType (2.0f));
    }
};

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void visibilityChanged() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;

    juce::AudioVisualiserComponent outputVisualiser { 2 };
    juce::AudioBuffer<float> tempScopeBuffer;

    juce::MidiKeyboardComponent keyboardComponent;
    juce::GroupComponent osc1Group;
    WaveformDisplayComponent waveform1Display;
    juce::ComboBox waveform1Selector;
    juce::Slider osc1MixSlider;
    juce::GroupComponent osc2Group;
    WaveformDisplayComponent waveform2Display;
    juce::ComboBox waveform2Selector;
    juce::Slider osc2MixSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1WaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2WaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1MixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2MixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
