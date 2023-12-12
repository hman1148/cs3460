#include "LifeSimulator.hpp"

#include "Pattern.hpp"

#include <vector>

LifeSimulator::LifeSimulator(std::uint8_t sizeX, std::uint8_t sizeY)
{
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    m_simGrid.resize(sizeX, std::vector<bool>(sizeY, false));
}

std::uint8_t LifeSimulator::getSizeX() const
{
    return m_sizeX;
}

std::uint8_t LifeSimulator::getSizeY() const
{
    return m_sizeY;
}

bool LifeSimulator::getCell(std::uint8_t x, std::uint8_t y) const
{
    if (((x >= this->getSizeX()) || (y >= this->getSizeY())))
    {
        return false;
    }
    return m_simGrid[x][y];
}

void LifeSimulator::update()
{
    std::vector<std::vector<bool>> updated(this->getSizeX(), std::vector<bool>(this->getSizeY(), false));

    for (std::uint8_t i = 0; i < m_simGrid.size(); i++)
    {
        for (std::uint8_t j = 0; j < m_simGrid[i].size(); j++)
        {
            std::uint8_t count = this->getNeighbor(i, j);

            if (this->getCell(i, j))
            {
                updated[i][j] = count >= 2 && count <= 3;
            }
            else if (!this->getCell(i, j) && count == 3)
            {
                updated[i][j] = true;
            }
        }
    }
    m_simGrid = updated;
}

void LifeSimulator::insertPattern(const Pattern& pattern, std::uint8_t startX, std::uint8_t startY)
{
    for (std::uint8_t i = 0; i < pattern.getSizeX(); i++)
    {
        for (std::uint8_t j = 0; j < pattern.getSizeY(); j++)
        {
            if ((this->getSizeX() > (startX + i)) && (this->getSizeY() > (startY + j)))
            {
                this->setSquare(((startX + i)), ((startY + j)), pattern.getCell(i, j));
            }
        }
    }
}

void LifeSimulator::setSquare(std::uint8_t x, std::uint8_t y, bool value)
{
    this->m_simGrid[x][y] = value;
}

std::uint8_t LifeSimulator::getNeighbor(std::uint8_t x, std::uint8_t y)
{
    std::uint8_t count = 0;

    if (this->getCell(x + 1, y + 1))
    {
        count++;
    }
    if (this->getCell(x + 1, y - 1))
    {
        count++;
    }
    if (this->getCell(x - 1, y + 1))
    {
        count++;
    }
    if (this->getCell(x - 1, y - 1))
    {
        count++;
    }
    if (this->getCell(x + 1, y))
    {
        count++;
    }
    if (this->getCell(x - 1, y))
    {
        count++;
    }
    if (this->getCell(x, y + 1))
    {
        count++;
    }
    if (this->getCell(x, y - 1))
    {
        count++;
    }

    return count;
}
