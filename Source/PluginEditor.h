/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"

//==============================================================================
/**
    SynthAudioProcessorEditor is the graphical interface for the audio processor.
    It inherits from juce::AudioProcessorEditor, which provides the basic framework
    for creating the UI components of an audio processing plugin.
*/
class SynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    /**
     * Constructor for the SynthAudioProcessorEditor.
     *
     * @param audioProcessor A reference to the SynthAudioProcessor that this editor will interact with.
     *                       This reference allows the editor to access the processor's parameters and functions.
     */
    SynthAudioProcessorEditor(SynthAudioProcessor&);

    /**
     * Destructor for the SynthAudioProcessorEditor.
     */
    ~SynthAudioProcessorEditor() override;

    //==============================================================================
    /**
     * Paints the editor window.
     *
     * This method is called when the look of the editor needs to be updated.
     *
     * @param g Graphics context used to draw the UI components on the screen.
     */
    void paint(juce::Graphics&) override;

    /**
     * Defines the layout and resizing behavior for the editor's components.
     *
     * This method is called when the editor window is resized. It should update the layout
     * of the UI components to fit the new size of the window.
     */
    void resized() override;

private:
    // Member variables
    SynthAudioProcessor& audioProcessor;  // Reference to the audio processor associated with this editor.
    OscComponent osc;                     // Oscillator component part of the UI, handles oscillator settings.
    AdsrComponent adsr;                   // ADSR envelope component part of the UI, handles envelope settings.

    // Macro to help with memory leaks detection during debugging.
    // It should be present in all classes that allocate memory dynamically.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessorEditor)
};