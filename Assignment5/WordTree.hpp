#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class WordTree
{
  public:
    void add(std::string word);
    bool find(std::string word);
    std::vector<std::string> predict(std::string partial, std::uint8_t howMany);
    std::size_t size();

  private:
    class TreeNode
    {
      public:
        bool endOfWord;
        std::array<std::shared_ptr<WordTree::TreeNode>, 26> children;
    };
    std::shared_ptr<TreeNode> root = std::make_shared<TreeNode>();
    void countWords(std::shared_ptr<TreeNode>& child, std::size_t& count);
};
