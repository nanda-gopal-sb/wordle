#include <iostream>
#include <fstream>
#include <random>
#include <chrono> //to implement a pause
#include <thread>

#define red 91    // to print red on the terminal
#define green 92  // same
#define yellow 93 // same

using std::cout, std::cin, std::string; // I dont wanna type out std:: all the time

// the number generator, public cuz I need it at two places
std::random_device rd;
std::mt19937 gen(rd());

class gameFunctions // the singular class in this file
{
public:
    void printColoredChar(char ch, int color);
    void welcomeMessage();
    string getWordForToday();
    int random();
    bool isPresentInsideString(string word, char toCheck);
    void display(string word, string currentWord);
    bool isWordInFile(string toCheck);
};
bool gameFunctions::isWordInFile(string toCheck) // does exactly what it says
{
    // basically checks if toCheck is inside the file, if yes true
    std::ifstream file("fiveLetterWords.dat");
    string line;
    int lineLength = 0;
    int targetLength = toCheck.length();
    while (file >> line)
    {
        lineLength = line.length();
        if (lineLength == targetLength && toCheck == line)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
int gameFunctions::random()
{
    // generates a random number between 1 and 5746(5746 being the number of words in the file)
    std::uniform_int_distribution<> dist(1, 5746);
    return dist(gen);
}
void gameFunctions::display(string guess, string currentWord)
{
    // handles the coloruing of the letters
    for (int i = 0; i < 5; i++)
    {
        if (guess[i] == currentWord[i])
        {
            // if the letter is in the right place
            printColoredChar(guess[i], green);
            continue;
        }
        if (isPresentInsideString(currentWord, guess[i]))
        {
            /// if the letter is in the word but not in the right place
            printColoredChar(guess[i], yellow);
            continue;
        }
        // if the letter is not in the word
        printColoredChar(guess[i], red);
    }
    cout << "\n";
}
void gameFunctions::printColoredChar(char ch, int color)
{
    // this is a terminal specific function, it prints the character in the specified color
    cout << "\033[1;" << color << "m" << ch << "\033[0m";
}
bool gameFunctions::isPresentInsideString(string word, char toCheck)
{
    // obvious
    for (int i = 0; i < 5; i++)
    {
        if (word[i] == toCheck)
        {
            return true;
        }
    }
    return false;
}
void gameFunctions::welcomeMessage()
{
    // the welcome message(duh)
    cout << "Welcome to Wordle! C++ Style" << "\n";
    cout << "Guess the Word in 6 tries." << "\n";
    cout << "Each guess must be a valid 5-letter word." << "\n";
    cout << "The color of the letters will change to show how close your guess was to the word." << "\n";
    printColoredChar('W', green);
    cout << "OODY" << "\n";
    cout << "Here the letter W is in word and in the right place" << "\n";
    cout << "L";
    printColoredChar('I', yellow);
    cout << "GHT" << "\n";
    cout << "Here the letter I is in the word but in the wrong place" << "\n";
    cout << "ROG";
    printColoredChar('U', red);
    cout << "E" << "\n";
    cout << "Here the letter U is not in the word" << "\n";
}
string gameFunctions::getWordForToday()
{
    // gets a random word from the file
    std::ifstream file("fiveLetterWords.dat");
    string word;
    int count = 0;
    int rand = random();
    while (file >> word)
    {
        count++;
        if (count == rand)
        {
            break;
        }
    }
    file.close();
    return word;
}
int main()
{
    gameFunctions *game = new gameFunctions();
    string guessArr[6];

    game->welcomeMessage();
    string currentWord = game->getWordForToday();

    int numberOfTries = 0;
    string guess;
    int commonIndex = 0;

    while (numberOfTries < 7)
    {
        if (numberOfTries == 6)
        {
            // checking for this as I dont want to clear the screen after the last try
            system("clear");
            for (int i = 0; i < 6; i++)
            {
                game->display(guessArr[i], currentWord);
            }
            cout << "Sadly you lose the game..." << "\n";
            cout << "The word was - " << currentWord << "\n";
            break;
        }
        if (numberOfTries > 0)
        {
            system("clear");
        }
        for (int i = 0; i < numberOfTries; i++)
        {
            game->display(guessArr[i], currentWord);
        }
        cout << "Your Guess: ";
        cin >> guess;
        cout << "\n";
        if (guess.length() != 5)
        {
            cout << "The entered word must be of 5 letters" << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(3)); // wating for 3 seconds
            continue;
        }
        if (!game->isWordInFile(guess))
        {
            cout << "Not in the word list try again :(" << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(3)); // same
            continue;
        }
        else
        {
            if (currentWord == guess)
            {
                for (int i = 0; i < 5; i++)
                {
                    game->printColoredChar(guess[i], green);
                }
                cout << " is right" << "\n";
                break;
            }
            else
            {
                guessArr[numberOfTries] = guess;
            }
        }
        numberOfTries++;
    }
    delete game;
}
