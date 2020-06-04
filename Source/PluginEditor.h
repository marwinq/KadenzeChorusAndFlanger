/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <array>

//==============================================================================
/**
*/
class KadenzeChorusAndFlangerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    void extracted(juce::AudioParameterFloat *dryWetParameter);
    
    KadenzeChorusAndFlangerAudioProcessorEditor (KadenzeChorusAndFlangerAudioProcessor&);
    ~KadenzeChorusAndFlangerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void initializeSlider(std::array<Slider *,5> s, std::array<AudioParameterFloat *,5> p);

private:
    
    KadenzeChorusAndFlangerAudioProcessor& processor;
    
    Slider mDryWetSlider;
    Slider mDepthSlider;
    Slider mRateSlider;
    Slider mPhaseOffsetSlider;
    Slider mFeedbackSlider;
    
    ComboBox mType;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeChorusAndFlangerAudioProcessorEditor)
};
