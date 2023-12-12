#pragma once
#include <array>
#include <cstdint>
#include <iostream>

const std::size_t HOW_MANY_ELEMENTS = 250000;
const std::uint8_t HOW_MANY_TIMES = 25;
using SourceArray = std::array<int, HOW_MANY_ELEMENTS>;

void initializeRawArrayFromStdArray(const SourceArray& source, int dest[]);

void organPipeStdArray(SourceArray& data);

void evaluateRawArray(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed, const SourceArray& organPipe, const SourceArray& rotated);

void evaluateStdArray(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed, const SourceArray& organPipe, const SourceArray& rotated);

void evaluateStdVector(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed, const SourceArray& organPipe, const SourceArray& rotated);
