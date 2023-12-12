// only look at changed positions in the renderer
#include "RendererConsole.hpp"

#include "LifeSimulator.hpp"
#include "rlutil.h"

void RendererConsole::render(const LifeSimulator& lifeSimulator)
{
    rlutil::cls();

    for (std::uint8_t i = 0; i < lifeSimulator.getSizeX(); i++)
    {
        for (std::uint8_t j = 0; j < lifeSimulator.getSizeY(); j++)
        {
            if (lifeSimulator.getCell(i, j))
            {
                rlutil::locate(i, j);
                rlutil::setChar('X');
            }
        }
    }
}