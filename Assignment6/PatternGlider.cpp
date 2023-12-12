#include "PatternGlider.hpp"

#include <vector>

PatternGlider::PatternGlider()
{
    glider = { { false, false, true },
               { true, false, true },
               { false, true, true } };
}

std::uint8_t PatternGlider::getSizeX() const
{
    return static_cast<std::uint8_t>(3);
}

std::uint8_t PatternGlider::getSizeY() const
{
    return static_cast<std::uint8_t>(3);
}

bool PatternGlider::getCell(std::uint8_t x, std::uint8_t y) const
{
    return this->glider[y][x];
}