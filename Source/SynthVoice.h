/*
  ==============================================================================

    SynthVoice.h
    Created: 15 Apr 2024 4:28:10pm
    Author: wllun

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/AdsrData.h"
#include "Data/OscData.h"

/**
 * SynthVoice class extends juce::SynthesiserVoice, providing a concrete implementation of a voice
 * that can play a sound within a synthesiser. It manages note playing, ADSR envelope, and oscillator data.
 */
class SynthVoice : public juce::SynthesiserVoice {

public:
    /**
     * Determines if this voice can play the given sound object.
     * @param sound Pointer to a juce::SynthesiserSound object.
     * @return True if the sound can be played, typically checked by dynamic casting to the expected sound type.
     */
    bool canPlaySound(juce::SynthesiserSound* sound) override;

    /**
     * Starts a note with specific parameters.
     * @param midiNoteNumber The MIDI note number that is triggered.
     * @param velocity The velocity at which the note is played.
     * @param sound The sound object this note will use.
     * @param currentPitchWheelPosition The current position of the pitch wheel.
     */
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;

    /**
     * Stops the note currently being played by this voice.
     * @param velocity The velocity at which the note is released.
     * @param allowTailOff Boolean indicating whether to allow the ADSR tail off or stop immediately.
     */
    void stopNote(float velocity, bool allowTailOff) override;

    /**
     * Responds to MIDI controller movements.
     * @param controllerNumber The number of the MIDI controller.
     * @param newControllerValue The new value for the MIDI controller.
     */
    void controllerMoved(int controllerNumber, int newControllerValue) override;

    /**
     * Responds to pitch wheel movements.
     * @param newPitchWheelValue The new position of the pitch wheel.
     */
    void pitchWheelMoved(int newPitchWheelValue) override;

    /**
     * Prepares the voice for playback by initializing processing specifications.
     * @param sampleRate The audio sample rate.
     * @param samplesPerBlock The number of samples per block to be processed.
     * @param outputChannels The number of output audio channels.
     */
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);

    /**
     * Renders audio output for the current block.
     * @param outputBuffer The buffer where audio data will be written.
     * @param startSample The starting sample number within the buffer to begin writing.
     * @param numSamples The number of samples to process in this block.
     */
    void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;

    /**
     * Updates the ADSR envelope parameters for this voice.
     * @param attack The attack time of the ADSR envelope.
     * @param decay The decay time of the ADSR envelope.
     * @param sustain The sustain level of the ADSR envelope.
     * @param release The release time of the ADSR envelope.
     */
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

    /**
     * Provides access to this voice's oscillator data.
     * @return Reference to the OscData object representing the oscillator.
     */
    OscData& getOscillator() { return osc; }

private:
    AdsrData adsr;                           ///< Manages ADSR envelope for this voice.
    juce::AudioBuffer<float> synthBuffer;    ///< Buffer used for synthesising audio within this voice.
    OscData osc;                             ///< Oscillator data handling waveforms and pitch modulation.
    juce::dsp::Gain<float> gain;             ///< Gain processor for adjusting output levels.
    bool isPrepared{ false };                ///< Flag to check if the voice has been prepared before playing.

};