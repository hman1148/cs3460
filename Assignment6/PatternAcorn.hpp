#pragma once
#include "Pattern.hpp"

#include <cstdint>
#include <vector>

class PatternAcorn : public Pattern
{
  public:
    PatternAcorn();
    std::uint8_t getSizeX() const;
    std::uint8_t getSizeY() const;
    bool getCell(std::uint8_t x, std::uint8_t y) const;

  private:
    std::vector<std::vector<bool>> accorn;
};
