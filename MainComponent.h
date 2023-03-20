///*
//  ==============================================================================
//
//    This file was auto-generated!
//
//  ==============================================================================
//*/
//
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "PlayListComponent.h"
#include "DeckGUI.h"


//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainComponent  : public juce::AudioAppComponent,
public juce::Button::Listener,
public juce::Slider::Listener,
public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    /** implement Button::Listener*/
    void buttonClicked(juce::Button*) override;
    
    /** implement Slider::Listener*/
    void sliderValueChanged (juce::Slider *slider) override;
    void timerCallback() override;
    
    
private:
    //==============================================================================
    // Your private member variables go here...
    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};
    
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache};
    
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache};
    juce::MixerAudioSource mixerSource;
    
    PlaylistComponent playlistComponent/*{&deckGUI1, &deckGUI2}*/;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
///*
//  ==============================================================================
//
//    This file was auto-generated!
//
//  ==============================================================================
//*/
//
//#pragma once
//
//#include "../JuceLibraryCode/JuceHeader.h"
//#include "DJAudioPlayer.h"
//#include "DeckGUI.h"
//#include "PlayListComponent.h"
//
////==============================================================================
///*
//    This component lives inside our window, and this is where you should put all
//    your controls and content.
//*/
//class MainComponent   : public AudioAppComponent
//{
//public:
//    //==============================================================================
//    MainComponent();
//    ~MainComponent();
//
//    //==============================================================================
//    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
//    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
//    void releaseResources() override;
//
//    //==============================================================================
//    void paint (Graphics& g) override;
//    void resized() override;
//
//private:
//    //==============================================================================
//    // Your private member variables go here...
//
//    AudioFormatManager formatManager;
//    AudioThumbnailCache thumbCache{100};
//
//    DJAudioPlayer player1{formatManager};
//    DeckGUI deckGUI1{&player1, formatManager, thumbCache};
//
//    DJAudioPlayer player2{formatManager};
//    DeckGUI deckGUI2{&player2, formatManager, thumbCache};
//
//    MixerAudioSource mixerSource;
//
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
//};
