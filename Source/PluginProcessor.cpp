/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeChorusAndFlangerAudioProcessor::KadenzeChorusAndFlangerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(mDryWetParameter = new AudioParameterFloat("drywet","Dry Wet",0.0, 1.0, 0.5));
    
    addParameter(mDepthParameter = new AudioParameterFloat("depth","Depth",0.0, 1.0, 0.5));
    
    addParameter(mRateParameter = new AudioParameterFloat("rate","Rate",0.1f, 20.f, 10.f));
    
    addParameter(mPhaseOffsetParameter = new AudioParameterFloat("phaseoffset","Phase Offset", 0.0f, 1.f, 0.f));
    
    addParameter(mFeedbackParameter = new AudioParameterFloat("feedback","Feedback",0,0.98,0.5));
    
    addParameter(mTypeParameter = new AudioParameterInt("type","Type",0,1,0));
    
    
    mLFOPhase = 0;
    
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = 0;
    
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
   // mDryWet = 0.5;  //Not used when using parameter
}

KadenzeChorusAndFlangerAudioProcessor::~KadenzeChorusAndFlangerAudioProcessor()
{
    if (mCircularBufferLeft != nullptr)
    {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if (mCircularBufferRight != nullptr)
    {
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
}

//==============================================================================
const String KadenzeChorusAndFlangerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KadenzeChorusAndFlangerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeChorusAndFlangerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeChorusAndFlangerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KadenzeChorusAndFlangerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KadenzeChorusAndFlangerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KadenzeChorusAndFlangerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KadenzeChorusAndFlangerAudioProcessor::setCurrentProgram (int index)
{
}

const String KadenzeChorusAndFlangerAudioProcessor::getProgramName (int index)
{
    return {};
}

void KadenzeChorusAndFlangerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void KadenzeChorusAndFlangerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mLFOPhase = 0;
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    
    if (mCircularBufferLeft != nullptr ) {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if (mCircularBufferRight != nullptr ) {
        delete [] mCircularBufferLeft;
        mCircularBufferRight = nullptr;
    }
    
    
    
    if (mCircularBufferLeft == nullptr)
    {
        mCircularBufferLeft = new float[mCircularBufferLength];
    }
    zeromem(mCircularBufferLeft, mCircularBufferLength * sizeof(float));
    
    
    if (mCircularBufferRight == nullptr)
    {
        mCircularBufferRight = new float[mCircularBufferLength];
    }
    zeromem(mCircularBufferRight, mCircularBufferLength * sizeof(float));
    
    mCircularBufferWriteHead = 0;

    
}

void KadenzeChorusAndFlangerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KadenzeChorusAndFlangerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KadenzeChorusAndFlangerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    for (int i =0; i< buffer.getNumSamples(); i++){
        
        float lfoOutLeft = sin(2*M_PI * mLFOPhase);
        
        float lfoPhaseRight = mLFOPhase + *mPhaseOffsetParameter;
        if (lfoPhaseRight > 1){
            
            lfoPhaseRight -= 1;
        }
        float lfoOutRight = sin (2*M_PI * lfoPhaseRight);
        
        lfoOutLeft *= *mDepthParameter;
        
        
        float lfoOutMappedLeft = 0;
        float lfoOutMappedRight = 0;

        //Chorus
        if (*mTypeParameter == 0){
            lfoOutMappedLeft = jmap(lfoOutLeft,-1.f,1.f,0.005f,0.03f);
            
            lfoOutMappedRight = jmap(lfoOutRight,-1.f,1.f,0.005f,0.03f);
            
        }
        //Flanger
        else{
            lfoOutMappedLeft = jmap(lfoOutLeft,-1.f,1.f,0.001f,0.005f);
            lfoOutMappedRight = jmap(lfoOutRight,-1.f,1.f,0.001f,0.005f);
            
        }
        
        float delayTimeSamplesLeft = getSampleRate() * lfoOutMappedLeft;
        float delayTimeSamplesRight = getSampleRate() * lfoOutMappedRight;

        
        
        mLFOPhase += *mRateParameter / getSampleRate();
        
        if ( mLFOPhase > 1){
            mLFOPhase -= 1;
        }

        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
        
        float delayReadHeadLeft = mCircularBufferWriteHead - delayTimeSamplesLeft;
        float delayReadHeadRight = mCircularBufferWriteHead - delayTimeSamplesRight;
        
        if (delayReadHeadLeft < 0) {
            
            delayReadHeadLeft += mCircularBufferLength;
        };
        
        if (delayReadHeadRight < 0) {
            
            delayReadHeadRight += mCircularBufferLength;
        };
        
        
        int readHeadLeft_x = (int) delayReadHeadLeft;
        int readHeadLeft_x1 = readHeadLeft_x + 1;
        float readHeadFloatLeft = delayReadHeadLeft - readHeadLeft_x; // decimal part
        

        if (readHeadLeft_x1 >= mCircularBufferLength){
            readHeadLeft_x1 -= mCircularBufferLength;
        }
        
        int readHeadRight_x = (int) delayReadHeadRight;
        int readHeadRight_x1 = readHeadRight_x + 1;
        float readHeadFloatRight = delayReadHeadRight - readHeadRight_x; // decimal part
        
        
        if (readHeadRight_x1 >= mCircularBufferLength){
            readHeadRight_x1 -= mCircularBufferLength;
        }

        float delayed_sample_left = lin_interp(mCircularBufferLeft[readHeadLeft_x], mCircularBufferLeft[readHeadLeft_x1], readHeadFloatLeft);//mCircularBufferLeft[(int)mDelayReadHead];
        float delayed_sample_right = lin_interp(mCircularBufferRight[readHeadRight_x], mCircularBufferRight[readHeadRight_x1],readHeadFloatRight);//mCircularBufferRight[(int)mDelayReadHead];
        
        mFeedbackLeft = delayed_sample_left* *mFeedbackParameter;
        mFeedbackRight = delayed_sample_right* *mFeedbackParameter;

        mCircularBufferWriteHead++;

        
        buffer.setSample(0,i,buffer.getSample(0,i) * (1 - *mDryWetParameter) + delayed_sample_left * *mDryWetParameter);
        buffer.setSample(1,i,buffer.getSample(1,i) * (1 - *mDryWetParameter) + delayed_sample_right * *mDryWetParameter);

        if (mCircularBufferWriteHead >= mCircularBufferLength){
            //reset circular buffer index to initial position
            mCircularBufferWriteHead = 0;
            
        }
        
        
    }
}

//==============================================================================
bool KadenzeChorusAndFlangerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KadenzeChorusAndFlangerAudioProcessor::createEditor()
{
    return new KadenzeChorusAndFlangerAudioProcessorEditor (*this);
}

//==============================================================================
void KadenzeChorusAndFlangerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KadenzeChorusAndFlangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KadenzeChorusAndFlangerAudioProcessor();
}

float KadenzeChorusAndFlangerAudioProcessor::lin_interp(float inSampleX, float inSampleY, float inFloatPhase){
    
    return (1-inFloatPhase)* inSampleX + inFloatPhase*inSampleY;
}
