///*
//  ==============================================================================
//
//    DeckGUI.cpp
//    Created: 13 Mar 2020 6:44:48pm
//    Author:  matthew
//
//  ==============================================================================
//*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse)
: player(_player), waveformDisplay(formatManagerToUse,cacheToUse)


{
    //loading pause and play button images
    //auto pauseImage = juce::ImageCache::getFromMemory(BinaryData::pausebutton01_png, BinaryData::pausebutton01_pngSize);
    //auto playImage = juce::ImageCache::getFromMemory(BinaryData::playbutton01_png, BinaryData::playbutton01_pngSize);
    
    //adding and making buttons,sliders,waveforms and texts visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(songName);
    addAndMakeVisible(loopButton);
    //addAndMakeVisible(fastForwardButton);
    
    playButton.setClickingTogglesState(true);
    playButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::snow);
    playButton.setColour(TextButton::ColourIds::buttonColourId, Colours::deeppink);
    playButton.setColour(TextButton::ColourIds::textColourOffId, Colours::snow);
    playButton.setColour(TextButton::ColourIds::textColourOnId, Colours::black);
    stopButton.setClickingTogglesState(true);
    stopButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::snow);
    stopButton.setColour(TextButton::ColourIds::buttonColourId, Colours::deeppink);
    stopButton.setColour(TextButton::ColourIds::textColourOffId, Colours::snow);
    stopButton.setColour(TextButton::ColourIds::textColourOnId, Colours::black);
    
    loadButton.setClickingTogglesState(true);
    loadButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::lightpink);
    loadButton.setColour(TextButton::ColourIds::buttonColourId, Colours::snow);
    loadButton.setColour(TextButton::ColourIds::textColourOffId, Colours::black);

    //adding listeners to buttons and sliders
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    loopButton.addListener(this);
    //fastForwardButton.addListener(this);
    //Setting stop and play button with images
    //stopButton.setImages(true,true,true,pauseImage, 0.5f, juce::Colours::transparentBlack, pauseImage, 1.0f, juce::Colours::transparentBlack, pauseImage, 0.5f, juce::Colours::transparentBlack, 0.0f);
    //playButton.setImages(true, true, true, playImage, 0.5f, juce::Colours::transparentBlack, playImage, 1.0f, juce::Colours::transparentBlack, playImage, 0.5f, juce::Colours::transparentBlack, 0.0f);
    
    //setting volume, speed and position sliders with ranges.
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);
    
    //loop button style

    //changing volume and speed sliders into rotary knobs
    volSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    volSlider.setColour(Slider::rotarySliderOutlineColourId, Colours::rosybrown);
    speedSlider.setColour(Slider::rotarySliderOutlineColourId, Colours::rosybrown);
    speedSlider.setColour(Slider::backgroundColourId, Colours::rosybrown);
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::saddlebrown);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::lightcoral);
    //adding text to vol and speed slider
    volSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 90,25);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 90, 25);
    
    //attaching text to the volume slider component
    volSliderLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    volSliderLabel.attachToComponent(&volSlider, false);
    volSliderLabel.setJustificationType(Justification::centred);
     volSliderLabel.setColour(Label::textColourId, Colours::darkslategrey);
    
    //attaching text to the speed slider component
    speedSliderLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, false);
    speedSliderLabel.setColour(Label::textColourId, Colours::darkslategrey);
    speedSliderLabel.setJustificationType(Justification::centred);
    
    //track name displayed on top of the deck is only readable
    songName.setReadOnly(true);
    songName.setTextToShowWhenEmpty("No track loaded", juce::Colours::darkgrey);
    
    
    
    
    //making label text visible
    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(volSliderLabel);
    
    //the position slider which is the playback slider is set as transparent and overlayed above the waveform display,
    //this allows the user to directly click on the waveform display to navigate through the audio file's playback
    posSlider.setSliderStyle(juce::Slider::LinearBar);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox,true,0,0);


    posSlider.setColour(juce::Slider::trackColourId,juce::Colours::transparentWhite);
    
    //Timer loops every 100ms, checks whether user has the "loop" button turned on or off and also updates the current position in the waveform display
    startTimer(100);
    
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
     draws some placeholder text to get you started.
     
     You should replace everything in this method with your own
     drawing code..
     */
    
    g.fillAll(juce::Colours::black);   // clear the background
    g.fillAll(juce::Colours::lightpink);
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    g.setColour (juce::Colours::orange);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    
    double rowH = getHeight() / 7;
    double mid = getWidth()/2;
    playButton.setBounds(mid/1.4, rowH*2.5, getWidth()/7, rowH/1.2);
    stopButton.setBounds(mid, rowH*2.5, getWidth() / 7, rowH/1.2);
    
    //playButton.setBounds((getWidth() / 7)*5, 0, getWidth()/7, rowH);
    //stopButton.setBounds((getWidth() / 7)*6, 0, getWidth() / 7, rowH);
    
