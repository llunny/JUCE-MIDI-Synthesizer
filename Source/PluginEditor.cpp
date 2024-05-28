/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
/**
 * Constructor for the SynthAudioProcessorEditor.
 * Initializes the editor with a reference to the SynthAudioProcessor, sets up the UI components,
 * and configures their initial settings.
 *
 * @param p Reference to SynthAudioProcessor to allow the editor to interact with the processor.
 */
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    osc(audioProcessor.apvts, "OSC1WAVETYPE", "OSC1FMFREQ", "OSC1FMDEPTH"),
    adsr(audioProcessor.apvts)
{
    // Set the size of the plugin window. This should match the expected UI dimensions.
    setSize(600, 500);

    // Adds the ADSR envelope controls to the visible interface and makes them interactable.
    addAndMakeVisible(adsr);

    // Adds the oscillator controls to the visible interface and makes them interactable.
    addAndMakeVisible(osc);
}

/**
 * Destructor for the SynthAudioProcessorEditor.
 */
SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
    // Cleanup can be handled here if needed, such as disconnecting listeners.
}

//==============================================================================
/**
 * Paints the graphical components of the editor.
 * This method is called when the editor's view needs to be redrawn. It is responsible for
 * all graphical output in the user interface.
 *
 * @param g Graphics context that handles painting operations within the editor.
 */
void SynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Sets the background to black. Since the component is opaque, we need to paint every pixel.
    g.fillAll(juce::Colours::navy);

}

/**
 * Sets the size and position of child components within the editor.
 * This method is called whenever the editor window is resized. It's responsible for
 * adjusting the layout of the interface elements according to the new size.
 */
void SynthAudioProcessorEditor::resized()
{
    // Layout for the oscillator component, positioned at the top-left of the window.
    osc.setBounds(10, 10, 280, 480);

    // Layout for the ADSR component, taking up the right half of the window.
    adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
}