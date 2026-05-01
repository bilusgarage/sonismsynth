#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), keyboardComponent (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible (outputVisualiser);

    addAndMakeVisible (keyboardComponent);

    oscGroup.setText ("");
    addAndMakeVisible (oscGroup);

    osc1TabButton.setRadioGroupId (1);
    osc1TabButton.setClickingTogglesState (true);
    osc1TabButton.setToggleState (true, juce::dontSendNotification);
    osc1TabButton.onClick = [this] { updateOscTabs(); };
    addAndMakeVisible (osc1TabButton);

    osc2TabButton.setRadioGroupId (1);
    osc2TabButton.setClickingTogglesState (true);
    osc2TabButton.setToggleState (false, juce::dontSendNotification);
    osc2TabButton.onClick = [this] { updateOscTabs(); };
    addAndMakeVisible (osc2TabButton);

    osc3TabButton.setRadioGroupId (1);
    osc3TabButton.setClickingTogglesState (true);
    osc3TabButton.setToggleState (false, juce::dontSendNotification);
    osc3TabButton.onClick = [this] { updateOscTabs(); };
    addAndMakeVisible (osc3TabButton);

    addChildComponent (waveform1Display);
    waveform1Selector.addItem ("Sine", 1);
    waveform1Selector.addItem ("Triangle", 2);
    waveform1Selector.addItem ("Square", 3);
    waveform1Selector.addItem ("Sawtooth", 4);
    waveform1Selector.addItem ("Pulse", 5);
    waveform1Selector.setSelectedId (1);
    addChildComponent (waveform1Selector);

    osc1MixSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    osc1MixSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addChildComponent (osc1MixSlider);

    addChildComponent (waveform2Display);
    waveform2Selector.addItem ("Sine", 1);
    waveform2Selector.addItem ("Triangle", 2);
    waveform2Selector.addItem ("Square", 3);
    waveform2Selector.addItem ("Sawtooth", 4);
    waveform2Selector.addItem ("Pulse", 5);
    waveform2Selector.setSelectedId (2);
    addChildComponent (waveform2Selector);

    osc2MixSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    osc2MixSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addChildComponent (osc2MixSlider);

    addChildComponent (waveform3Display);
    waveform3Selector.addItem ("Sine", 1);
    waveform3Selector.addItem ("Triangle", 2);
    waveform3Selector.addItem ("Square", 3);
    waveform3Selector.addItem ("Sawtooth", 4);
    waveform3Selector.addItem ("Pulse", 5);
    waveform3Selector.setSelectedId (3);
    addChildComponent (waveform3Selector);

    osc3MixSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    osc3MixSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addChildComponent (osc3MixSlider);

    osc1WaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (processorRef.apvts, "OSC1WAVETYPE", waveform1Selector);
    osc2WaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (processorRef.apvts, "OSC2WAVETYPE", waveform2Selector);
    osc3WaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (processorRef.apvts, "OSC3WAVETYPE", waveform3Selector);
    osc1MixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "OSC1MIX", osc1MixSlider);
    osc2MixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "OSC2MIX", osc2MixSlider);
    osc3MixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "OSC3MIX", osc3MixSlider);

    filterGroup.setText ("Filter");
    addAndMakeVisible (filterGroup);

    cutoffSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible (cutoffSlider);

    resonanceSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible (resonanceSlider);

    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "FILTERCUTOFF", cutoffSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "FILTERRES", resonanceSlider);

    ampEnvGroup.setText ("Amp Env");
    addAndMakeVisible (ampEnvGroup);

    attackSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible (attackSlider);

    decaySlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible (decaySlider);

    sustainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    sustainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible (sustainSlider);

    releaseSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible (releaseSlider);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processorRef.apvts, "RELEASE", releaseSlider);

    // // this chunk of code instantiates and opens the melatonin inspector
    // if (!inspector)
    // {
    //     inspector = std::make_unique<melatonin::Inspector> (*this);
    //     inspector->onClose = [this]() { inspector.reset(); };
    // }

    // inspector->setVisible (true);

    updateOscTabs();

    startTimerHz (30);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 400);

    addMouseListener (this, true);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::updateOscTabs()
{
    bool showOsc1 = osc1TabButton.getToggleState();
    bool showOsc2 = osc2TabButton.getToggleState();
    bool showOsc3 = osc3TabButton.getToggleState();

    waveform1Display.setVisible (showOsc1);
    waveform1Selector.setVisible (showOsc1);
    osc1MixSlider.setVisible (showOsc1);

    waveform2Display.setVisible (showOsc2);
    waveform2Selector.setVisible (showOsc2);
    osc2MixSlider.setVisible (showOsc2);

    waveform3Display.setVisible (showOsc3);
    waveform3Selector.setVisible (showOsc3);
    osc3MixSlider.setVisible (showOsc3);
}

