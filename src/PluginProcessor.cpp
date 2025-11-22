#include "PluginProcessor.h"
#include "PluginEditor.h"

FlangeriftProcessor::FlangeriftProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    addParameter(flangerRate = new juce::AudioParameterFloat("flangerRate", "Flanger Rate", 0.1f, 10.0f, 0.5f));
    addParameter(flangerDepth = new juce::AudioParameterFloat("flangerDepth", "Flanger Depth", 0.0f, 1.0f, 0.5f));
    addParameter(flangerFeedback = new juce::AudioParameterFloat("flangerFeedback", "Flanger Feedback", 0.0f, 0.9f, 0.3f));
    addParameter(flangerMix = new juce::AudioParameterFloat("flangerMix", "Flanger Mix", 0.0f, 1.0f, 0.5f));
    addParameter(filterCutoff = new juce::AudioParameterFloat("filterCutoff", "Filter Cutoff", 20.0f, 20000.0f, 1000.0f));
    addParameter(filterResonance = new juce::AudioParameterFloat("filterResonance", "Filter Resonance", 0.0f, 1.0f, 0.5f));
    addParameter(filterMix = new juce::AudioParameterFloat("filterMix", "Filter Mix", 0.0f, 1.0f, 0.5f));
}

FlangeriftProcessor::~FlangeriftProcessor() {}

const juce::String FlangeriftProcessor::getName() const { return JucePlugin_Name; }
bool FlangeriftProcessor::acceptsMidi() const { return false; }
bool FlangeriftProcessor::producesMidi() const { return false; }
bool FlangeriftProcessor::isMidiEffect() const { return false; }
double FlangeriftProcessor::getTailLengthSeconds() const { return 0.0; }
int FlangeriftProcessor::getNumPrograms() { return 1; }
int FlangeriftProcessor::getCurrentProgram() { return 0; }
void FlangeriftProcessor::setCurrentProgram(int) {}
const juce::String FlangeriftProcessor::getProgramName(int) { return {}; }
void FlangeriftProcessor::changeProgramName(int, const juce::String&) {}

void FlangeriftProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    int maxDelaySamples = static_cast<int>(sampleRate * 0.02);
    delayBuffer.setSize(2, maxDelaySamples);
    delayBuffer.clear();
    writePosition = 0;
    lfoPhase = 0.0f;
}

void FlangeriftProcessor::releaseResources()
{
    delayBuffer.setSize(0, 0);
}

void FlangeriftProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto numSamples = buffer.getNumSamples();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float lfo = std::sin(lfoPhase * 2.0f * juce::MathConstants<float>::pi);
        lfoPhase += *flangerRate / static_cast<float>(currentSampleRate);
        if (lfoPhase >= 1.0f) lfoPhase -= 1.0f;

        float minDelay = static_cast<float>(currentSampleRate) * 0.002f;
        float maxDelay = static_cast<float>(currentSampleRate) * 0.01f;
        float delayTime = minDelay + (maxDelay - minDelay) * (0.5f + 0.5f * lfo * *flangerDepth);

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            auto* delayData = delayBuffer.getWritePointer(channel);
            
            float input = channelData[sample];
            
            float readPos = writePosition - delayTime;
            if (readPos < 0) readPos += delayBuffer.getNumSamples();
            
            int readPos1 = static_cast<int>(readPos);
            int readPos2 = (readPos1 + 1) % delayBuffer.getNumSamples();
            float frac = readPos - readPos1;
            
            float delaySample = delayData[readPos1] * (1.0f - frac) + delayData[readPos2] * frac;
            delayData[writePosition] = input + delaySample * *flangerFeedback;
            
            channelData[sample] = input * (1.0f - *flangerMix) + delaySample * *flangerMix;
        }
        
        writePosition = (writePosition + 1) % delayBuffer.getNumSamples();
    }
}

bool FlangeriftProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* FlangeriftProcessor::createEditor()
{
    return new FlangeriftEditor(*this);
}

void FlangeriftProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ignoreUnused(destData);
}

void FlangeriftProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlangeriftProcessor();
}
