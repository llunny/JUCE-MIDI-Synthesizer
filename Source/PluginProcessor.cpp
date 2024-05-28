/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

// Include the headers for the processor and editor
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Constructor for the audio processor class
SynthAudioProcessor::SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        // Setting up the audio bus properties with stereo configuration
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    // Add a new sound and voice to the synthesizer
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

// Destructor for the audio processor class
SynthAudioProcessor::~SynthAudioProcessor()
{
}

//==============================================================================
// Returns the plugin's name
const juce::String SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

// Determines if the plugin can accept MIDI input
bool SynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

// Determines if the plugin can produce MIDI output
bool SynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

// Checks if this is a MIDI effect plugin
bool SynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

// Returns the length of the audio tail in seconds
double SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

// Returns the number of programs available
int SynthAudioProcessor::getNumPrograms()
{
    // Some hosts do not handle 0 programs well, so return at least 1
    return 1;
}

// Returns the current program number
int SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

// Sets the current program to the given index
void SynthAudioProcessor::setCurrentProgram(int index)
{
}

// Returns the name of the program at the given index
const juce::String SynthAudioProcessor::getProgramName(int index)
{
    return {};
}

// Changes the name of the program at the given index to newName
void SynthAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
// Prepare the plugin for playing
void SynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Sets the current playback sample rate for the synthesizer
    synth.setCurrentPlaybackSampleRate(sampleRate);

    // Prepare each voice for playing with the current configuration
    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

// Releases any resources that are no longer needed
void SynthAudioProcessor::releaseResources()
{
    // This is an opportunity to free up any spare memory when playback stops
}

#ifndef JucePlugin_PreferredChannelConfigurations
// Checks if the bus layout is supported
bool SynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // Check if the layout is stereo or mono, as we support only these configurations
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Ensure the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

// The process block where audio processing happens
void SynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't have input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Handle each voice's processing
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {

            // Update oscillator and ADSR settings from parameters
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");

            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& fmDepth = *apvts.getRawParameterValue("OSC1FMDEPTH");
            auto& fmFreq = *apvts.getRawParameterValue("OSC1FMFREQ");

            voice->getOscillator().setWaveType(oscWaveChoice);
            voice->getOscillator().setFmParams(fmDepth, fmFreq);

            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
        }
    }

    // Render the current block of audio
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
// Returns whether this plugin has a custom editor
bool SynthAudioProcessor::hasEditor() const
{
    return true; // Modify as needed if no editor is provided
}

// Creates the editor interface for this plugin
juce::AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
    return new SynthAudioProcessorEditor(*this);
}

//==============================================================================
// Saves the current state of the plugin parameters
void SynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Implementation would be here to save plugin state
}

// Restores the plugin state from the given memory block
void SynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Implementation would be here to load plugin state
}

//==============================================================================
// Factory method to create new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}

// Defines the layout of parameters for the plugin
juce::AudioProcessorValueTreeState::ParameterLayout SynthAudioProcessor::createParameters() {

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Define parameters for oscillator wave type, FM frequency and depth, and ADSR settings
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Sine", "Saw", "Square" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FMFREQ", "OSC 1 FM Frequency",
        juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FMDEPTH", "OSC 1 FM Depth",
        juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.1f, 0.3f }, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> { 0.1f, 3.0f, }, 0.4f));

    return { params.begin(), params.end() };
}