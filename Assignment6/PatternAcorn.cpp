#include "PatternAcorn.hpp"

#include <vector>

PatternAcorn::PatternAcorn()
{
    accorn = { { false, true, false, false, false, false, false },
               { false, false, false, true, false, false, false },
               { true, true, false, false, true, true, true } };
}

std::uint8_t PatternAcorn::getSizeX() const
{
    return static_cast<std::uint8_t>(7);
}

std::uint8_t PatternAcorn::getSizeY() const
{
    return static_cast<std::uint8_t>(3);
}

bool PatternAcorn::getCell(std::uint8_t x, std::uint8_t y) const
{
    return this->accorn[y][x];
}
