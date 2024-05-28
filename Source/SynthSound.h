/*
  ==============================================================================

    SynthSound.h
    Created: 15 Apr 2024 4:28:33pm
    Author: wllun

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
 * SynthSound is a subclass of juce::SynthesiserSound, which is used to define the properties
 * of a sound that a synthesiser can play. In this simple example, SynthSound is configured
 * to apply to all MIDI notes and channels, making it a universal sound for any MIDI input.
 */
class SynthSound : public juce::SynthesiserSound {

public:
    /**
     * Determines whether this sound should be used for a given MIDI note.
     * In this implementation, it always returns true, meaning this sound object
     * is applicable to all MIDI notes.
     *
     * @param midiNoteNumber The MIDI note number.
     * @return Always returns true, applicable to any note.
     */
    bool appliesToNote(int midiNoteNumber) override { return true; }

    /**
     * Determines whether this sound should be used for a given MIDI channel.
     * In this implementation, it always returns true, meaning this sound object
     * is applicable to all MIDI channels.
     *
     * @param midiChannel The MIDI channel number.
     * @return Always returns true, applicable to any channel.
     */
    bool appliesToChannel(int midiChannel) override { return true; }

};