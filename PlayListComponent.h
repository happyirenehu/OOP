/*
 ==============================================================================
 
 //    PlaylistComponent.h
 //    Created: 7 Mar 2023 7:43:38pm
 //    Author:  Irene's macbook
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

#include "DeckGUI.h"
#include "DJAudioPlayer.h"



//==============================================================================
/*
 */
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(/*DeckGUI* _deck1, DeckGUI* _deck2*/);  //was planned to load the deckGUI and directly load the URL to the player, but it crashed somehow.
    ~PlaylistComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    
    void paintRowBackground(juce::Graphics &,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;
    
    void paintCell(juce::Graphics &,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;
    
    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component* existingComponentToUpdate) override;
    void buttonClicked(juce::Button* button) override;
    void textEditorTextChanged(juce::TextEditor& editor) override; //loop through  juce::Array<juce::String> fileNames to search for the matching song name.
    void deleteFile(); //Deleting files from the playlist when the row number clicked
  
    //void createFile();
    //void readFile(juce::File testFile);
    
    juce::String fileToDeckGUI;
    std::string stdstring;
    void loadInLibrary(); // load the file into library
    void addFileToHistory(); // add file to another folder for future use
    void loadToDeck1(); //Loads library files to the deckGUI
    void loadToDeck2(); //Loads library files to the deckGUI
    void prepFile(); // prepare the files to take in music library data
    
    
    
    
private:
    //DeckGUI* deck1;
    //DeckGUI* deck2;
    //juce::AudioFormatManager formatManager;
    AudioFormatManager formatManager;
    std::vector <std::string > existingFiles;
    double songLength;
    juce::Array<double> libraryFilesDuration;
    juce::TableListBox tableComponent;
    bool historyClicked = false;
    double clickedRowIndex;

    int alreadyExistIndex = -1;
    
    juce::TextButton loadToLibrary{"Load to library...."};
    juce::TextButton deleteLibraryButton;
    juce::TextButton libraryDeck1;
    juce::TextButton libraryDeck2;
    juce::TextEditor searchLibrary{ "Search songs" }; //search matching filename in library
    juce::String searchKey;
    juce::String fileName;
    
    juce::Array<juce::String> fileNames;
    juce::Array<juce::File> libraryFiles;
    
    juce::URL URL;
    std::vector <std::string> importedURL;
    
    //juce::File testFile;
    int rowClicked = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
