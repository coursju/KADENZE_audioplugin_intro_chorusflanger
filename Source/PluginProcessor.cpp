/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeChorusFlangerAudioProcessor::KadenzeChorusFlangerAudioProcessor()
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
    //addparameters
    addParameter(mDryWetParameter = new AudioParameterFloat("drywet",
        "Dry Wet",
        0.01,
        1.0,
        0.5));

    addParameter(mFeedbackParameter = new AudioParameterFloat("feedback",
        "Feedback",
        0,
        0.98,
        0.5));

    addParameter(mDephParameter = new AudioParameterFloat("deph",
        "Deph",
        0.0,
        1.0,
        0.5));

    addParameter(mRateParameter = new AudioParameterFloat("rate",
        "Rate",
        0.1,
        20.0,
        10.0));

    addParameter(mPhaseOffsetParameter = new AudioParameterFloat("phaseoffset",
        "Phase Ofset",
        0.0,
        1.0,
        0.0));

    addParameter(mTypeParameter = new AudioParameterFloat("type",
        "Type",
        0.0,
        1.0,
        0.0));

    mDelayTimeSmoothed = 0;

    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLenght = 0;
    mDelayTimeInSamples = 0;
    mDelayReadHead = 0;

    mFeedbackLeft = 0;
    mFeedbackRight = 0;

    mLFOPhase = 0;

}

KadenzeChorusFlangerAudioProcessor::~KadenzeChorusFlangerAudioProcessor()
{
}

//==============================================================================
const String KadenzeChorusFlangerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KadenzeChorusFlangerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeChorusFlangerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeChorusFlangerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KadenzeChorusFlangerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KadenzeChorusFlangerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KadenzeChorusFlangerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KadenzeChorusFlangerAudioProcessor::setCurrentProgram (int index)
{
}

const String KadenzeChorusFlangerAudioProcessor::getProgramName (int index)
{
    return {};
}

void KadenzeChorusFlangerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void KadenzeChorusFlangerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mCircularBufferLenght = sampleRate * MAX_DELAY_TIME;

    mLFOPhase = 0;

    if (mCircularBufferLeft == nullptr) {
        mCircularBufferLeft = new float[mCircularBufferLenght];
        for (int i = 0; i < mCircularBufferLenght; i++) {
            mCircularBufferLeft[i] = 0;
        }

    }
    //autre facon d initialiser le mCircularBufferLeft
    // zeromem( mCircularBufferLeft, mCircularBufferLenght * sizeof(float));

    if (mCircularBufferRight == nullptr) {
        mCircularBufferRight = new float[mCircularBufferLenght];
        for (int i = 0; i < mCircularBufferLenght; i++) {
            mCircularBufferRight[i] = 0;
        }
    }
    //autre facon d initialiser le mCircularBufferRight
    // zeromem( mCircularBufferRight, mCircularBufferLenght * sizeof(float));


    mCircularBufferWriteHead = 0;
    mDelayTimeInSamples = sampleRate * 0.5;

    //pour adoucir le son lors de la rotation du parameter
    mDelayTimeSmoothed = 1;
}

void KadenzeChorusFlangerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KadenzeChorusFlangerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void KadenzeChorusFlangerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // visualisation des variations des parametres
    DBG("DRY WET: " << *mDryWetParameter);
    DBG("DEPTH: " << *mDephParameter);
    DBG("RATE: " << *mRateParameter);
    DBG("PHASE: " << *mPhaseOffsetParameter);
    DBG("FEEDBACK: " << *mFeedbackParameter);
    DBG("TYPE: " << *mTypeParameter);


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    //mDelayTimeInSamples = getSampleRate() * *mDelayTimeParameter;

    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);


    for (int i = 0; i < buffer.getNumSamples(); i++) {

        //Left
        float lfoOutLeft = sin(2 * 3.14f * mLFOPhase); //M_PI  3.14f
        lfoOutLeft *= *mDephParameter;

        //Right
        float lfoPhaseRight = mLFOPhase + *mPhaseOffsetParameter;

        if (lfoPhaseRight > 1) {
            lfoPhaseRight -= 1;
        }
        float lfoOutRight = sin(2 * 3.14f * mLFOPhase); //M_PI  3.14f
        lfoOutRight *= *mDephParameter;

        float lfoOutMappedLeft = 0;
        float lfoOutMappedRight = 0;

        //***chorus***
        if (*mTypeParameter == 0) {
             lfoOutMappedLeft = jmap(lfoOutLeft, -1.0f, 1.0f, 0.005f, 0.03f);
             lfoOutMappedRight = jmap(lfoOutRight, -1.0f, 1.0f, 0.005f, 0.03f);
        }
        else {
             lfoOutMappedLeft = jmap(lfoOutLeft, -1.0f, 1.0f, 0.001f, 0.005f);
             lfoOutMappedRight = jmap(lfoOutRight, -1.0f, 1.0f, 0.001f, 0.005f);
        }

        float delayTimeSamplesLeft = getSampleRate() * lfoOutMappedLeft;
        float delayTimeSamplesRight = getSampleRate() * lfoOutMappedRight;

        mLFOPhase += *mRateParameter / getSampleRate();

        if (mLFOPhase > 1){
            mLFOPhase -= 1;
        }



        //mDelayTimeSmoothed = mDelayTimeSmoothed - 0.001f * (mDelayTimeSmoothed - lfoOutMapped);


        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;

        float delayReadHeadLeft = mCircularBufferWriteHead - delayTimeSamplesLeft;
        if (delayReadHeadLeft < 0){
            delayReadHeadLeft += mCircularBufferLenght;
        }

        float delayReadHeadRight = mCircularBufferWriteHead - delayTimeSamplesRight;
        if (delayReadHeadRight < 0) {
            delayReadHeadRight += mCircularBufferLenght;
        }
    /*    if (mDelayReadHead < 0) {
            mDelayReadHead += mCircularBufferLenght;
        }*/

        //interpolation lineaire
        int readHeadLeft_x = (int)delayReadHeadLeft;
        int readHeadLeft_x1 = readHeadLeft_x + 1;
        float readHeadFloatLeft = delayReadHeadLeft - readHeadLeft_x;

        int readHeadRight_x = (int)delayReadHeadRight;
        int readHeadRight_x1 = readHeadRight_x + 1;
        float readHeadFloatRight = delayReadHeadRight - readHeadRight_x;

        if (readHeadLeft_x1 >= mCircularBufferLenght)readHeadLeft_x1 -= mCircularBufferLenght;
        if (readHeadRight_x1 >= mCircularBufferLenght)readHeadRight_x1 -= mCircularBufferLenght;

        float delay_sample_left = lin_interp(mCircularBufferLeft[readHeadLeft_x], mCircularBufferLeft[readHeadLeft_x1], readHeadFloatLeft);
        float delay_sample_right = lin_interp(mCircularBufferRight[readHeadRight_x], mCircularBufferRight[readHeadRight_x1], readHeadFloatRight);

        //*****
        /*float delay_sample_left = mCircularBufferLeft[(int)mDelayReadHead];
        float delay_sample_right = mCircularBufferRight[(int)mDelayReadHead];*/

        mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
        mFeedbackRight = delay_sample_right * *mFeedbackParameter;


        //buffer.addSample(0, i, mCircularBufferLeft[(int)mDelayReadHead]);
        //buffer.addSample(1, i, mCircularBufferRight[(int)mDelayReadHead]);

        mCircularBufferWriteHead++;


        /* buffer.addSample(0, i, delay_sample_left);
         buffer.addSample(1, i, delay_sample_right);*/
        buffer.setSample(0, i, buffer.getSample(0, i) * (1 - *mDryWetParameter) + delay_sample_left * *mDryWetParameter);
        buffer.setSample(1, i, buffer.getSample(1, i) * (1 - *mDryWetParameter) + delay_sample_right * *mDryWetParameter);

        // mCircularBufferWriteHead++;


        if (mCircularBufferWriteHead >= mCircularBufferLenght) {
            mCircularBufferWriteHead = 0;
        }

    }
}

//==============================================================================
bool KadenzeChorusFlangerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KadenzeChorusFlangerAudioProcessor::createEditor()
{
    return new KadenzeChorusFlangerAudioProcessorEditor (*this);
}

//==============================================================================
void KadenzeChorusFlangerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KadenzeChorusFlangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KadenzeChorusFlangerAudioProcessor();
}
float KadenzeChorusFlangerAudioProcessor::lin_interp(float sample_x, float sample_x1, float inPhase) {
    return (1 - inPhase) * sample_x + inPhase * sample_x1;
}
