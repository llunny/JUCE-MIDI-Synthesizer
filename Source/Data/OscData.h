/*
  ==============================================================================

    OscData.h
    Created: 17 Apr 2024 7:04:37pm
    Author: wllun

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
 * OscData class extends juce::dsp::Oscillator to provide additional functionalities
 * for audio synthesis, including waveform type selection, frequency modulation, and
 * integration with MIDI note frequencies.
 */
class OscData : public juce::dsp::Oscillator<float> {

public:
    /**
     * Prepares the oscillator for playback. This must be called before using the oscillator
     * in an audio processing context.
     *
     * @param spec The specification detailing the sample rate, maximum block size, and number of output channels.
     */
    void prepareToPlay(juce::dsp::ProcessSpec& spec);

    /**
     * Processes the next block of audio samples, applying frequency modulation and waveform generation.
     *
     * @param block The audio block that will be processed and modified by the oscillator.
     */
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);

    /**
     * Sets the frequency of the oscillator based on a MIDI note number.
     *
     * @param midiNoteNumber The MIDI note number which is converted to a frequency in Hz.
     */
    void setWaveFrequency(const int midiNoteNumber);

    /**
     * Sets the waveform type of the oscillator based on a given choice.
     *
     * @param choice An integer representing the waveform type (e.g., 0 for sine wave, 1 for saw wave, 2 for square wave).
     */
    void setWaveType(const int choice);

    /**
     * Sets the parameters for frequency modulation, influencing the timbre and characteristics of the produced sound.
     *
     * @param depth The depth of the frequency modulation.
     * @param freq The frequency at which the modulation oscillator operates.
     */
    void setFmParams(const float depth, const float freq);

private:
    /**
     * Processes frequency modulation for an audio block, affecting the pitch based on the modulation depth and frequency.
     *
     * @param block The audio block that will receive the frequency modulation effects.
     */
    void processFmOsc(juce::dsp::AudioBlock<float>& block);

    juce::dsp::Oscillator<float> fmOsc{ [](float x) {return std::sin(x); } }; // The oscillator used for frequency modulation.

    float fmMod{ 0.0f }; // Current frequency modulation value.
    float fmDepth{ 0.0f }; // Depth of frequency modulation.
    int lastMidiNote{ 0 }; // Last MIDI note received, used to calculate frequency changes.

};