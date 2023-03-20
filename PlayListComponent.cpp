/*
 ==============================================================================
 
 PlaylistComponent.cpp
 Created: 7 Mar 2023 7:43:38pm
 Author:  Irene's macbook
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include <string>
#include "PlayListComponent.h"
#include <time.h>

//==============================================================================

PlaylistComponent::PlaylistComponent(/*DeckGUI* _deckGUI1, DeckGUI* _deckGUI2)
                    : deckGUI1(_deckGUI1), deckGUI2(_deckGUI2*/)
{
    
    //creates columns for the playlist component
    tableComponent.getHeader().addColumn("Song", 1, 300);
    tableComponent.getHeader().addColumn("Length", 2, 100);
    tableComponent.getHeader().addColumn("Load into Deck 1", 3, 150);
    tableComponent.getHeader().addColumn("Load into Deck 2", 4, 150);
    tableComponent.getHeader().addColumn("Delete", 5, 100);
    //adding and making visible for the playlist table component, buttons and search input. Also adding listeners for buttons and search input
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadToLibrary);
    addAndMakeVisible(searchLibrary);
    loadToLibrary.addListener(this);
    
    formatManager.registerBasicFormats();
    
    searchLibrary.addListener(this);
    
    loadToLibrary.setClickingTogglesState(true);
    loadToLibrary.setColour(TextButton::ColourIds::buttonOnColourId, Colours::deeppink);
    loadToLibrary.setColour(TextButton::ColourIds::buttonColourId, Colours::snow);
    loadToLibrary.setColour(TextButton::ColourIds::textColourOffId, Colours::black);
    
    libraryDeck1.setColour(TextButton::ColourIds::buttonOnColourId, Colours::deeppink);
    libraryDeck1.setColour(TextButton::ColourIds::buttonColourId, Colours::snow);
    libraryDeck1.setColour(TextButton::ColourIds::textColourOffId, Colours::black);
    
    prepFile();
    
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
     draws some placeholder text to get you started.
     
     You should replace everything in this method with your own
     drawing code..
     */
    
    g.fillAll (juce::Colours::deeppink);   // clear the background
    
    g.setColour (juce::Colours::purple);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 7;
    searchLibrary.setBounds(0, 0, getWidth() / 2, rowH);
    loadToLibrary.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    tableComponent.setBounds(0, rowH, getWidth(), getHeight());
    
    //playlist header columns
    tableComponent.getHeader().setColumnWidth(1, (getWidth() / 8) * 3);
    tableComponent.getHeader().setColumnWidth(2, getWidth() / 8);
    tableComponent.getHeader().setColumnWidth(3, (getWidth() / 8) * 1.5);
    tableComponent.getHeader().setColumnWidth(4, (getWidth() / 8) * 1.5);
    tableComponent.getHeader().setColumnWidth(5, (getWidth() / 8));
    
    tableComponent.setColour(juce::ListBox::backgroundColourId, juce::Colours::rosybrown);
    
    //search bar
    searchLibrary.setTextToShowWhenEmpty("Search song...", juce::Colours::snow);
    searchLibrary.setFont(20.0f);
    searchLibrary.setColour(TextEditor::ColourIds::backgroundColourId, Colours::deeppink);

    
    
}

