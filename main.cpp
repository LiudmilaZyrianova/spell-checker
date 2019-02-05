//Liudmila Zyrianova
//CSCI 335
//11/18/2018
//Programming Assignment 3

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <set>
#include <chrono>
#include <ctype.h>
#include <string>

using namespace std;

//The type of set can be changed in the following line:
typedef unordered_set<std::string> T;           //For the final version of the program I've chosen unordered_set instead of set.


void FileToSet (T& s, fstream & file) {         //Fills set(s) with the words from the dictionary(file)
    string word;
    while (file >> word)
    {
        s.insert(word);
    }

}

void RemovePunctuation(string & word)  {          //Remove comma, periods, apostrophe
    while (!isalpha(word[word.length()-1])) {      //Remove comma, periods
        word.pop_back();
    }



    if (word[word.length()-2] == '\''){       //ends on " ...'s "
        word.pop_back();
        word.pop_back();
    }
}

void ToLowerCase (string & word) {          //Switch all letters to the lower case
    int i = 0;
    while (i < word.length())
    {
        char& c = word[i];
        c = tolower(c);     //The value is returned as an int value that can be implicitly casted to char.
        i++;
    }
}

int DashInMiddle (const string & word, const string & initialWord, ofstream & outputFile){         //Returns the position of dash in the word, or returns -1 if there are at least 2 dashes, or returns the length of the word if there is no dash
    int dashNum = 0;
    int dashPos = 0;
    for (int i = 0; i<word.length(); i++){
        if (word[i]=='-'){
            dashNum++;
            dashPos = i;
        }
    }
    if (dashNum == 0) return word.length();
    if (dashNum == 1) return dashPos;
    if (dashNum > 1) {
        outputFile << initialWord<< ':'<<" Too many dashes in the word\n";
        return -1;
    }
}

bool WordHasBeenFound (const T& s, const string & word){        //Tries to find the word
    if (s.find( word )== s.end()) return false;     //Word has not been found
    else return true;       //Word has been found
}

void PrintSuggestionIfFound (const T& s, const string & temp, bool & suggestions, ofstream & outputFile){   //Print suggestion to the file if it has been found in the dictionary
    if (s.find( temp ) != s.end()){     //Suggestion has been found
        suggestions = true;
        outputFile << temp << ' ';
    }
}

void MissingLetter (const T& s, const string & word, bool & suggestions, ofstream & outputFile){    //Type of mistake: single letter is missing
    string temp = word;
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (auto c : alphabet){        //Insert every letter of alphabet into every position in the word
        for (int i = 0; i<=word.length(); i++){
            string::iterator it = next(temp.begin(),i);
            temp.insert (it, c);
            PrintSuggestionIfFound(s, temp, suggestions, outputFile);   //Print suggestion to the file if it has been found in the dictionary
            temp = word;
        }
    }
}

void ExtraLetter (const T& s, const string & word, bool & suggestions, ofstream & outputFile) {     //Type of mistake: single extra letter appears
    string temp = word;
    for (int i = 0; i<word.length(); i++) {     //Going to erase every letter
        string::iterator it = next(temp.begin(),i);
        temp.erase(it);
        PrintSuggestionIfFound(s, temp, suggestions, outputFile);   //Print suggestion to the file if it has been found in the dictionary
        temp = word;
    }
}

void ReplacedLetter (const T& s, const string & word, bool & suggestions, ofstream & outputFile){   //Type of mistake: single letter is replaced by other letter
    string temp = word;
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (auto c : alphabet){        //Substitute by every letter of alphabet every letters in the word
        for (int i = 0; i < word.length(); i++){
            temp[i] = c;
            PrintSuggestionIfFound(s, temp, suggestions, outputFile);   //Print suggestion to the file if it has been found in the dictionary
            temp = word;
        }
    }
}

void SwappedLetter (const T& s, const string & word, bool & suggestions, ofstream & outputFile){    //Type of mistake: 2 neighboring letters have been swapped
    string temp = word;
    for (int i = 0; i < word.length()-1; i++) {
        char c = temp[i];
        temp[i] = temp[i+1];
        temp[i+1] = c;
        PrintSuggestionIfFound(s, temp, suggestions, outputFile);   //Print suggestion to the file if it has been found in the dictionary
        temp = word;
    }
}

void MakeSuggestions (const T& s, const string & word, const string & initialWord, ofstream & outputFile){      //Make suggestions for the word
    if (!WordHasBeenFound(s, word)) {       //There is no word in the dictionary
        outputFile << initialWord << ':' << ' ';        //We will make some suggestions
        bool suggestions = false;
        MissingLetter(s, word, suggestions, outputFile); //Type of mistake: single letter is missing
        ExtraLetter(s, word, suggestions, outputFile);  //Type of mistake: single extra letter appears
        ReplacedLetter(s, word, suggestions, outputFile);   //Type of mistake: single letter is replaced by other letter
        SwappedLetter(s, word, suggestions, outputFile);    //Type of mistake: 2 neighboring letters have been swapped
        if (!suggestions) {     //Suggestions haven't been found
            outputFile << "no suggestions";
        }
        outputFile << '\n';
    }
}

void SpellCheck (const T& s, fstream & file, ofstream & outputFile){        //Checks the words from the test file
    string initialWord;
    while (file >> initialWord)         //For every word in the test file
    {
        if ( (isalpha(initialWord[0])) || (initialWord.length()>=2)){       //Word is not a single punctuation symbol
            RemovePunctuation(initialWord);     //Remove punctuation from the end of the word
            string word = initialWord;          //Need to create a version of the word without capital letters
            ToLowerCase(word);                  //Switch all letters to the lower case
            int dashPos = DashInMiddle(word, initialWord, outputFile);      //Return the position of dash in the word, or return -1 if there are at least 2 dashes, or return the length of the word if there is no dash
            if (dashPos == -1 ){            //Word has at least 2 dashes
            } else if (dashPos < word.length()){        //Word has one dash
                string word1 = word.substr (0,dashPos);         //First part of the word before dash
                string word2 = word.substr (dashPos + 1 ,word.length()-dashPos-1);  //Second part of the word after dash
                string initialWord1 = initialWord.substr (0,dashPos);   //First part of the word before dash
                string initialWord2 = initialWord.substr (dashPos + 1 ,word.length()-dashPos-1);    //Second part of the word after dash
                MakeSuggestions(s, word1, initialWord1, outputFile);        //Make suggestions for the first part
                MakeSuggestions(s, word2, initialWord2, outputFile);        //Make suggestions for the second part
            } else {
                MakeSuggestions(s, word, initialWord, outputFile);          //Make suggestions for the word
            }
        }
    }
}

int main(int argc, char** argv) {
    T s;        //Set for the dictionary

    std::chrono::steady_clock::time_point t1;       //Point to measure start of the program
    std::chrono::steady_clock::time_point t2;       //Point to measure finish of the program

    t1 = std::chrono::steady_clock::now();          //Program starts

    fstream file;
    string filename = argv[2];                      //filename = "./dictionary.txt";
    file.open (filename);
    FileToSet(s, file);                             //Fills set(s) with the words from the dictionary(file)
    file.close();

    ofstream outputFile;
    outputFile.open("Suggestions.txt");             //Create test file with suggestions

    filename = argv[1];                             //filename = "./check_it.txt";
    file.open (filename);
    SpellCheck(s, file, outputFile);                //Checks the words from the file
    file.close();

    outputFile.close();

    t2 = std::chrono::steady_clock::now();          //Program ends

    std::cout<<"Time passed in ms is "<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<'\n';        //Prints the running time of the program

    return 0;
}
