#include "sortutils.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <execution>
#include <format>
#include <iostream>
#include <random>

void evaluateArrayRawSeq(const SourceArray& source, std::string name);
void evaluateArrayRawPar(const SourceArray& source, std::string name);
void evaluateStdAndVectorSeq(const SourceArray& source, std::string name, bool isVector);
void evaluateStdAnVectorPar(const SourceArray& source, std::string name, bool isVector);

void initializeRawArrayFromStdArray(const SourceArray& source, int dest[])
{
    std::copy(source.begin(), source.end(), dest);
}

void organPipeStdArray(SourceArray& data)
{
    int totalSize = static_cast<int>(data.size());
    int center = static_cast<int>(totalSize / 2);

    for (int i = 0; i < center; i++)
    {
        data[totalSize - 1 - i] = data[i];
    }
}

void evaluateRawArray(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed, const SourceArray& organPipe, const SourceArray& rotated)
{
    std::cout << " --- Raw Array Performance ---" << std::endl;
    std::cout << "\n";
    // Sequential
    std::cout << "Sequential" << std::endl;
    evaluateArrayRawSeq(random, "Random");
    evaluateArrayRawSeq(sorted, "Sorted");
    evaluateArrayRawSeq(reversed, "Reversed");
    evaluateArrayRawSeq(organPipe, "Organ Pipe");
    evaluateArrayRawSeq(rotated, "Rotated");
    std::cout << "\n";

    // Parallel
    std::cout << "Parallel" << std::endl;
    evaluateArrayRawPar(random, "Random");
    evaluateArrayRawPar(sorted, "Sorted");
    evaluateArrayRawPar(reversed, "Reversed");
    evaluateArrayRawPar(organPipe, "Organ Pipe");
    evaluateArrayRawPar(rotated, "Rotated");
    std::cout << "\n";
}

void evaluateStdArray(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed, const SourceArray& organPipe, const SourceArray& rotated)
{
    // Sequential
    std::cout << " --- std::array Performance ---" << std::endl;
    std::cout << "\n";

    std::cout << "Sequential" << std::endl;
    evaluateStdAndVectorSeq(sorted, "Sorted", false);
    evaluateStdAndVectorSeq(random, "Random", false);
    evaluateStdAndVectorSeq(reversed, "Reversed", false);
    evaluateStdAndVectorSeq(organPipe, "Organ Pipe", false);
    evaluateStdAndVectorSeq(rotated, "Rotated", false);
    std::cout << "\n";

    // Parallel
    std::cout << "Parallel" << std::endl;
    evaluateStdAnVectorPar(sorted, "Sorted", false);
    evaluateStdAnVectorPar(random, "Random", false);
    evaluateStdAnVectorPar(reversed, "Reversed", false);
    evaluateStdAnVectorPar(organPipe, "Organ Pipe", false);
    evaluateStdAnVectorPar(rotated, "Rotated", false);
    std::cout << "\n";
}

void evaluateStdVector(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed, const SourceArray& organPipe, const SourceArray& rotated)
{
    // Sequential

    std::cout << " ---  std::vector Performance ---" << std::endl;
    std::cout << "\n";

    std::cout << "Sequential" << std::endl;
    evaluateStdAndVectorSeq(sorted, "Sorted", true);
    evaluateStdAndVectorSeq(random, "Random", true);
    evaluateStdAndVectorSeq(reversed, "Reversed", true);
    evaluateStdAndVectorSeq(organPipe, "Organ Pipe", true);
    evaluateStdAndVectorSeq(rotated, "Rotated", true);
    std::cout << "\n";

    // Parallel
    std::cout << "Parallel" << std::endl;
    evaluateStdAnVectorPar(sorted, "Sorted", true);
    evaluateStdAnVectorPar(random, "Random", true);
    evaluateStdAnVectorPar(reversed, "Reversed", true);
    evaluateStdAnVectorPar(organPipe, "Organ Pipe", true);
    evaluateStdAnVectorPar(rotated, "Rotated", true);
    std::cout << "\n";
}