void PluginEditor::visibilityChanged()
{
    if (isShowing())
        keyboardComponent.grabKeyboardFocus();
}

void PluginEditor::mouseDown (const juce::MouseEvent& e)
{
    if (dynamic_cast<juce::TextEditor*> (e.originalComponent) == nullptr)
        keyboardComponent.grabKeyboardFocus();
}

void PluginEditor::timerCallback()
{
    auto readScopeBuffer = [this] (AudioBufferFifo<float>& fifo, int numChannels) -> bool {
        auto numReady = fifo.getNumReady();
        if (numReady <= 0)
            return false;

        tempScopeBuffer.setSize (numChannels, numReady);
        fifo.pop (tempScopeBuffer);

        constexpr auto scopeDisplayGain = 4.0f;
        tempScopeBuffer.applyGain (scopeDisplayGain);

        for (auto channel = 0; channel < tempScopeBuffer.getNumChannels(); ++channel)
            juce::FloatVectorOperations::clip (tempScopeBuffer.getWritePointer (channel),
                tempScopeBuffer.getReadPointer (channel),
                -1.0f,
                1.0f,
                tempScopeBuffer.getNumSamples());

        return true;
    };

    if (readScopeBuffer (processorRef.scopeFifo, processorRef.getTotalNumOutputChannels()))
    {
        outputVisualiser.setSampleRate (processorRef.getSampleRate());
        outputVisualiser.pushBuffer (tempScopeBuffer);
    }

    if (readScopeBuffer (processorRef.osc1ScopeFifo, 1))
        waveform1Display.pushBuffer (tempScopeBuffer);

    if (readScopeBuffer (processorRef.osc2ScopeFifo, 1))
        waveform2Display.pushBuffer (tempScopeBuffer);

    if (readScopeBuffer (processorRef.osc3ScopeFifo, 1))
        waveform3Display.pushBuffer (tempScopeBuffer);
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

    auto bottomSection = area.removeFromBottom (120);

    outputVisualiser.setBounds (area.removeFromTop (128));

    auto partWidth = bottomSection.getWidth() / 3;

    auto oscArea = bottomSection.removeFromLeft (partWidth).reduced (10);
    oscGroup.setBounds (oscArea);

    auto tabsArea = juce::Rectangle<int> (oscArea.getX() + 15, oscArea.getY(), 180, 24);
    auto tabWidth = tabsArea.getWidth() / 3;
    osc1TabButton.setBounds (tabsArea.removeFromLeft (tabWidth));
    osc2TabButton.setBounds (tabsArea.removeFromLeft (tabWidth));
    osc3TabButton.setBounds (tabsArea);

    auto oscContent = oscArea.withTop (oscArea.getY() + 24).reduced (10);

    auto oscControls = oscContent.removeFromRight (100);

    auto selectorBounds = oscControls.removeFromTop (24);
    waveform1Selector.setBounds (selectorBounds);
    waveform2Selector.setBounds (selectorBounds);
    waveform3Selector.setBounds (selectorBounds);

    auto mixBounds = oscControls.removeFromTop (24).withTrimmedTop (4);
    osc1MixSlider.setBounds (mixBounds);
    osc2MixSlider.setBounds (mixBounds);
    osc3MixSlider.setBounds (mixBounds);

    auto displayBounds = oscContent.withTrimmedRight (10);
    waveform1Display.setBounds (displayBounds);
    waveform2Display.setBounds (displayBounds);
    waveform3Display.setBounds (displayBounds);

    auto filterArea = bottomSection.removeFromLeft (partWidth).reduced (10);
    filterGroup.setBounds (filterArea);

    auto filterContent = filterArea.withTop (filterArea.getY() + 15).reduced (10);
    cutoffSlider.setBounds (filterContent.removeFromLeft (filterContent.getWidth() / 2));
    resonanceSlider.setBounds (filterContent);

    auto envArea = bottomSection.reduced (10);
    ampEnvGroup.setBounds (envArea);

    auto envContent = envArea.withTop (envArea.getY() + 15).reduced (10);
    auto envKnobWidth = envContent.getWidth() / 4;
    attackSlider.setBounds (envContent.removeFromLeft (envKnobWidth));
    decaySlider.setBounds (envContent.removeFromLeft (envKnobWidth));
    sustainSlider.setBounds (envContent.removeFromLeft (envKnobWidth));
    releaseSlider.setBounds (envContent);
}
