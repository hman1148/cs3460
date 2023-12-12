#include "PatternBlock.hpp"

#include <vector>

PatternBlock::PatternBlock()
{
    block = { {
                  true,
                  true,
              },
              { true, true } };
};

std::uint8_t PatternBlock::getSizeX() const
{
    return static_cast<std::uint8_t>(2);
}

std::uint8_t PatternBlock::getSizeY() const
{
    return static_cast<std::uint8_t>(2);
}

bool PatternBlock::getCell(std::uint8_t x, std::uint8_t y) const
{
    return this->block[x][y];
}