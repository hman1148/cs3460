#include "PatternBlinker.hpp"

#include <vector>

PatternBlinker::PatternBlinker()
{
    blinker = { { true }, { true }, { true } };
}

std::uint8_t PatternBlinker::getSizeX() const
{
    return static_cast<std::uint8_t>(1);
}

std::uint8_t PatternBlinker::getSizeY() const
{
    return static_cast<std::uint8_t>(3);
}

bool PatternBlinker::getCell(std::uint8_t x, std::uint8_t y) const
{
    return this->blinker[y][x];
}