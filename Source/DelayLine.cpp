#include "DelayLine.h"

DelayLine::DelayLine()
{
    writeIndex = 0;
    chunkReadIndex = 0;
}

DelayLine::~DelayLine() {}

// increase delay line size if need be (no shrinking delay line for now)
void DelayLine::setSize(int newNumSamples)
{
    if (newNumSamples > buffer.getNumSamples())
    {
        buffer.setSize(1, newNumSamples, true, true);
    }
}

// add samples from buffer to delay line
void DelayLine::addFrom(const AudioBuffer<float> &source, int sourceChannel, int sourceStartSample, int numSamples)
{
    // either simple copy
    if ( writeIndex + numSamples <= buffer.getNumSamples() )
    {
        buffer.copyFrom(0, writeIndex, source, 0, 0, numSamples);
    }
    
    // or circular copy (last samples of audio buffer will go at delay line buffer begining)
    else
    {
        int numSamplesTail = buffer.getNumSamples() - writeIndex;
        buffer.copyFrom(0, writeIndex, source, 0, 0, numSamplesTail);
        buffer.copyFrom(0, 0, source, 0, numSamplesTail, numSamples - numSamplesTail);
    }
}

// increment write position, apply circular shift if needed
void DelayLine::incrementWritePosition(int numSamples)
{
    writeIndex += numSamples;
    writeIndex %= buffer.getNumSamples();
}

void DelayLine::initBufferSize(int newNumSamples)
{
    buffer.setSize(1, newNumSamples);
    buffer.clear();
    chunkBuffer.setSize(1, newNumSamples);
    chunkBuffer.clear();
}

// get delayed buffer out of delay line
AudioBuffer<float> DelayLine::getChunk(int numSamples, int delayInSamples)
{

    int writePos = writeIndex - delayInSamples;
    
    if ( writePos < 0 )
    {
        writePos = buffer.getNumSamples() + writePos;
        if ( writePos < 0 ) // if after an update the first delay force to go fetch far to far: not best option yet (to set write pointer to zero)
            writePos = 0;
    }
    
    if ( ( writePos + numSamples ) < buffer.getNumSamples() )
    { // simple copy
        chunkBuffer.copyFrom(0, 0, buffer, 0, writePos, numSamples);
    }
    else
    { // circular loop
        int numSamplesTail = buffer.getNumSamples() - writePos;
        chunkBuffer.copyFrom(0, 0, buffer, 0, writePos, numSamplesTail );
        chunkBuffer.copyFrom(0, numSamplesTail, buffer, 0, 0, numSamples - numSamplesTail);
    }
    
    return chunkBuffer;
}