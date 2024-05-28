/*
  ==============================================================================

    OscComponent.cpp
    Created: 17 Apr 2024 7:30:42pm
    Author: wllun

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
/**
 * Constructs the OscComponent and initializes UI components linked to the oscillator settings.
 *
 * @param apvts Reference to the AudioProcessorValueTreeState that manages parameters for the audio processor,
 *              allowing for easy linkage of GUI elements to these parameters.
 * @param waveSelectorId ID for the wave type parameter in the value tree.
 * @param fmFreqId ID for the FM frequency parameter in the value tree.
 * @param fmDepthId ID for the FM depth parameter in the value tree.
 */
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId)
{
    juce::StringArray choices{ "Sine", "Saw", "Square" };  // Define waveform types.
    oscWaveSelector.addItemList(choices, 1);              // Populate the combo box with choices.
    addAndMakeVisible(oscWaveSelector);                   // Make the wave selector visible and interactable.

    // Attach the combo box to its corresponding parameter in the value tree.
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);

    // Initialize sliders and labels for frequency modulation settings.
    setSliderWithLabel(fmFreqSlider, fmFreqLabel, apvts, fmFreqId, fmFreqAttachment);
    setSliderWithLabel(fmDepthSlider, fmDepthLabel, apvts, fmDepthId, fmDepthAttachment);
}

OscComponent::~OscComponent()
{
    // Destructor for cleanup, if needed.
}

void OscComponent::paint(juce::Graphics& g)
{
    // Paints the background and draws a rectangle around the component.
    g.fillAll(juce::Colours::black); // Fill background with black.
    g.setColour(juce::Colours::white); // Set drawing color to white.
    g.drawRect(getLocalBounds(), 1); // Draw a white border around the component.
}

void OscComponent::resized()
{
    // Layout child components based on the current size of this component.
    const int sliderPosY = 80;
    const int sliderWidth = 100;
    const int sliderHeight = 90;
    const int labelYOffset = 20;
    const int labelHeight = 20;

    oscWaveSelector.setBounds(0, 0, 90, 20);  // Set the position and size of the wave selector.
    fmFreqSlider.setBounds(0, sliderPosY, sliderWidth, sliderHeight);  // Set position and size for FM frequency slider.
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - labelYOffset, fmFreqSlider.getWidth(), labelHeight);  // Position the label above the slider.

    fmDepthSlider.setBounds(fmFreqSlider.getRight(), sliderPosY, sliderWidth, sliderHeight);  // Set position and size for FM depth slider.
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - labelYOffset, fmDepthSlider.getWidth(), labelHeight);  // Position the label above the slider.
}

/**
 * Initializes a slider and its accompanying label, attaching it to a parameter in the value tree.
 *
 * @param slider The slider component to configure and display.
 * @param label The label to display associated text for the slider.
 * @param apvts Reference to the AudioProcessorValueTreeState to link the slider to a parameter.
 * @param paramId The ID of the parameter to which the slider will be linked.
 * @param attachment The unique pointer to manage the attachment, ensuring the slider updates with the parameter.
 */
void OscComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts,
    juce::String paramId, std::unique_ptr<Attachment>& attachment) {

    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);  // Set slider style to rotary.
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);  // Set text box style below the slider.
    addAndMakeVisible(slider);  // Make the slider visible and interactable.

    // Create a new attachment for the slider, linking it to the specified parameter.
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramId, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);  // Set label text color to white.
    label.setFont(15.0f);  // Set font size for the label.
    label.setJustificationType(juce::Justification::centred);  // Center the text within the label.
    addAndMakeVisible(label);  // Make the label visible and interactable.
}