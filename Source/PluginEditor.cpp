/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeChorusFlangerAudioProcessorEditor::KadenzeChorusFlangerAudioProcessorEditor (KadenzeChorusFlangerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);


    auto& params = processor.getParameters();

    //mDryWetSlider
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(0);
    mDryWetSlider.setBounds(0, 0, 100, 100);
    mDryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end);
    mDryWetSlider.setValue(*dryWetParameter);
    mDryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(mDryWetSlider);

    mDryWetSlider.onValueChange = [this, dryWetParameter] {*dryWetParameter = mDryWetSlider.getValue(); };
    mDryWetSlider.onDragStart = [dryWetParameter] {dryWetParameter->beginChangeGesture(); };
    mDryWetSlider.onDragEnd = [dryWetParameter] {dryWetParameter->endChangeGesture(); };

    //mDephSlider
    AudioParameterFloat* depthParameter = (AudioParameterFloat*)params.getUnchecked(1);
    mDephSlider.setBounds(100, 0, 100, 100);
    mDephSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    mDephSlider.setRange(depthParameter->range.start, depthParameter->range.end);
    mDephSlider.setValue(*depthParameter);
    mDephSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(mDephSlider);

    mDephSlider.onValueChange = [this, depthParameter] {*depthParameter = mDephSlider.getValue(); };
    mDephSlider.onDragStart = [depthParameter] {depthParameter->beginChangeGesture(); };
    mDephSlider.onDragEnd = [depthParameter] {depthParameter->endChangeGesture(); };

    //mRateSlider
    AudioParameterFloat* rateParameter = (AudioParameterFloat*)params.getUnchecked(2);
    mRateSlider.setBounds(200, 0, 100, 100);
    mRateSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    mRateSlider.setRange(rateParameter->range.start, rateParameter->range.end);
    mRateSlider.setValue(*rateParameter);
    mRateSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(mRateSlider);

    mRateSlider.onValueChange = [this, rateParameter] {*rateParameter = mRateSlider.getValue(); };
    mRateSlider.onDragStart = [rateParameter] {rateParameter->beginChangeGesture(); };
    mRateSlider.onDragEnd = [rateParameter] {rateParameter->endChangeGesture(); };

    //mPhaseOffsetSlider
    AudioParameterFloat* phaseOffsetParameter = (AudioParameterFloat*)params.getUnchecked(3);
    mPhaseOffsetSlider.setBounds(300, 0, 100, 100);
    mPhaseOffsetSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    mPhaseOffsetSlider.setRange(phaseOffsetParameter->range.start, phaseOffsetParameter->range.end);
    mPhaseOffsetSlider.setValue(*phaseOffsetParameter);
    mPhaseOffsetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(mPhaseOffsetSlider);

    mPhaseOffsetSlider.onValueChange = [this, phaseOffsetParameter] {*phaseOffsetParameter = mPhaseOffsetSlider.getValue(); };
    mPhaseOffsetSlider.onDragStart = [phaseOffsetParameter] {phaseOffsetParameter->beginChangeGesture(); };
    mPhaseOffsetSlider.onDragEnd = [phaseOffsetParameter] {phaseOffsetParameter->endChangeGesture(); };

    //mFeedbackSlider
    AudioParameterFloat* feedbackParameter = (AudioParameterFloat*)params.getUnchecked(4);
    mFeedbackSlider.setBounds(0, 100, 100, 100);
    mFeedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    mFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end);
    mFeedbackSlider.setValue(*feedbackParameter);
    mFeedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(mFeedbackSlider);

    mFeedbackSlider.onValueChange = [this, feedbackParameter] {*feedbackParameter = mFeedbackSlider.getValue(); };
    mFeedbackSlider.onDragStart = [feedbackParameter] {feedbackParameter->beginChangeGesture(); };
    mFeedbackSlider.onDragEnd = [feedbackParameter] {feedbackParameter->endChangeGesture(); };

    //mType
    AudioParameterInt* typeParameter = (AudioParameterInt*)params.getUnchecked(5);

    mType.setBounds(100, 100, 100, 30);
    mType.addItem("chorus", 1);
    mType.addItem("flanger", 2); 
    addAndMakeVisible(mType);

    mType.onChange = [this, typeParameter]{
        typeParameter->beginChangeGesture();
    *typeParameter = mType.getSelectedItemIndex();
    typeParameter->endChangeGesture();
    };
    mType.setSelectedItemIndex(*typeParameter);
}

KadenzeChorusFlangerAudioProcessorEditor::~KadenzeChorusFlangerAudioProcessorEditor()
{
}

//==============================================================================
void KadenzeChorusFlangerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void KadenzeChorusFlangerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
