#pragma once
#include "Pattern.hpp"

#include <vector>

class PatternGlider : public Pattern
{
  public:
    PatternGlider();
    std::uint8_t getSizeX() const;
    std::uint8_t getSizeY() const;
    bool getCell(std::uint8_t x, std::uint8_t y) const;

  private:
    std::vector<std::vector<bool>> glider;
};