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
    WaveformDisplayComponent()
    {
        samples.resize (512, 0.0f);
    }

    void pushBuffer (const juce::AudioBuffer<float>& buffer)
    {
        if (buffer.getNumChannels() == 0)
            return;

        auto* channelData = buffer.getReadPointer (0);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            samples[(size_t) writePosition] = juce::jlimit (-1.0f, 1.0f, channelData[i] * displayGain);
            writePosition = (writePosition + 1) % (int) samples.size();
        }

        repaint();
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        g.setColour (juce::Colours::black);
        g.fillRect (bounds);

        g.setColour (juce::Colours::darkgrey);
        auto centerY = bounds.getCentreY();
        g.drawHorizontalLine (juce::roundToInt (centerY), bounds.getX(), bounds.getRight());

        g.setColour (juce::Colours::cyan);
        juce::Path p;

        for (size_t i = 0; i < samples.size(); ++i)
        {
            auto sampleIndex = ((size_t) writePosition + i) % samples.size();
            auto x = juce::jmap ((float) i, 0.0f, (float) samples.size() - 1.0f, bounds.getX(), bounds.getRight());
            auto y = centerY - (samples[sampleIndex] * bounds.getHeight() * 0.45f);

            if (i == 0)
                p.startNewSubPath (x, y);
            else
                p.lineTo (x, y);
        }

        g.strokePath (p, juce::PathStrokeType (2.0f));
    }

private:
    std::vector<float> samples;
    int writePosition = 0;
    float displayGain = 1.0f;
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

    juce::AudioVisualiserComponent outputVisualiser { 1 };
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