int PlaylistComponent::getNumRows() { //get number of rows in playlist by getting the size of the file names
    return fileNames.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::lavender);
    } else {
        g.fillAll(juce::Colours::floralwhite);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected) {
    
    if (columnId == 1) {//column 1 shows the file name of the audio file
        g.drawText(fileNames[rowNumber],
                   2, 0,
                   width - 4, height,
                   juce::Justification::centredLeft,
                   true);
    }
    if (columnId == 2) {// column 2 shows the file's duration, in the format of hh:mm:ss
        std::time_t seconds(libraryFilesDuration[rowNumber]);
        tm* p = gmtime(&seconds);
        std::string hhmmss;
        std::string hours;
        std::string mins;
        std::string secs;
        //converting file duration from double into seconds and then into hours, minutes and seconds
        if (p->tm_hour < 10) {
            hours = "0" + std::to_string(p->tm_hour);
        }
        else {
            hours = std::to_string(p->tm_hour);
        }
        if (p->tm_min < 10) {
            mins = "0" + std::to_string(p->tm_min);
        }
        else {
            mins = std::to_string(p->tm_min);
        }
        if (p->tm_sec < 10) {
            secs = "0" + std::to_string(p->tm_sec);
        }
        else {
            secs = std::to_string(p->tm_sec);
        }
        
        hhmmss = hours + ":" + mins + ":" + secs;
        g.drawText(hhmmss,
                   2, 0,
                   width - 4, height,
                   juce::Justification::centredLeft,
                   true);
    }
    
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                            int columnId,
                                                            bool isRowSelected,
                                                            Component* existingComponentToUpdate) {
    if (columnId == 3) {//column 3 is a button that loads the audio file into deck 1
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Deck 1" }; //creates button with deck 1 as the text
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id); //set component id to button for identifying which button was clicked
            btn->addListener(this); //adding listener to the button
            btn->setColour(TextButton::ColourIds::buttonColourId, Colours::pink);
            btn->setColour(TextButton::ColourIds::textColourOffId, Colours::darkgrey);
            existingComponentToUpdate = btn;
            rowNumber = rowClicked;
            btn->onClick = [this] {loadToDeck1(); }; //loadToDeck1() runs when button is clicked
        }
    }
    if (columnId == 4) {//column 4 is a button that loads the audio file into deck 2
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Deck 2" }; //creates button with deck 2 as the text
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            btn->setColour(TextButton::ColourIds::buttonColourId, Colours::pink);
            btn->setColour(TextButton::ColourIds::textColourOffId, Colours::darkgrey);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {loadToDeck2(); }; //loadToDeck2() runs when button is clicked
        }
    }
    if (columnId == 5) {//column 5 is a button that deletes the audio file
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Delete" }; //creates button with delete as the text
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            btn->setColour(TextButton::ColourIds::buttonColourId, Colours::pink);
            btn->setColour(TextButton::ColourIds::textColourOffId, Colours::darkgrey);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {deleteFile(); };//deleteFile() runs when button is clicked
            
            
        }
    }
    
    
    return existingComponentToUpdate;
    
}
void PlaylistComponent::prepFile(){
  
    std::ofstream storageFiles("playLibrary.txt");
    storageFiles << stdstring << std::endl << "0" << std::endl;
    storageFiles.close();
    
    
    std::ifstream file("PlaylistHistory.txt");
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    
    //If text file contains audio file URLs, add each line into existing files array
    for (int i = 0; i < lines.size(); ++i) {
        existingFiles.push_back(lines[i]);
    }
    
    //If there are existing files before
    if (existingFiles.size() != 0) {
        
        //adds existing files into my files
        for (int i = 0; i < existingFiles.size(); ++i) {
            libraryFiles.add(juce::File{ existingFiles[i] });
            
        }
        
        for (int i = 0; i < libraryFiles.size(); ++i) { //for every file, get the file name and duration and prepares them for reading
            
            fileName = juce::URL::removeEscapeChars(juce::URL{ libraryFiles[i] }.getFileName()); //gets file name
            fileNames.add(fileName); //adds file name into filenames array, to be displayed in playlist table component
            juce::AudioFormatReader* reader = formatManager.createReaderFor(libraryFiles[i]); //creates reader for each file
            songLength = reader->lengthInSamples / reader->sampleRate; //gets duration of the file by dividing length by sample rate
            libraryFilesDuration.add(songLength); //adds duration into duration array to be displayed in playlist table component
            
        }
        tableComponent.updateContent(); //updates the table component to display the file names and duration
    }
}

