#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), keyboardComponent (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible (keyboardComponent);

    osc1Group.setText ("OSC 1");
    addAndMakeVisible (osc1Group);
    addAndMakeVisible (waveform1Display);
    waveform1Selector.addItem ("Sine", 1);
    waveform1Selector.addItem ("Triangle", 2);
    waveform1Selector.addItem ("Square", 3);
    waveform1Selector.setSelectedId (1);
    addAndMakeVisible (waveform1Selector);
    osc2Group.setText ("OSC 2");
    addAndMakeVisible (osc2Group);
    addAndMakeVisible (waveform2Display);
    waveform2Selector.addItem ("Sine", 1);
    waveform2Selector.addItem ("Triangle", 2);
    waveform2Selector.addItem ("Square", 3);
    waveform2Selector.setSelectedId (2);
    addAndMakeVisible (waveform2Selector);

    // // this chunk of code instantiates and opens the melatonin inspector
    if (!inspector)
    {
        inspector = std::make_unique<melatonin::Inspector> (*this);
        inspector->onClose = [this]() { inspector.reset(); };
    }

    inspector->setVisible (true);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::visibilityChanged()
{
    if (isShowing())
        keyboardComponent.grabKeyboardFocus();
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

    auto osc1Area = oscArea.removeFromLeft (oscArea.getWidth() / 2).reduced (10);
    osc1Group.setBounds (osc1Area);

    auto osc1Content = osc1Area.withTop (osc1Area.getY() + 15).reduced (10);
    waveform1Selector.setBounds (osc1Content.removeFromRight (100).withSizeKeepingCentre (100, 24));
    waveform1Display.setBounds (osc1Content.withTrimmedRight (10));

    auto osc2Area = oscArea.reduced (10);
    osc2Group.setBounds (osc2Area);

    auto osc2Content = osc2Area.withTop (osc2Area.getY() + 15).reduced (10);
    waveform2Selector.setBounds (osc2Content.removeFromRight (100).withSizeKeepingCentre (100, 24));
    waveform2Display.setBounds (osc2Content.withTrimmedRight (10));
}
