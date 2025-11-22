#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class FlangeriftAudioProcessor  : public juce::AudioProcessor
{
public:
    FlangeriftAudioProcessor();
    ~FlangeriftAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    // Flanger parameters
    static constexpr float MAX_DELAY_TIME = 0.010f; // 10ms max delay
    static constexpr float LFO_RATE = 0.25f;         // LFO frequency in Hz
    static constexpr float DEPTH = 0.7f;             // Modulation depth (0-1)
    static constexpr float FEEDBACK = 0.5f;          // Feedback amount
    static constexpr float MIX = 0.5f;               // Dry/wet mix

    // Flanger state
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferLength = 0;
    int writePosition = 0;
    float lfoPhase = 0.0f;
    double currentSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangeriftAudioProcessor)
};