//void PlaylistComponent::createFile(){
//    std::unique_ptr<juce::TextEditor> textContent;
//    File testFile = File::getCurrentWorkingDirectory().getChildFile("file.txt");
////    File testFile("User/irenesmacbook/Downloads/OtoDecks_start_topic_10-2");
//    if(!testFile.existsAsFile()){
//        DBG("File doesn't exist...");
//    }else{
//        DBG("File exist");
//    }
//
//    //write in file
//    TemporaryFile tempFile(testFile);
//    //FileOutputStream output(tempFile.getFile());
//
//
//    if (auto output = std::unique_ptr<juce::FileOutputStream>(testFile.createOutputStream()))
//    {
//        if(!output->openedOk()){
//            DBG ("FileOutputStream didn't open correctly ...");
//        }else {
//            DBG("FileOutputStream open correctly");
//        }
//        output->setPosition(0);
//        output->truncate();
//        output->writeText("Text 1", false, false, nullptr);
//        output->setNewLineString("\n");
//        output->writeText("Text 2", false, false, nullptr);
//        output->setNewLineString("\n");
//        output->writeText("Text 3", false, false, nullptr);
//        output->setNewLineString("\n");
//    }
//
//
//    std::unique_ptr<FileInputStream> input (testFile.createInputStream());
//
//    if (! input->openedOk())
//    {
//        DBG("Failed to open file");
//        // ... Error handling here
//    }
//
//    bool readWholeFile = true;
//    if(readWholeFile)
//    {
//        String content = input->readString();
//        // ... Do your stuff. New lines are normally seperated by "\n"
//    }
//    else
//    {
//        while (! input->isExhausted())
//        {
//            String singleLine = input->readNextLine();
//            // ... Do something with each line
//        }
//    }
//
//}
//
//void PlaylistComponent::readFile(juce::File testFile){
//    std::unique_ptr<FileInputStream> input (testFile.createInputStream());
//
//    if (! input->openedOk())
//    {
//        DBG("Failed to open file");
//        // ... Error handling here
//    }
//
//    bool readWholeFile = true;
//    if(readWholeFile)
//    {
//        String content = input->readString();
//        // ... Do your stuff. New lines are normally seperated by "\n"
//    }
//    else
//    {
//        while (! input->isExhausted())
//        {
//            String singleLine = input->readNextLine();
//            // ... Do something with each line
//        }
//    }
//}

void PlaylistComponent::loadInLibrary(){
    juce::FileChooser choosers{ "Select for file(s) to add to the library...", juce::File() , "*.mp3;*.wav;*.aiff" };
    
    if (choosers.browseForMultipleFilesToOpen()) {
        libraryFiles = choosers.getResults(); //libraryFiles is an array of files
        for (int i = 0; i < libraryFiles.size(); ++i) { //for loop repeats for libraryFiles size
            
            if (existingFiles.size() != 0 || i == 0) { //if there are existing files, or it is the first iteration of the loop
                for (int j = 0; j < existingFiles.size(); ++j) { //loops through number of times based on how many existing files there is
                    if (libraryFiles[i].getFullPathName().toStdString() == existingFiles[j]) { //loops through the existing files, if any matches, do not add the index
                        DBG("duplicates FOUND");
                        
                        alreadyExistIndex = i; //taking note of which file index is a duplicated
                        break;
                    }
                }
                if (i != alreadyExistIndex) { //If the file is not a duplicated file index, add the file
                    fileName = juce::URL::removeEscapeChars(juce::URL{libraryFiles[i] }.getFileName()); //get the file name and adds them to the fileNames array for display
                    fileNames.add(fileName);
                    juce::AudioFormatReader* reader = formatManager.createReaderFor(libraryFiles[i]);
                    songLength = reader->lengthInSamples / reader->sampleRate; //gets duration of the file by dividing the length in samples by the sample rate
                    libraryFilesDuration.add(songLength); //adds duration into libraryFilesDuration, which is used in paint cell
                    
                    //juce::URL URL.push_back(i);
                    //DBG(URL[i]);
                    DBG(fileNames[i]);
                    DBG(songLength);
                    
                    existingFiles.push_back(libraryFiles[i].getFullPathName().toStdString()); //file path is added to the existingFiles array
                    //DBG(libraryFiles[1]);
                    importedURL.push_back((libraryFiles[i].getFullPathName().toStdString()));
                    DBG("libraryFiles");
                    DBG(libraryFiles[i].getFullPathName().toStdString());
                    DBG(importedURL[i]);
            
                }
                
            }
            
        }
        
        addFileToHistory();
        
        
        tableComponent.updateContent();
    }
}
void PlaylistComponent::addFileToHistory(){
    std::ofstream history("PlaylistHistory.txt");
//    for (const juce::File& file : existingFiles)
    
    for (int i = 0; i < existingFiles.size(); ++i) {
        history << existingFiles[i] << std::endl;
    }
    history.close();
}
void PlaylistComponent::buttonClicked(juce::Button* button) {
    
    if (button == &loadToLibrary) {
        loadInLibrary();
        clickedRowIndex = button->getComponentID().getDoubleValue();
        DBG(clickedRowIndex);
    }
    
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& editor) {
    
    searchKey = searchLibrary.getText();
    
    for (int i = 0; i < fileNames.size(); ++i) {
        if (searchKey != "" && fileNames[i].containsWholeWord(searchKey)) { 
            tableComponent.selectRow(i, false, true);
        }
    }
    
}