//    playButton.setBounds((getWidth() / 7)*5, 0, getWidth()/7, rowH);
//    stopButton.setBounds((getWidth() / 7)*6, 0, getWidth() / 7, rowH);
    songName.setBounds((getWidth() / 7) * 2,0,(getWidth() / 7)*3, rowH);
    loopButton.setBounds((getWidth() / 7)*5, 0, getWidth() / 7, rowH);
    loadButton.setBounds(mid/1.4, rowH*1.5, getWidth()/7*2, rowH/1.2);
    
    double sliderRow = getWidth()/4;
    volSlider.setBounds(0, rowH*1.5, getWidth()/4, rowH*2);
    speedSlider.setBounds(sliderRow*3, rowH*1.5, getWidth()/4, rowH*2);
    
    //fastForwardButton.setBounds(150, rowH*1.2, getWidth()/4, rowH*2);
    
    //volSliderLabel.setCentreRelative(0.43f, 0.4f);
    //speedSliderLabel.setCentreRelative(0.94f, 0.4f);
    
    
    songName.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    songName.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    
    
    
    
    waveformDisplay.setBounds(0, rowH * 4, getWidth(), rowH*3);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH * 3);
    
    
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) { //If button is clicked, start the audio file
        DBG("Play button is clicked");
        player->start();
    }
    if (button == &stopButton) { //If button is clicked, stops the audio file
        DBG("Stop button is clicked");
        player->stop();
    }
    if(button == &fastForwardButton){
        DBG("fast forward button is clicked");
        player->fastForward();
    }
    if (button == &loadButton) { //If button is clicked, allows user to select file to be loaded into the deck
        juce::FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen()) {
            player->loadURL(juce::URL{ chooser.getResult() });
            waveformDisplay.loadURL(juce::URL{ chooser.getResult() });
            DBG(juce::URL{ chooser.getResult() }.getFileName());
            

            songName.setColour(TextEditor::textColourId, Colours::darkgrey);
            songName.setText(juce::URL{ chooser.getResult() }.getFileName(), juce::NotificationType::dontSendNotification);
        }
    }
   

    
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volSlider) { //Sets audio file gain as vol slider value
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {// Sets audio file speed as speed slider value
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider) { //Sets audio file position as the position slider value
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) {
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1) {
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::timerCallback() { //Timer loops every 100ms, which checks if the "loop" button is on or off and also updates the wave form display current position
    if (std::to_string(loopButton.getToggleState()) == "1") { //If loop button is checked
        if (player->getPositionRelative() >= 1) {// and the position more than 1 meaning it is the end of the audio file
            player->setPositionRelative(0); // Sets position back to the start of the audio file
            player->start(); // Plays the audio file
        }
    }
    
    if (std::to_string(loopButton.getToggleState()) == "0") { //If loop button is not checked
        if (player->getPositionRelative() >= 1) { //and the position more than 1 meaning it is the end of the audio file
            player->setPositionRelative(0); //Sets position back to the start of the audio file
            player->stop(); // Stops the audio file
        }
    }
    waveformDisplay.setPositionRelative(player->getPositionRelative()); //Every 100ms, updates the waveform display to show current timestamp for the audio file
}
void DeckGUI::loadFile(juce::URL importedURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(importedURL);
    waveformDisplay.loadURL(importedURL);
}

void DeckGUI::addLibraryToDeck() { //function rea
    
//    std::ifstream file("playLibrary.txt"); //Opens playLibrary.txt which contains the audio file URL which the user loads from the playlist into the deck
//    std::string str;
//    std::getline(file, str);
//    std::string URL = "file:///" + str; //adds file:/// to the audio file URL, which converts the URL into a file
//    DBG(URL);
//    juce::URL audioURL{ URL };
//    player->loadURL(audioURL); //Loads the file into the DJAudioPlayer
//    waveformDisplay.loadURL(audioURL); //Loads the file and returns the waveform of the audio file
//    DBG(audioURL.getFileName());
//
//    msongName = audioURL.getFileName(); //Gets the file name from the URL
//
//    songName.setText(msongName, juce::NotificationType::dontSendNotification);
    
}
///*
//  ==============================================================================
//
//    DeckGUI.cpp
//    Created: 13 Mar 2020 6:44:48pm
//    Author:  matthew
//
//  ==============================================================================
//*/
//
//#include "../JuceLibraryCode/JuceHeader.h"
//#include "DeckGUI.h"
//
////==============================================================================
//DeckGUI::DeckGUI(DJAudioPlayer* _player,
//                AudioFormatManager &     formatManagerToUse,
//                AudioThumbnailCache &     cacheToUse
//           ) : player(_player),
//               waveformDisplay(formatManagerToUse, cacheToUse)
//{
//
//    addAndMakeVisible(playButton);
//    addAndMakeVisible(stopButton);
//    addAndMakeVisible(loadButton);
//
//    addAndMakeVisible(volSlider);
//    addAndMakeVisible(speedSlider);
//    addAndMakeVisible(posSlider);
//
//    addAndMakeVisible(waveformDisplay);
//
//
//    playButton.addListener(this);
//    stopButton.addListener(this);
//    loadButton.addListener(this);
//
//    volSlider.addListener(this);
//    speedSlider.addListener(this);
//    posSlider.addListener(this);
//
//
//    volSlider.setRange(0.0, 1.0);
//    speedSlider.setRange(0.0, 100.0);
//    posSlider.setRange(0.0, 1.0);
//
//    startTimer(500);
//
//
//}
//
//DeckGUI::~DeckGUI()
//{
//    stopTimer();
//}
//
//void DeckGUI::paint (Graphics& g)
//{
//    /* This demo code just fills the component's background and
//       draws some placeholder text to get you started.
//
//       You should replace everything in this method with your own
//       drawing code..
//    */
//
//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
//
//    g.setColour (Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//    g.setColour (Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("DeckGUI", getLocalBounds(),
//                Justification::centred, true);   // draw some placeholder text
//}
//
//void DeckGUI::resized()
//{
//    double rowH = getHeight() / 8;
//    playButton.setBounds(0, 0, getWidth(), rowH);
//    stopButton.setBounds(0, rowH, getWidth(), rowH);
//    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
//    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
//    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
//    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
//    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
//
//}
//
//void DeckGUI::buttonClicked(Button* button)
//{
//    if (button == &playButton)
//    {
//        std::cout << "Play button was clicked " << std::endl;
//        player->start();
//    }
//     if (button == &stopButton)
//    {
//        std::cout << "Stop button was clicked " << std::endl;
//        player->stop();
//
//    }
//       if (button == &loadButton)
//    {
//        auto fileChooserFlags =
//        FileBrowserComponent::canSelectFiles;
//        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
//        {
//            player->loadURL(URL{chooser.getResult()});
//            // and now the waveformDisplay as well
//            waveformDisplay.loadURL(URL{chooser.getResult()});
//        });
//    }
//    // if (button == &loadButton)
//    // {
//    //     FileChooser chooser{"Select a file..."};
//    //     if (chooser.browseForFileToOpen())
//    //     {
//    //         player->loadURL(URL{chooser.getResult()});
//    //         waveformDisplay.loadURL(URL{chooser.getResult()});
//
//    //     }
//
//
//    // }
//}
//
//void DeckGUI::sliderValueChanged (Slider *slider)
//{
//    if (slider == &volSlider)
//    {
//        player->setGain(slider->getValue());
//    }
//
//    if (slider == &speedSlider)
//    {
//        player->setSpeed(slider->getValue());
//    }
//
//    if (slider == &posSlider)
//    {
//        player->setPositionRelative(slider->getValue());
//    }
//
//}
//
//bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
//{
//  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
//  return true;
//}
//
//void DeckGUI::filesDropped (const StringArray &files, int x, int y)
//{
//  std::cout << "DeckGUI::filesDropped" << std::endl;
//  if (files.size() == 1)
//  {
//    player->loadURL(URL{File{files[0]}});
//  }
//}
//
//void DeckGUI::timerCheck()
//{
//    //std::cout << "DeckGUI::timerCheck" << std::endl;
//    waveformDisplay.setPositionRelative(
//            player->getPositionRelative());
//}
//
//
//
//
