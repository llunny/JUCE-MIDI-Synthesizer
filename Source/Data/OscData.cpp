/*
  ==============================================================================

    OscData.cpp
    Created: 17 Apr 2024 7:04:37pm
    Author:  wllun

  ==============================================================================
*/

#include "OscData.h"

/**
 * Prepares the oscillator data for playback by initializing all required components with the given specifications.
 *
 * @param spec The audio processing specifications including sample rate, block size, and number of channels.
 *             This specification is used to prepare the internal state of the oscillator and frequency modulation oscillator.
 */
void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    fmOsc.prepare(spec);  // Prepare the frequency modulation oscillator with the provided audio specifications.
    prepare(spec);        // Prepare this oscillator with the same specifications.
}

/**
 * Sets the waveform type of the oscillator based on a given choice.
 *
 * @param choice The type of waveform to generate:
 *               0 for Sine Wave,
 *               1 for Saw Wave,
 *               2 for Square Wave.
 */
void OscData::setWaveType(const int choice) {
    switch (choice) {
    case 0:
        // Sine wave
        initialise([](float x) { return std::sin(x); });
        break;
    case 1:
        // Saw wave
        initialise([](float x) { return x / juce::MathConstants<float>::pi; });
        break;
    case 2:
        // Square wave
        initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        break;
    default:
        jassertfalse; // Triggers a breakpoint in debug mode if an undefined wave type is selected.
        break;
    }
}

/**
 * Sets the frequency of the oscillator based on a MIDI note number.
 *
 * @param midiNoteNumber The MIDI note number, which is converted to a frequency in Hertz and used to set the oscillator's frequency.
 */
void OscData::setWaveFrequency(const int midiNoteNumber) {
    // Convert MIDI note to frequency and adjust by current frequency modulation value
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber; // Store the last MIDI note for potential future use.
}

/**
 * Processes the next audio block by applying frequency modulation and oscillator processing.
 *
 * @param block The audio block to process. This function modifies the block in-place.
 */
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
    processFmOsc(block); // Apply frequency modulation to the block.
    process(juce::dsp::ProcessContextReplacing<float>(block)); // Process the block with the current oscillator settings.
}

/**
 * Processes the frequency modulation oscillator.
 *
 * @param block The audio block to which frequency modulation will be applied.
 *              Modifies the audio block by applying FM based on the current settings.
 */
void OscData::processFmOsc(juce::dsp::AudioBlock<float>& block) {
    for (int ch = 0; ch < block.getNumChannels(); ++ch) {
        for (int s = 0; s < block.getNumSamples(); ++s) {
            // Apply frequency modulation to each sample
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }
}

/**
 * Sets the parameters for frequency modulation including depth and frequency.
 *
 * @param depth The depth of frequency modulation, affecting how much the base frequency is altered.
 * @param freq The frequency of the modulation oscillator.
 */
void OscData::setFmParams(const float depth, const float freq) {
    fmOsc.setFrequency(freq); // Set the frequency of the modulation oscillator.
    fmDepth = depth; // Set the depth of frequency modulation.
    // Recalculate the frequency of the base oscillator to incorporate current frequency modulation.
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}