#include "PatternPulsar.hpp"

#include <vector>

PatternPulsar::PatternPulsar()
{
    this->patternPulsar = {
        { false, false, true, true, true, false, false, false, true, true, true, false, false },

        { false, false, false, false, false, false, false, false, false, false, false, false, false },

        { true, false, false, false, false, true, false, true, false, false, false, false, true },

        { true, false, false, false, false, true, false, true, false, false, false, false, true },

        { true, false, false, false, false, true, false, true, false, false, false, false, true },

        { false, false, true, true, true, false, false, false, true, true, true, false, false },

        { false, false, false, false, false, false, false, false, false, false, false, false, false },

        { false, false, true, true, true, false, false, false, true, true, true, false, false },

        { true, false, false, false, false, true, false, true, false, false, false, false, true },

        { true, false, false, false, false, true, false, true, false, false, false, false, true },

        { true, false, false, false, false, true, false, true, false, false, false, false, true },

        { false, false, false, false, false, false, false, false, false, false, false, false, false },

        { false, false, true, true, true, false, false, false, true, true, true, false, false },
    };
}

std::uint8_t PatternPulsar::getSizeX() const
{
    return static_cast<std::uint8_t>(13);
}

std::uint8_t PatternPulsar::getSizeY() const
{
    return static_cast<std::uint8_t>(13);
}

bool PatternPulsar::getCell(std::uint8_t x, std::uint8_t y) const
{
    return this->patternPulsar[y][x];
}
