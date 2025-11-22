#include "PluginProcessor.h"
#include "PluginEditor.h"

FlangeriftEditor::FlangeriftEditor(FlangeriftProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
}

FlangeriftEditor::~FlangeriftEditor() {}

void FlangeriftEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    g.setColour(juce::Colour(0xfff97316));
    g.setFont(36.0f);
    g.drawText("FLANGERIFT", getLocalBounds(), juce::Justification::centred, true);
}

void FlangeriftEditor::resized() {}
