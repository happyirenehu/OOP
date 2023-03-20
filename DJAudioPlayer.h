///*
//  ==============================================================================
//
//    DJAudioPlayer.h
//    Created: 13 Mar 2020 4:22:22pm
//    Author:  matthew
//
//  ==============================================================================
//*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "PlayListComponent.h"

class DJAudioPlayer : public juce::AudioSource {
public:
    
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void loadURL(juce::URL audioURL); //Reads the audio file from a URL
    void setGain(double gain); // Sets the audio gain from 0 to 1.0, to control the volume
    void setSpeed(double ratio); // Sets the speed from a ratio of 0 to 10.0
    void setPosition(double posInSecs); // Sets the position of the audio file from seconds
    void setPositionRelative(double pos); //Sets the position of the audio file from 0 to 1.0
    
    void start(); //playing the audio file
    void stop(); //Stops playing the audio file
    
    /** get the relative position of the playhead*/
    double getPositionRelative();
    
    void fastForward();
    
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
};

///*
//  ==============================================================================
//
//    DJAudioPlayer.h
//    Created: 13 Mar 2020 4:22:22pm
//    Author:  matthew
//
//  ==============================================================================
//*/
//
//#pragma once
//
//#include "../JuceLibraryCode/JuceHeader.h"
//
//class DJAudioPlayer : public AudioSource {
//  public:
//
//    DJAudioPlayer(AudioFormatManager& _formatManager);
//    ~DJAudioPlayer();
//
//    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
//    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
//    void releaseResources() override;
//
//    void loadURL(URL audioURL);
//    void setGain(double gain);
//    void setSpeed(double ratio);
//    void setPosition(double posInSecs);
//    void setPositionRelative(double pos);
//    
//
//    void start();
//    void stop();
//
//    /** get the relative position of the playhead */
//    double getPositionRelative();
//
//private:
//    AudioFormatManager& formatManager;
//    std::unique_ptr<AudioFormatReaderSource> readerSource;
//    AudioTransportSource transportSource; 
//    ResamplingAudioSource resampleSource{&transportSource, false, 2};
//
//};
//
//
//
//
