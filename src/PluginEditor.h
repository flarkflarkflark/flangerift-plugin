#pragma once

#include "PluginProcessor.h"

class FlangeriftAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit FlangeriftAudioProcessorEditor (FlangeriftAudioProcessor&);
    ~FlangeriftAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    FlangeriftAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangeriftAudioProcessorEditor)
};
