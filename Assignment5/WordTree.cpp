#include "WordTree.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

class TreeNode
{
    bool endOfWord;
    std::array<std::shared_ptr<TreeNode>, 26> children;
};

void WordTree::add(std::string word)
{
    std::shared_ptr<TreeNode> currentNode = root;

    if (word.length() == 0)
    {
        return;
    }

    for (int i = 0; i < static_cast<int>(word.length()); i++)
    {
        char character = word[i];

        if (!std::isalpha(character))
        {
            return;
        }
        if (std::isspace(character))
        {
            continue;
        }

        character = static_cast<char>(std::tolower(character));

        int characterIndex = character - 'a';

        if (currentNode->children[characterIndex] == nullptr)
        {
            currentNode->children[characterIndex] = std::make_shared<TreeNode>();
        }
        currentNode = currentNode->children[characterIndex];
    }
    currentNode->endOfWord = true;
}

bool WordTree::find(std::string word)
{
    std::shared_ptr<TreeNode> current = root;
    int totalNumberOfLetters = 26;

    if (word.length() <= 0)
    {
        return false;
    }

    for (int i = 0; i < static_cast<int>(word.length()); i++)
    {
        char letter = static_cast<char>(std::tolower(word[i]));
        if (!std::isalpha(letter))
        {
            return false;
        }

        int characterIndex = letter - 'a';

        if (characterIndex < 0 || characterIndex >= totalNumberOfLetters || !current->children[characterIndex])
        {
            return false;
        }

        current = current->children[characterIndex];
    }
    return current->endOfWord;
}

std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{
    std::shared_ptr<TreeNode> current = root;
    std::vector<std::string> predictions;

    if (partial.length() <= 0)
    {
        return std::vector<std::string>();
    }

    for (int i = 0; i < static_cast<int>(partial.length()); i++)
    {
        char letter = static_cast<char>(std::tolower(partial[i]));

        if (!std::isalpha(letter))
        {
            return predictions;
        }

        int characterIndex = letter - 'a';

        if (current->children[characterIndex] == nullptr)
        {
            return predictions;
        }
        current = current->children[characterIndex];
    }

    std::shared_ptr<TreeNode> traceBack = current;
    std::queue<std::tuple<std::shared_ptr<TreeNode>, std::string>> treeNodeQueue;
    treeNodeQueue.push({ current, partial });

    while (!treeNodeQueue.empty() && predictions.size() < howMany)
    {
        auto [treeNode, word] = treeNodeQueue.front();
        treeNodeQueue.pop();

        if (treeNode->endOfWord && partial != word)
        {
            predictions.push_back(word);
        }

        for (int j = 0; j < static_cast<int>(treeNode->children.size()); j++)
        {
            if (treeNode->children[j])
            {
                char currentLetter = static_cast<char>(j + 'a');
                treeNodeQueue.push({ std::tuple<std::shared_ptr<TreeNode>, std::string>(treeNode->children[j], word + currentLetter) });
            }
        }
    }
    return predictions;
}

std::size_t WordTree::size()
{
    std::size_t totalWordsInTree = 0;
    countWords(root, totalWordsInTree);

    return totalWordsInTree;
}

void WordTree::countWords(std::shared_ptr<TreeNode>& child, std::size_t& count)
{
    std::size_t totalLetters = 26;
    if (child->endOfWord)
    {
        count++;
    }
    for (size_t i = 0; i < totalLetters; i++)
    {
        if (child->children[i])
        {
            countWords(child->children[i], count);
        }
    }
}