void evaluateArrayRawSeq(const SourceArray& source, std::string name)
{
    std::chrono::milliseconds totalSortDuration(0);
    int rawArray[HOW_MANY_ELEMENTS];

    for (int i = 0; i < HOW_MANY_TIMES; i++)
    {
        auto start = std::chrono::steady_clock::now();
        initializeRawArrayFromStdArray(source, rawArray);

        std::sort(std::begin(rawArray), std::end(rawArray));

        auto end = std::chrono::steady_clock::now();

        totalSortDuration += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    }

    std::cout << std::string(8, ' ') << name << " Time" << std::string((11 - name.size()), ' ') << ": " << totalSortDuration << std::endl;
}

void evaluateArrayRawPar(const SourceArray& source, std::string name)
{
    std::chrono::milliseconds totalDurationSort(0);
    int rawArray[HOW_MANY_ELEMENTS];

    for (int i = 0; i < HOW_MANY_TIMES; i++)
    {
        auto start = std::chrono::steady_clock::now();
        initializeRawArrayFromStdArray(source, rawArray);
        std::sort(std::execution::par, std::begin(rawArray), std::end(rawArray));
        auto end = std::chrono::steady_clock::now();

        totalDurationSort += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    }

    std::cout << std::string(8, ' ') << name << " Time" << std::string((11 - name.size()), ' ') << ": " << totalDurationSort << std::endl;
}

void evaluateStdAndVectorSeq(const SourceArray& source, std::string name, bool isVector)
{
    if (isVector)
    {
        std::vector<int> testVector = std::vector<int>(source.begin(), source.end());

        std::chrono::milliseconds totalDurationSort(0);

        for (int i = 0; i < HOW_MANY_TIMES; i++)
        {
            auto start = std::chrono::steady_clock::now();

            std::sort(testVector.begin(), testVector.end());
            auto end = std::chrono::steady_clock::now();

            totalDurationSort += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        }

        std::cout << std::string(8, ' ') << name << " Time" << std::string((11 - name.size()), ' ') << ": " << totalDurationSort << std::endl;
    }
    else
    {
        std::array testArray = source;

        std::chrono::milliseconds totalDurationSort(0);

        for (int i = 0; i < HOW_MANY_TIMES; i++)
        {
            auto start = std::chrono::steady_clock::now();

            std::sort(testArray.begin(), testArray.end());
            auto end = std::chrono::steady_clock::now();

            totalDurationSort += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        }

        std::cout << std::string(8, ' ') << name << " Time" << std::string((11 - name.size()), ' ') << ": " << totalDurationSort << std::endl;
    }
}

void evaluateStdAnVectorPar(const SourceArray& source, std::string name, bool isVector)
{
    if (isVector)
    {
        std::vector<int> testVector = std::vector<int>(source.begin(), source.end());

        std::chrono::milliseconds totalDurationSort(0);

        for (int i = 0; i < HOW_MANY_TIMES; i++)
        {
            auto start = std::chrono::steady_clock::now();

            std::sort(std::execution::par, testVector.begin(), testVector.end());
            auto end = std::chrono::steady_clock::now();

            totalDurationSort += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        }
        std::cout << std::string(8, ' ') << name << " Time" << std::string((11 - name.size()), ' ') << ": " << totalDurationSort << std::endl;
    }
    else
    {
        std::array testArray = source;

        std::chrono::milliseconds totalDurationSort(0);

        for (int i = 0; i < HOW_MANY_TIMES; i++)
        {
            auto start = std::chrono::steady_clock::now();

            std::sort(std::execution::par, testArray.begin(), testArray.end());
            auto end = std::chrono::steady_clock::now();

            totalDurationSort += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        }
        std::cout << std::string(8, ' ') << name << " Time" << std::string((11 - name.size()), ' ') << ": " << totalDurationSort << std::endl;
    }
}
