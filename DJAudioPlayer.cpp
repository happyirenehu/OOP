///*
//==============================================================================
//
//DJAudioPlayer.cpp
//Created: 13 Mar 2020 4:22:22pm
//Author:  matthew
//
//==============================================================================
//*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager): formatManager(_formatManager) {
    
}
DJAudioPlayer::~DJAudioPlayer() {
    
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL) {
    
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        transportSource.start();
    }
    
}
void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain >1.0) {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    } else {
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio >10.0) {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 10");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}
void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos >1.0) {
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start() {
    transportSource.start();
}
void DJAudioPlayer::stop() {
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative() {
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::fastForward(){
    double lastPosition = transportSource.getLengthInSeconds(); // get the length in seconds, showcasing what the last file length was
    int currentPosition = transportSource.getCurrentPosition(); // get the current position  of the audio
    
    if (currentPosition + 1.5 != lastPosition && currentPosition + 1.5 > lastPosition)
    {
        transportSource.setPosition(currentPosition + 5.0); // add a value of 2 to the current position
    }
}
//
///*
//==============================================================================
//
//DJAudioPlayer.cpp
//Created: 13 Mar 2020 4:22:22pm
//Author:  matthew
//
//==============================================================================
//*/
//
//#include "DJAudioPlayer.h"
//
//DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
//: formatManager(_formatManager)
//{
//
//}
//DJAudioPlayer::~DJAudioPlayer()
//{
//
//}
//
//void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
//{
//    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
//    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
//}
//void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
//{
//    resampleSource.getNextAudioBlock(bufferToFill);
//
//}
//void DJAudioPlayer::releaseResources()
//{
//    transportSource.releaseResources();
//    resampleSource.releaseResources();
//}
//
//void DJAudioPlayer::loadURL(URL audioURL)
//{
//    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
//    if (reader != nullptr) // good file!
//    {       
//        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, 
//true)); 
//        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
//        readerSource.reset (newSource.release());          
//    }
//}
//void DJAudioPlayer::setGain(double gain)
//{
//    if (gain < 0 || gain > 1.0)
//    {
//        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
//    }
//    else {
//        transportSource.setGain(gain);
//    }
//   
//}
//void DJAudioPlayer::setSpeed(double ratio)
//{
//  if (ratio < 0 || ratio > 100.0)
//    {
//        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
//    }
//    else {
//        resampleSource.setResamplingRatio(ratio);
//    }
//}
//void DJAudioPlayer::setPosition(double posInSecs)
//{
//    transportSource.setPosition(posInSecs);
//}
//
//void DJAudioPlayer::setPositionRelative(double pos)
//{
//     if (pos < 0 || pos > 1.0)
//    {
//        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
//    }
//    else {
//        double posInSecs = transportSource.getLengthInSeconds() * pos;
//        setPosition(posInSecs);
//    }
//}
//
//
//void DJAudioPlayer::start()
//{
//    transportSource.start();
//}
//void DJAudioPlayer::stop()
//{
//  transportSource.stop();
//}
//
//double DJAudioPlayer::getPositionRelative()
//{
//    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
//}
