#include "sortutils.hpp"

#include <algorithm>
#include <array>
#include <random>

SourceArray generateSorted();

int main()
{
    SourceArray random = generateSorted();
    SourceArray sorted = random;

    SourceArray reverse = random;
    SourceArray rotated = random;
    SourceArray organPipe = random;
    
    std::sort(sorted.begin(), sorted.end());
    std::sort(reverse.begin(), reverse.end());
    std::sort(rotated.begin(), rotated.end());
    std::sort(organPipe.begin(), organPipe.end());

    std::reverse(reverse.begin(), reverse.end());
    std::rotate(rotated.begin(), rotated.begin(), rotated.end());
    organPipeStdArray(organPipe);

    evaluateRawArray(random, sorted, reverse, organPipe, rotated);
    evaluateStdArray(random, sorted, reverse, organPipe, rotated);
    evaluateStdVector(random, sorted, reverse, organPipe, rotated);

    return 0;
}

SourceArray generateSorted()
{
    SourceArray sourceArray = std::array<int, 250000>();

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> uniformDistribution(-10000000, 10000000);

    for (size_t i = 0; i < HOW_MANY_ELEMENTS; i++)
    {
        sourceArray[i] = uniformDistribution(engine);
    }

    return sourceArray;
}
