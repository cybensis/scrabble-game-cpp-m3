
#include "LinkedList.h"

#include <iostream>
#include "GameEngine.h"
#include <string>
#include <fstream>
#include <vector>

using std::string;
using std::fstream;
string userInput();
using std::cin;
using std::cout;
using std::endl;


#define EXIT_SUCCESS    0

void welcomeMessage();
void mainMenu(bool enableColour);
void printMainMenu();
void newGame(bool enableColour);
void loadGame(bool enableColour);
void credits();
std::fstream& GotoLine(std::fstream& file, unsigned int num);
bool validation(std::fstream myFile);
string userInput();
void printHelp();

int main(int argc, char *argv[]) {
    // arg[0] is the filename
    bool argErrors = true;
    bool enableColour = false;
    std::string colourToggle = "--colour";
    if (argc <= 1) { argErrors = false; }
    // if there are 2 args, then check if the second (argv[1]) is equal to "--colour", as there is only one toggleable enhancement
    else if (argc == 2 && colourToggle.compare(argv[1]) == 0) { argErrors = false; enableColour = true;}

    if (argErrors) {
        std::cout << "One of the arguments you supplied are invalid" << std::endl;
        std::cout << "Available arguments: --colour" << std::endl << std::endl;
    }
    else {
        welcomeMessage();
        mainMenu(enableColour);
    }
    return EXIT_SUCCESS;
}

void welcomeMessage() {
    cout << "Welcome to Scrabble!" << endl;
    cout << "-------------------"<< endl;
}

void mainMenu(bool enableColour) {
    printMainMenu();

    bool toRePrompt;
    string choice;
    do {
        toRePrompt = false;
        choice = userInput();
        if (choice == "help") { printHelp(); }
        else if (choice == "1") { newGame(enableColour); } 
        else if (choice == "2") { loadGame(enableColour); } 
        else if (choice == "3") {
            credits();
            // Reprompt user after showing credits
            toRePrompt = true;
            printMainMenu();
        } 
        else if (choice == "4") { cout << "\nGoodbye" << endl;} 
        else { cout << "Invalid Input" << endl; toRePrompt = true; }
    } while (toRePrompt);
}

void printMainMenu() {
    cout << endl << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl << endl;
}

void newGame(bool enableColour) {
    cout << endl << "Starting a New Game" << endl << endl;
    GameEngine* gameInstance = new GameEngine(enableColour);
    gameInstance->gameController();
    std::cout << std::endl << "Goodbye" << std::endl;
    delete gameInstance;
    return;
}

void loadGame(bool enableColour) {
    cout << endl << "Enter the filename from which load a game" << endl;
    string dir = userInput();
    fstream myFile;
    myFile.open(dir, std::ios::in);
    if (!myFile) { std::cout << "File not found" << std::endl; }
    else {
        GameEngine* gameInstance = new GameEngine(&myFile, enableColour);
        gameInstance->gameController();
        delete gameInstance;
    }
    std::cout << std::endl << "Goodbye" << std::endl;
    return;
}

void credits() {
    std::ifstream f("authors.txt");
    cout << endl << "----------------------------------" << endl;
    if (f.is_open())
        cout << f.rdbuf();
    cout << endl << "----------------------------------" << endl;
    /*
    This does to need to recursively call mainMenu().
    Causes memory leak when viewing credits then quitting.
    Instead, simply reprompt user for input after showing credits
    */
    // mainMenu(); 
}

//ref = https://stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file
std::fstream& GotoLine(std::fstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(unsigned int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}


string userInput() {
    string input;
    cout << "> ";
    getline(cin, input);
    if (cin.eof() || input == "^D") {
        cout << endl << endl << "Goodbye" << endl;
        exit(EXIT_SUCCESS);
    }
    return input;
}


void printHelp() {
    std::cout << "Enter \"1\" to start a new game" << std::endl;
    std::cout << "Enter \"2\" to load a game" << std::endl;
    std::cout << "Enter \"3\" to show credits" << std::endl;
    std::cout << "Enter \"4\" to quit the game" << std::endl;
    std::cout << std::endl;
    return;
}