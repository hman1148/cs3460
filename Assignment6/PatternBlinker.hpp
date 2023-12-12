#pragma once

#include "Pattern.hpp"

#include <cstdint>
#include <vector>

class PatternBlinker : public Pattern
{
  public:
    PatternBlinker();
    std::uint8_t getSizeX() const;
    std::uint8_t getSizeY() const;
    bool getCell(std::uint8_t x, std::uint8_t y) const;

  private:
    std::vector<std::vector<bool>> blinker;
};