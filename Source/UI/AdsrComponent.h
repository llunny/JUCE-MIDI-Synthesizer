/*
  ==============================================================================

    AdsrComponent.h
    Created: 17 Apr 2024 3:56:54pm
    Author: wllun

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * AdsrComponent class defines a custom component for an ADSR (Attack, Decay, Sustain, Release)
 * envelope controller. This component is responsible for providing a user interface to adjust
 * the ADSR parameters of a sound using sliders.
 */
class AdsrComponent : public juce::Component
{
public:
    /**
     * Constructor for the AdsrComponent class.
     * Initializes the sliders and attaches them to the corresponding parameters in the AudioProcessorValueTreeState.
     *
     * @param apvts Reference to the AudioProcessorValueTreeState associated with this component, used to link sliders to audio parameters.
     */
    AdsrComponent(juce::AudioProcessorValueTreeState& apvts);

    /**
     * Destructor for the AdsrComponent class.
     */
    ~AdsrComponent() override;

    /**
     * Overridden paint method from juce::Component.
     * Used to draw the component's background.
     *
     * @param g Graphics context used for drawing within this component.
     */
    void paint(juce::Graphics&) override;

    /**
     * Overridden resized method from juce::Component.
     * Used to layout the sliders within the component as it is resized.
     */
    void resized() override;

private:
    /**
     * Helper method to set parameters for a slider.
     * Configures common properties such as style and text box settings.
     *
     * @param slider The slider to configure.
     */
    void setSliderParams(juce::Slider& slider);

    // Sliders for controlling ADSR parameters.
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    // Attachments that link the sliders to the audio parameters in the value tree state.
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    // Macro for helping identify memory leaks during debugging.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdsrComponent)
};
