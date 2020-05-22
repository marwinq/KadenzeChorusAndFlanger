/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define MAX_DELAY_TIME 2 //In seconds

//==============================================================================
/**
*/
class KadenzeChorusAndFlangerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    KadenzeChorusAndFlangerAudioProcessor();
    ~KadenzeChorusAndFlangerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    float lin_interp(float inSampleX, float inSampleY, float inFloatPhase);

private:
    
    float mLFOPhase;
    
    AudioParameterFloat* mDryWetParameter;
    //How wide it sweeps
    AudioParameterFloat* mDepthParameter;
    //How quickly it sweeps
    AudioParameterFloat* mRateParameter;
    //Difference in Readhead between Left and Right channels
    AudioParameterFloat* mPhaseOffsetParameter;
    AudioParameterFloat* mFeedbackParameter;
    
    AudioParameterInt* mTypeParameter;
    
    //float mDryWet;
    float mFeedbackLeft;
    float mFeedbackRight;
    
    
    int mCircularBufferWriteHead;
    int mCircularBufferLength;
    
    
    float* mCircularBufferLeft;
    float* mCircularBufferRight;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeChorusAndFlangerAudioProcessor)
};