void PlaylistComponent::deleteFile() {//only runs when user clicks on the delete button on a file
    
    DBG(clickedRowIndex);
    
    for (int i = 0; i < libraryFiles.size(); ++i) {//loops through libraryFiles
        if (clickedRowIndex == i) { //matches row(file) which the user clicked on
   
        bool result = true;
        if (result == true) {//If true, deletes file
            DBG("deletefile");
            fileNames.remove(i); //Removes file name from fileNames array
            libraryFilesDuration.remove(i); //Removes file duration from libraryFilesDuration array
            existingFiles.erase(existingFiles.begin() + i); //Removes the file from the existingFiles array
            }
     
        }
        }
        //Opens the PlaylistHistory text file, to remove the deleted file and update the current existing files
        std::ofstream history("PlaylistHistory.txt");
        for (int i = 0; i < existingFiles.size(); ++i) {
            history << existingFiles[i] << std::endl;
        }
        history.close();
    
        tableComponent.updateContent();
}

void PlaylistComponent::loadToDeck1() {
    
    DBG(clickedRowIndex);
    for (int i = 0; i < libraryFiles.size(); ++i) { //Loops through libraryFiles
        juce::AudioFormatReader* reader = formatManager.createReaderFor(libraryFiles[i]); //creates reader for the file
        //DBG(clickedRowIndex);
        if (clickedRowIndex == i) { //Matches the row(file) which the user clicked to be added into Deck 1
            DBG("loadtodeck1");
            DBG(libraryFiles[i].getFileName());
            DBG(libraryFiles[i].getFullPathName().toStdString());
//            DBG(clickedRowIndex);
            //DBG(importedURL[i]);
//          deckGUI1->loadFile([URL[i]);
        }
    }
    //DBG(libraryFiles[rowNum].getFileName());
    tableComponent.updateContent();
}
    
void PlaylistComponent::loadToDeck2(){
    DBG(clickedRowIndex);
    for (int i = 0; i < libraryFiles.size(); ++i) { //Loops through libraryFiles
        juce::AudioFormatReader* reader = formatManager.createReaderFor(libraryFiles[i]); //creates reader for the file
        //DBG(clickedRowIndex);
        if (clickedRowIndex == i) { //Matches the row(file) which the user clicked to be added into Deck 1
            DBG("loadtodeck1");
            DBG(libraryFiles[i].getFileName());
            DBG(libraryFiles[i].getFullPathName().toStdString());
            //            DBG(clickedRowIndex);
            //DBG(importedURL[i]);
            //          deckGUI1->loadFile([URL[i]);
        }
    }
    //DBG(libraryFiles[rowNum].getFileName());
    tableComponent.updateContent();
}

