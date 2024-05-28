/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthVoice.h" // Include the definition of our SynthVoice, which will handle the actual sound generation.
#include "SynthSound.h" // Include the definition of our SynthSound, which will be used to determine if a given MIDI note should trigger a voice.

//==============================================================================
/**
 * The SynthAudioProcessor class defines the core functionality of our plugin.
 * It inherits from juce::AudioProcessor, which provides the basic framework for a JUCE plugin.
 */
class SynthAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    // Constructor and destructor
    SynthAudioProcessor(); // Constructor declaration.
    ~SynthAudioProcessor() override; // Destructor declaration.

    //==============================================================================
    // Core processing functions
    void prepareToPlay(double sampleRate, int samplesPerBlock) override; // Prepares the plugin for playback.
    void releaseResources() override; // Used to release any resources that the plugin might have allocated.

#ifndef JucePlugin_PreferredChannelConfigurations
    // Checks if the proposed layout of input/output buses is supported.
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    // The processing block where audio processing happens.
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    // GUI related functions
    // Creates the plugin's GUI editor if it has one.
    juce::AudioProcessorEditor* createEditor() override;
    // Reports whether this plugin has its own custom editor.
    bool hasEditor() const override;

    //==============================================================================
    // Information functions
    // Returns the name of the plugin.
    const juce::String getName() const override;

    // MIDI functionality support queries.
    bool acceptsMidi() const override; // Can this plugin accept MIDI messages?
    bool producesMidi() const override; // Does this plugin produce MIDI messages?
    bool isMidiEffect() const override; // Is this plugin a MIDI effect?
    // Returns the tail length of the plugin or 0 if not applicable.
    double getTailLengthSeconds() const override;

    //==============================================================================
    // Program (preset) related functions
    // The total number of programs (presets) available in the plugin.
    int getNumPrograms() override;
    // Returns the index of the currently active program.
    int getCurrentProgram() override;
    // Sets the current program to the given index.
    void setCurrentProgram(int index) override;
    // Returns the name of the program at the given index.
    const juce::String getProgramName(int index) override;
    // Changes the name of the program at the given index to the new name.
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    // State information functions
    // Saves the current state of the plugin parameters to a given memory block.
    void getStateInformation(juce::MemoryBlock& destData) override;
    // Restores the state of the plugin parameters from a given memory block.
    void setStateInformation(const void* data, int sizeInBytes) override;

    // The AudioProcessorValueTreeState object, which manages the plugin's parameters and state.
    juce::AudioProcessorValueTreeState apvts;

private:
    // The synthesiser instance that will manage voices and sounds.
    juce::Synthesiser synth;

    // Function to create and return the parameter layout for the plugin's parameters.
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    //==============================================================================
    // A macro to help detect memory leaks and to ensure that no copies of the processor are made.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessor)
};
