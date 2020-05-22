/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class KadenzeChorusAndFlangerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    KadenzeChorusAndFlangerAudioProcessorEditor (KadenzeChorusAndFlangerAudioProcessor&);
    ~KadenzeChorusAndFlangerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    KadenzeChorusAndFlangerAudioProcessor& processor;
    
    Slider mDryWetSlider;
    Slider mFeedbackSlider;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeChorusAndFlangerAudioProcessorEditor)
};
