/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 17 Apr 2024 3:56:54pm
    Author: wllun

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
/**
 * Constructs the AdsrComponent and sets up slider attachments to the audio processor value tree state.
 *
 * @param apvts Reference to the AudioProcessorValueTreeState that manages parameters for the audio processor,
 *              allowing for easy linkage of GUI elements to these parameters.
 */
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts)
{
    // Each slider is attached to a corresponding parameter in the audio processor value tree state.
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE", releaseSlider);

    // Initialize slider parameters for each ADSR component.
    setSliderParams(attackSlider);
    setSliderParams(decaySlider);
    setSliderParams(sustainSlider);
    setSliderParams(releaseSlider);
}

AdsrComponent::~AdsrComponent()
{
    // Cleanup can be handled here if needed.
}

/**
 * Paints the component.
 *
 * @param g Graphics context used to draw the component. In this case, the background is filled black.
 */
void AdsrComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);  // Sets the background color of the ADSR component to black.

    ////////////////////////////////////////////////
    // Set the colour and font for the text.
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);

    // Calculate the width of the area where the sliders are located.
    int slidersAreaWidth = getWidth();
    int textPositionY = 5; // Decrease this value to move the text up
    int textHeight = 20;   // The height for the text area

    // Create a rectangle that spans the width of the sliders area and is positioned at the top.
    juce::Rectangle<int> textArea(0, textPositionY, slidersAreaWidth, textHeight);

    // Draw the text within the rectangle, centered horizontally and vertically.
    g.drawFittedText("Attack         Decay         Sustain         Release", textArea, juce::Justification::centred, 1);

}

/**
 * Sets the size and position of each slider within the component.
 */
void AdsrComponent::resized()
{
    // Defines the local bounds and padding for layout calculation.
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight();
    const auto sliderStartX = 0;
    const auto sliderStartY = 20;

    // Position each slider within the component using calculated bounds.
    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
}

/**
 * Configures the properties of a slider.
 *
 * @param slider The slider to configure, setting its style and visibility.
 */
void AdsrComponent::setSliderParams(juce::Slider& slider) {
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);  // Makes the slider visible and ready for interaction.
}