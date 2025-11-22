#include "PluginProcessor.h"
#include "PluginEditor.h"

FlangeriftAudioProcessorEditor::FlangeriftAudioProcessorEditor (FlangeriftAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    setSize (400, 300);
}

FlangeriftAudioProcessorEditor::~FlangeriftAudioProcessorEditor()
{
}

void FlangeriftAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colour (0xfff97316));
    g.setFont (30.0f);
    g.drawFittedText ("FLANGERIFT", getLocalBounds(), juce::Justification::centred, 1);
}

void FlangeriftAudioProcessorEditor::resized()
{
}
