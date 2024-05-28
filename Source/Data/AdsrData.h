/*
  ==============================================================================

    AdsrData.h
    Created: 17 Apr 2024 3:57:18pm
    Author:  wllun

  ==============================================================================
*/

#pragma once
/*
  ==============================================================================

    AdsrData.h
    Created: 17 Apr 2024 3:57:18pm
    Author:  wllun

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
 * The AdsrData class manages the ADSR envelope parameters for an audio application.
 * It extends the functionality of the juce::ADSR class by allowing detailed configuration
 * of its parameters, which are essential for controlling the dynamics of a sound.
 *
 * ADSR stands for Attack, Decay, Sustain, and Release:
 * - Attack: Time taken for the initial run-up of level from nil to peak, immediately upon triggering.
 * - Decay: Time taken for the subsequent run down from the attack level to the designated sustain level.
 * - Sustain: Level during the main sequence of the sound's duration, until the release is triggered.
 * - Release: Time taken for the level to decay from the sustain level to zero after the release is triggered.
 */
class AdsrData : public juce::ADSR {

public:
    /**
     * Updates the ADSR parameters of the envelope.
     *
     * This method allows for precise control over the shape of the sound's envelope by adjusting
     * its four key parameters: attack, decay, sustain, and release.
     *
     * @param attack   Duration (in seconds) for the envelope to reach its peak at the start of a note.
     * @param decay    Duration (in seconds) for the envelope to fall from the peak to the sustain level.
     * @param sustain  Sustain level (as a fraction of the peak) that the envelope holds until release is initiated.
     * @param release  Duration (in seconds) for the envelope to diminish from the sustain level to zero after release.
     */
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

private:
    // Stores the current parameters of the ADSR envelope.
    juce::ADSR::Parameters ADSRParameters;

};