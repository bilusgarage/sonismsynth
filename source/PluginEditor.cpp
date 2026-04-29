#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), keyboardComponent (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    juce::ignoreUnused (processorRef);

    outputVisualiser.setColours (juce::Colours::black, juce::Colours::cyan);
    outputVisualiser.setRepaintRate (30);
    addAndMakeVisible (outputVisualiser);

    addAndMakeVisible (keyboardComponent);

    osc1Group.setText ("OSC 1");
    addAndMakeVisible (osc1Group);
    addAndMakeVisible (waveform1Display);
    waveform1Selector.addItem ("Sine", 1);
    waveform1Selector.addItem ("Triangle", 2);
    waveform1Selector.addItem ("Square", 3);
    waveform1Selector.setSelectedId (1);
    addAndMakeVisible (waveform1Selector);

    osc1MixSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    osc1MixSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible (osc1MixSlider);

    osc2Group.setText ("OSC 2");
    addAndMakeVisible (osc2Group);
    addAndMakeVisible (waveform2Display);
    waveform2Selector.addItem ("Sine", 1);
    waveform2Selector.addItem ("Triangle", 2);
    waveform2Selector.addItem ("Square", 3);
    waveform2Selector.setSelectedId (2);
    addAndMakeVisible (waveform2Selector);

    osc2MixSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    osc2MixSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible (osc2MixSlider);

    osc1WaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (processorRef.apvts, "OSC1WAVETYPE", waveform1Selector);
    osc2WaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (processorRef.apvts, "OSC2WAVETYPE", waveform2Selector);
    osc1MixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "OSC1MIX", osc1MixSlider);
    osc2MixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "OSC2MIX", osc2MixSlider);

    // // this chunk of code instantiates and opens the melatonin inspector
    if (!inspector)
    {
        inspector = std::make_unique<melatonin::Inspector> (*this);
        inspector->onClose = [this]() { inspector.reset(); };
    }

    inspector->setVisible (true);

    startTimerHz (30);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::visibilityChanged()
{
    if (isShowing())
        keyboardComponent.grabKeyboardFocus();
}

void PluginEditor::timerCallback()
{
    auto numReady = processorRef.scopeFifo.getNumReady();
    if (numReady > 0)
    {
        tempScopeBuffer.setSize (processorRef.getTotalNumOutputChannels(), numReady);
        processorRef.scopeFifo.pop (tempScopeBuffer);
        outputVisualiser.pushBuffer (tempScopeBuffer);
    }
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    keyboardComponent.setBounds (area.removeFromBottom (100));

    auto oscArea = area.removeFromBottom (120);

    outputVisualiser.setBounds (area.reduced (10));

    auto osc1Area = oscArea.removeFromLeft (oscArea.getWidth() / 2).reduced (10);
    osc1Group.setBounds (osc1Area);

    auto osc1Content = osc1Area.withTop (osc1Area.getY() + 15).reduced (10);
    auto osc1Controls = osc1Content.removeFromRight (100);
    waveform1Selector.setBounds (osc1Controls.removeFromTop (24));
    osc1MixSlider.setBounds (osc1Controls.removeFromTop (24).withTrimmedTop (4));
    waveform1Display.setBounds (osc1Content.withTrimmedRight (10));

    auto osc2Area = oscArea.reduced (10);
    osc2Group.setBounds (osc2Area);

    auto osc2Content = osc2Area.withTop (osc2Area.getY() + 15).reduced (10);
    auto osc2Controls = osc2Content.removeFromRight (100);
    waveform2Selector.setBounds (osc2Controls.removeFromTop (24));
    osc2MixSlider.setBounds (osc2Controls.removeFromTop (24).withTrimmedTop (4));
    waveform2Display.setBounds (osc2Content.withTrimmedRight (10));
}
