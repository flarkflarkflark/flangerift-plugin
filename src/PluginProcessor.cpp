#include "PluginProcessor.h"
#include "PluginEditor.h"

FlangeriftAudioProcessor::FlangeriftAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

FlangeriftAudioProcessor::~FlangeriftAudioProcessor()
{
}

const juce::String FlangeriftAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FlangeriftAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FlangeriftAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FlangeriftAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FlangeriftAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FlangeriftAudioProcessor::getNumPrograms()
{
    return 1;
}

int FlangeriftAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FlangeriftAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String FlangeriftAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void FlangeriftAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void FlangeriftAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);

    currentSampleRate = sampleRate;

    // Allocate delay buffer
    delayBufferLength = static_cast<int>(MAX_DELAY_TIME * sampleRate) + 1;
    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();

    writePosition = 0;
    lfoPhase = 0.0f;
}

void FlangeriftAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FlangeriftAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FlangeriftAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Flanger effect implementation
    const int numSamples = buffer.getNumSamples();
    const float lfoIncrement = LFO_RATE / static_cast<float>(currentSampleRate);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        auto* delayData = delayBuffer.getWritePointer(channel);
        int localWritePosition = writePosition;
        float localLfoPhase = lfoPhase;

        for (int sample = 0; sample < numSamples; ++sample)
        {
            const float in = channelData[sample];

            // Calculate LFO value (sine wave 0-1)
            const float lfo = 0.5f + 0.5f * std::sin(2.0f * juce::MathConstants<float>::pi * localLfoPhase);

            // Calculate delay time in samples (modulated by LFO)
            const float delayTimeSamples = lfo * DEPTH * delayBufferLength;

            // Calculate read position with fractional part
            float readPosition = localWritePosition - delayTimeSamples;
            if (readPosition < 0.0f)
                readPosition += delayBufferLength;

            // Linear interpolation for smooth delay
            const int readIndex1 = static_cast<int>(readPosition);
            const int readIndex2 = (readIndex1 + 1) % delayBufferLength;
            const float fraction = readPosition - readIndex1;

            const float delayed = delayData[readIndex1] * (1.0f - fraction) +
                                  delayData[readIndex2] * fraction;

            // Write to delay buffer (input + feedback)
            delayData[localWritePosition] = in + delayed * FEEDBACK;

            // Mix dry and wet signals
            channelData[sample] = in * (1.0f - MIX) + delayed * MIX;

            // Increment write position
            localWritePosition = (localWritePosition + 1) % delayBufferLength;

            // Increment LFO phase
            localLfoPhase += lfoIncrement;
            if (localLfoPhase >= 1.0f)
                localLfoPhase -= 1.0f;
        }
    }

    // Update global state
    writePosition = (writePosition + numSamples) % delayBufferLength;
    lfoPhase += lfoIncrement * numSamples;
    while (lfoPhase >= 1.0f)
        lfoPhase -= 1.0f;
}

bool FlangeriftAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* FlangeriftAudioProcessor::createEditor()
{
    return new FlangeriftAudioProcessorEditor (*this);
}

void FlangeriftAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

void FlangeriftAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlangeriftAudioProcessor();
}
