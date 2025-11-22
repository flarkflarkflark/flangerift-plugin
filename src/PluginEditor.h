#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class FlangeriftEditor : public juce::AudioProcessorEditor
{
public:
    FlangeriftEditor(FlangeriftProcessor&);
    ~FlangeriftEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    FlangeriftProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlangeriftEditor)
};
