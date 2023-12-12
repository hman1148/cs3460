#pragma once
#include "LifeSimulator.hpp"
#include "Renderer.hpp"

class RendererConsole : public Renderer
{
  public:
    void render(const LifeSimulator& simulation) override;
};
