/*
  ==============================================================================

    OscComponent.h
    Created: 17 Apr 2024 7:30:42pm
    Author:  wllun

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * OscComponent is a user interface component that provides controls for adjusting
 * oscillator settings, specifically wave type selection and frequency modulation parameters
 * within an audio processor's framework.
 */
class OscComponent  : public juce::Component
{
public:
    /**
     * Constructs the OscComponent.
     * Initializes the component with selectors for oscillator waveform and sliders for frequency modulation.
     *
     * @param apvts Reference to the AudioProcessorValueTreeState which provides access to parameters stored in a value tree.
     * @param waveSelectorId The parameter ID for the wave selector in the value tree.
     * @param fmFreqId The parameter ID for frequency modulation frequency in the value tree.
     * @param fmDepthId The parameter ID for frequency modulation depth in the value tree.
     */
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId);

    /**
     * Destructor for OscComponent.
     */
    ~OscComponent() override;

    /**
    * Paints the component's background and outlines.
    *
    * @param g Graphics context used to draw the component's appearance.
    */
    void paint (juce::Graphics&) override;

    /**
     * Resizes and repositions child components based on the component's current size.
     */
    void resized() override;

private:
    /**
     * Helper method to configure a slider and its associated label for the UI.
     *
     * @param slider The slider to be configured.
     * @param label The label associated with the slider.
     * @param apvts Reference to the AudioProcessorValueTreeState for parameter linkage.
     * @param paramId The ID of the parameter that the slider will control.
     * @param attachment The unique pointer to manage the attachment of the slider to the parameter.
     */

     // UI components for oscillator wave selection.
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

    // Sliders and labels for the frequency modulation frequency and depth parameters.
    juce::Slider fmFreqSlider; // Label for the FM frequency slider.
    juce::Slider fmDepthSlider; // Label for the FM depth slider.

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<Attachment> fmFreqAttachment;
    std::unique_ptr<Attachment> fmDepthAttachment;

    // Attachments that link sliders to parameters in the value tree.
    juce::Label fmFreqLabel{"FM Frequency", "FM Frequency"};
    juce::Label fmDepthLabel{"FM Depth", "FM Depth"};

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts,
        juce::String paramId, std::unique_ptr<Attachment>& attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
