/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeChorusAndFlangerAudioProcessorEditor::KadenzeChorusAndFlangerAudioProcessorEditor (KadenzeChorusAndFlangerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);
    
    auto& params = processor.getParameters();
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* depthParameter= (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* rateParameter= (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterFloat* phaseParameter= (AudioParameterFloat*)params.getUnchecked(3);
    AudioParameterFloat* feedbackParameter = (AudioParameterFloat*)params.getUnchecked(4);
    
    std::array<AudioParameterFloat *,5> pArray {dryWetParameter, depthParameter, rateParameter,phaseParameter,feedbackParameter};
    std::array<Slider *, 5> sArray {&mDryWetSlider, &mDepthSlider, &mRateSlider, &mPhaseOffsetSlider,&mFeedbackSlider};
    
    initializeSlider(sArray,pArray);

    
    //Type
    AudioParameterInt* typeParameter = (AudioParameterInt*)params.getUnchecked(5);
    mType.setBounds(0, 100, 100, 30);
    mType.addItem("Chorus",1);
    mType.addItem("Flanger",2);
    addAndMakeVisible(mType);
    
    mType.onChange = [this, typeParameter]{
        typeParameter->beginChangeGesture();
        *typeParameter = mType.getSelectedItemIndex();
        typeParameter->endChangeGesture();
    };
    
    mType.setSelectedItemIndex(*typeParameter);
    
}

KadenzeChorusAndFlangerAudioProcessorEditor::~KadenzeChorusAndFlangerAudioProcessorEditor()
{
}

//==============================================================================
void KadenzeChorusAndFlangerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::lightviolet); // Colours::violet
    g.setFont (15.0f);
    g.drawFittedText ("Chorus and Flanger", getLocalBounds(), Justification::centred, 1);
}

void KadenzeChorusAndFlangerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void KadenzeChorusAndFlangerAudioProcessorEditor::initializeSlider(std::array<Slider *,5> s, std::array<AudioParameterFloat *,5> p)
{
    for (int i=0; i < s.size();i++){
        
        s.at(i)->setBounds((i)*100,0,100,100);
        s.at(i)->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        s.at(i)->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 30);
        s.at(i)->setRange(p.at(i)->range.start, p.at(i)->range.end);
        s.at(i)->setValue(*p.at(i));
        addAndMakeVisible(s.at(i));
        
        s.at(i)->onValueChange = [=] { *p.at(i) = s.at(i)->getValue() ;};
        s.at(i)->onDragStart = [=] { p.at(i)->beginChangeGesture();};
        s.at(i)->onDragEnd = [=] { p.at(i)->endChangeGesture();};
    }
    
}
