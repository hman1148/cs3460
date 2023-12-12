#include "WordTree.hpp"
#include "rlutil.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

std::shared_ptr<WordTree> readDictionary(std::string filename);
void drawToScreen(const std::string& input, int yOffset, int xOffset);
std::string getLastWord(const std::string& input);

int main()
{
    std::shared_ptr<WordTree> wordTree = readDictionary("dictionary.txt");

    bool finished = false;

    std::string word;
    std::cout << wordTree->size();
    rlutil::cls();

    std::string sentence;

    while (!finished)
    {
        int key = static_cast<char>(rlutil::getkey());
        char character = static_cast<char>(std::tolower(key));

        rlutil::cls();

        drawToScreen("--- Predictions ---", 4, 1);

        if (key == rlutil::KEY_BACKSPACE && sentence.size() > 0)
        {
            sentence.erase(sentence.size() - 1);
        }
        else
        {
            sentence += character;
        }

        if (isspace(character) || key == rlutil::KEY_SPACE)
        {
            drawToScreen("--- Predictions ---", 4, 1);
        }

        drawToScreen("--- Predictions ---", 4, 1);

        std::string lastWord = getLastWord(sentence);

        std::vector<std::string> predictions = wordTree->predict(lastWord, static_cast<std::uint8_t>(rlutil::trows() - 5));

        for (auto i = 0; i < static_cast<int>(predictions.size()); i++)
        {
            drawToScreen(predictions[i], 5 + i, 1);
        }

        rlutil::locate(1, 1);
        rlutil::setString(sentence);
        rlutil::locate(static_cast<int>(sentence.length()) + 1, 1);

        if (key == rlutil::KEY_ESCAPE)
        {
            finished = true;
        }
    }
}

void drawToScreen(const std::string& input, int yOffset, int xOffset)
{
    rlutil::locate(1, yOffset);
    rlutil::setString(input);
    rlutil::locate(xOffset, yOffset);
}

std::string getLastWord(const std::string& input)
{
    std::istringstream iss(input);
    std::string subs;
    do
    {
        iss >> subs;
    } while (iss);

    return subs;
}

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);

    while (!inFile.eof())
    {
        std::string word;
        std::getline(inFile, word);
        // Need to consume the carriage return character for some systems, if it exists
        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        // Keep only if everything is an alphabetic character -- Have to send isalpha an unsigned char or
        // it will throw exception on negative values; e.g., characters with accent marks.
        if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                        {
                            return std::isalpha(c);
                        }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c)
                           {
                               return static_cast<char>(std::tolower(c));
                           });
            wordTree->add(word);
        }
    }

    return wordTree;
}