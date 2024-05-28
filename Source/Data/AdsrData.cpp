/*
  ==============================================================================

    AdsrData.cpp
    Created: 17 Apr 2024 3:57:18pm
    Author:  wllun

  ==============================================================================
*/

#include "AdsrData.h"

/**
 * Updates the parameters of the ADSR envelope based on user input or internal changes.
 *
 * This function sets the ADSR (Attack, Decay, Sustain, Release) values for the envelope
 * generator. These parameters shape the envelope that controls various aspects of a
 * sound's amplitude characteristics over time.
 *
 * @param attack The time it takes for the envelope to reach its maximum level after being triggered.
 * @param decay The time it takes for the envelope to drop to the sustain level after the attack phase.
 * @param sustain The level during the main sequence of the sound's duration, until the key is released.
 * @param release The time it takes for the envelope to close or reach zero level after the key is released.
 */
void AdsrData::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    // Update the internal ADSR structure with the new values
    ADSRParameters.attack = attack;
    ADSRParameters.decay = decay;
    ADSRParameters.sustain = sustain;
    ADSRParameters.release = release;

    // Apply the updated parameters to the ADSR envelope
    setParameters(ADSRParameters);
}