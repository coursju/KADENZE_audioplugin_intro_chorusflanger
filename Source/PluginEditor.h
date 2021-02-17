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
class KadenzeChorusFlangerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    KadenzeChorusFlangerAudioProcessorEditor (KadenzeChorusFlangerAudioProcessor&);
    ~KadenzeChorusFlangerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

    Slider mDryWetSlider;
    Slider mDephSlider;
    Slider mRateSlider;
    Slider mPhaseOffsetSlider;
    Slider mFeedbackSlider;

    ComboBox mType;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KadenzeChorusFlangerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeChorusFlangerAudioProcessorEditor)
};
