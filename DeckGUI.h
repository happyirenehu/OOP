///*
//  ==============================================================================
//
//    DeckGUI.h
//    Created: 13 Mar 2020 6:44:48pm
//    Author:  matthew
//
//  ==============================================================================
//*/
//

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
//#include "PlayListComponent.h"

//==============================================================================
/*
 */
class DeckGUI  : public juce::Component,
public juce::Button::Listener,
public juce::Slider::Listener,
public juce::FileDragAndDropTarget,
public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager & formatManagerToUse,
            juce::AudioThumbnailCache & cacheToUse);
    ~DeckGUI();
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** implement Button::Listener*/
    void buttonClicked(juce::Button*) override;
    
    /** implement Slider::Listener*/
    void sliderValueChanged(juce::Slider* slider) override;
    
    bool isInterestedInFileDrag(const juce::StringArray &files) override; // fileDropped function
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    void timerCallback() override; //to check if the loop box is checked
    void addLibraryToDeck(); //it's to load the library to DeckGUI through files.
    void loadFile(juce::URL importedURL); //load the URL to DJaudioplayer to play, was origianl planned for playlistcomponent but can't link both class.....
private:
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::ToggleButton loopButton{ "LOOP" };
    juce::TextEditor songName;
    juce::String msongName;
    juce::TextButton loadButton{ "LOAD" };
    juce::Label volSliderLabel;
    juce::Label speedSliderLabel;
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    
    TextButton fastForwardButton{"FAST FORWARD"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
    
};
///*
//  ==============================================================================
//
//    DeckGUI.h
//    Created: 13 Mar 2020 6:44:48pm
//    Author:  matthew
//
//  ==============================================================================
//*/
//
//#pragma once
//
//#include "../JuceLibraryCode/JuceHeader.h"
//#include "DJAudioPlayer.h"
//#include "WaveformDisplay.h"
//#include "PlayListComponent.h"
//
////==============================================================================
///*
//*/
//class DeckGUI    : public Component,
//                   public Button::Listener,
//                   public Slider::Listener,
//                   public FileDragAndDropTarget,
//                   public Timer
//{
//public:
//    DeckGUI(DJAudioPlayer* player,
//           AudioFormatManager &     formatManagerToUse,
//           AudioThumbnailCache &     cacheToUse );
//    ~DeckGUI();
//
//    void paint (Graphics&) override;
//    void resized() override;
//
//     /** implement Button::Listener */
//    void buttonClicked (Button *) override;
//
//    /** implement Slider::Listener */
//    void sliderValueChanged (Slider *slider) override;
//
//    bool isInterestedInFileDrag (const StringArray &files) override;
//    void filesDropped (const StringArray &files, int x, int y) override;
//
//    void timerCallback() override;
//
//private:
//
//    TextButton playButton{"PLAY"};
//    TextButton stopButton{"STOP"};
//    TextButton loadButton{"LOAD"};
//
//    Slider volSlider;
//    Slider speedSlider;
//    Slider posSlider;
//
//    FileChooser fChooser{"Select a file..."};
//
//
//    WaveformDisplay waveformDisplay;
//
//    DJAudioPlayer* player;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
//};
