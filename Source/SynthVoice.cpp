/*
  ==============================================================================

    SynthVoice.cpp
    Defines the behavior of each voice in the synthesizer, including how it
    starts and stops notes, applies modulation, and processes the generated audio.

  ==============================================================================
*/

#include "SynthVoice.h"

// Checks if this voice can play a given sound.
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    // The dynamic_cast returns a non-null pointer if the cast is successful,
    // indicating the voice can play the SynthesiserSound.
    return dynamic_cast<juce::SynthesiserSound*> (sound) != nullptr;
}

// Called when a MIDI note-on event is received.
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    // Sets the oscillator frequency based on the MIDI note number.
    osc.setWaveFrequency(midiNoteNumber);
    // Triggers the ADSR envelope's note-on event.
    adsr.noteOn();
}

// Called when a MIDI note-off event is received.
void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    // Triggers the ADSR envelope's note-off event.
    adsr.noteOff();
    // If tail-off is not allowed or the envelope has finished its release stage, clear the current note.
    if (!allowTailOff || !adsr.isActive()) {
        clearCurrentNote();
    }
}

// Called when a MIDI controller event is received.
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
    // Here you would handle MIDI controller changes (e.g., mod wheel).
}

// Called when the MIDI pitch wheel is moved.
void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
    // Here you would handle pitch wheel movements.
}

// Prepares the voice for playback.
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    // Sets the ADSR sample rate, which is essential for the envelope timing.
    adsr.setSampleRate(sampleRate);

    // Initializes the DSP processing spec with the current playback context.
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    // Prepares the oscillator and gain DSP objects with the spec.
    osc.prepareToPlay(spec);
    gain.prepare(spec);

    // Sets an initial gain value.
    gain.setGainLinear(0.3f);

    // Ensures that the voice is ready to process audio.
    isPrepared = true;
}

// Updates the parameters of the ADSR envelope.
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    // Passes the updated envelope parameters to the ADSR.
    adsr.updateADSR(attack, decay, sustain, release);
}

// Renders the next block of audio samples.
void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    // Ensures the voice is prepared before generating audio.
    jassert(isPrepared);

    // If the voice is not active, there's no need to render anything.
    if (!isVoiceActive())
        return;

    // Resizes the temporary buffer and clears any previous content.
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // Wraps the buffer in an AudioBlock for processing by the DSP objects.
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };

    // Generates the oscillator output for the current block.
    osc.getNextAudioBlock(audioBlock);

    // Processes the generated audio through the gain stage.
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    // Applies the ADSR envelope to the generated audio.
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    // Adds the processed audio from the temporary buffer to the output buffer.
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        // If the ADSR envelope has finished its release stage, clear the current note.
        if (!adsr.isActive()) {
            clearCurrentNote();
        }
    }
}