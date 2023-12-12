#include "LifeSimulator.hpp"
#include "PatternAcorn.hpp"
#include "PatternBlinker.hpp"
#include "PatternBlock.hpp"
#include "PatternGlider.hpp"
#include "PatternGosperGliderGun.hpp"
#include "PatternPulsar.hpp"
#include "RendererConsole.hpp"
#include "rlutil.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main()
{
    // Create a life simulator and renderer
    LifeSimulator lifeSim(static_cast<std::uint8_t>(rlutil::trows()), static_cast<std::uint8_t>(rlutil::tcols()));

    PatternGlider glider;
    PatternPulsar pulsar;
    PatternGosperGliderGun gun;
    PatternBlinker blinker;
    PatternAcorn accorn;

    RendererConsole render;

    lifeSim.insertPattern(glider, 3, 5);
    lifeSim.insertPattern(blinker, 5, 5);
    lifeSim.insertPattern(accorn, 13, 15);
    lifeSim.insertPattern(glider, 0, 0);

    rlutil::hidecursor();

    for (int i = 0; i < 1000; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        render.render(lifeSim);
        lifeSim.update();
    }

    rlutil::cls();

    return 0;
}