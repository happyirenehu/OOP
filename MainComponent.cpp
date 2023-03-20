///*
//  ==============================================================================
//
//    This file was auto-generated!
//
//  ==============================================================================
//*/

#include "MainComponent.h"
#include <iostream>

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);
    
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
                                          [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }
    
    //adds and makes the 2 deckGUIs visible on top
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    
    //adds the playlist compoment at the bottom
    addAndMakeVisible(playlistComponent);
    
    //registers basic formats for the audio files
    formatManager.registerBasicFormats();
    
    //Timer loops every 500ms, checks whether user added an audio file from playlist into a deckGUI
    startTimer(500);
    
    
    
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}


//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::rosybrown);
    
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    double h = getHeight()/3;
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight()/1.5);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight()/1.5);
    playlistComponent.setBounds(0, getHeight()/1.5, getWidth(), getHeight()/2);
}


void MainComponent::buttonClicked(juce::Button* button) {
    
}

void MainComponent::sliderValueChanged(juce::Slider* slider) {
    
}

void MainComponent::timerCallback() { //Function loops every 500ms, checks whether user has loaded a playlist audio file into a deckGUI
    
    std::ifstream file("playLibrary.txt"); //Opens playLibrary.txt
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str)) //Reads the lines in playLibrary.txt
    {
        lines.push_back(str);
    }
    //If the user added a song from playlist into deckGUI playLibrary.txt will contain the audio file URL and a number indicating which deckGUI the user selected
    if (std::stoi(lines[1]) == 1) { //If user selects deck 1, line 2 will show 1
        deckGUI1.addLibraryToDeck(); //plays audio file in deckGUI1
        std::ofstream storageFiles("playLibrary.txt");
        storageFiles << "" << std::endl << "0" << std::endl; //Changes line 2 from "1" to "0" which resets playLibrary.txt file and stops the timer from looping endlessly
        storageFiles.close(); //closes file
    }
    else if (std::stoi(lines[1]) == 2) { //If user selects deck 2, line 2 will show 2
        deckGUI2.addLibraryToDeck(); //plays audio file in deckGUI2
        std::ofstream storageFiles("playLibrary.txt");
        storageFiles << "" << std::endl << "0" << std::endl; //Changes line 2 from "2" to "0" which resets playLibrary.txt file and stops the timer from looping endlessly
        storageFiles.close(); //closes file
    }
    
}







///*
//  ==============================================================================
//
//    This file was auto-generated!
//
//  ==============================================================================
//*/
//
//#include "MainComponent.h"
//
////==============================================================================
//MainComponent::MainComponent()
//{
//    // Make sure you set the size of the component after
//    // you add any child components.
//    setSize (800, 600);
//
//    // Some platforms require permissions to open input channels so request that here
//    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
//        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
//    {
//        RuntimePermissions::request (RuntimePermissions::recordAudio,
//                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
//    }
//    else
//    {
//        // Specify the number of input and output channels that we want to open
//        setAudioChannels (0, 2);
//    }
//
//    addAndMakeVisible(deckGUI1);
//    addAndMakeVisible(deckGUI2);
//
//
//    formatManager.registerBasicFormats();
//}
//
//MainComponent::~MainComponent()
//{
//    // This shuts down the audio device and clears the audio source.
//    shutdownAudio();
//}
//
////==============================================================================
//void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
//{
//    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
//    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
//
//    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
//
//    mixerSource.addInputSource(&player1, false);
//    mixerSource.addInputSource(&player2, false);
//
// }
//void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
//{
//    mixerSource.getNextAudioBlock(bufferToFill);
//}
//
//void MainComponent::releaseResources()
//{
//    // This will be called when the audio device stops, or when it is being
//    // restarted due to a setting change.
//
//    // For more details, see the help for AudioProcessor::releaseResources()
//    player1.releaseResources();
//    player2.releaseResources();
//    mixerSource.releaseResources();
//}
//
////==============================================================================
//void MainComponent::paint (Graphics& g)
//{
//    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
//
//    // You can add your drawing code here!
//}
//
//void MainComponent::resized()
//{
//    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight());
//    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
//
//}
//
