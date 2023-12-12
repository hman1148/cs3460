#include "distributions.hpp"

#include <format>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<DistributionPair> generateUniformDistribution(std::uint32_t howMany, std::uint32_t min, std::uint32_t max, std::uint8_t numberBins)
{
    std::vector<DistributionPair> bins;

    auto binRange = static_cast<std::uint32_t>((max - min) + 1) / (numberBins);
    auto minValue{ min };

    for (std::uint32_t i = 0; i < numberBins; i++)
    {
        auto maxValue = binRange + minValue - 1;
        bins.push_back(DistributionPair(static_cast<std::uint32_t>(minValue), static_cast<std::uint32_t>(maxValue)));
        minValue = maxValue + 1;
    }

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<std::uint32_t> uniformDistribution(min, max);

    for (std::uint32_t i = 0; i < howMany; i++)
    {
        auto randomVal = uniformDistribution(engine);

        for (DistributionPair& bin : bins)
        {
            if (bin.minValue <= randomVal && bin.maxValue >= randomVal)
            {
                bin.count++;
            }
        }
    }
    return bins;
}

std::vector<DistributionPair> generateNormalDistribution(std::uint32_t howMany, float mean, float stdev, std::uint8_t numberBins)
{
    std::vector<DistributionPair> bins;

    auto min = mean - 4 * stdev;
    auto max = mean + 4 * stdev - 1;
    auto binRange = static_cast<std::uint32_t>((max - min) + 1) / (numberBins);

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::normal_distribution<float> normalDistribution(mean, stdev);

    auto minValue{ min };

    for (std::uint32_t i = 0; i < numberBins; i++)
    {
        auto maxValue = minValue + binRange - 1;
        bins.push_back(DistributionPair(static_cast<std::uint32_t>(minValue), static_cast<std::uint32_t>(maxValue)));
        minValue = maxValue + 1;
    }

    for (std::uint32_t i = 0; i < howMany; i++)
    {
        float randomVal = normalDistribution(engine);

        if (randomVal < min)
        {
            randomVal = min;
        }
        else if (randomVal > max)
        {
            randomVal = max;
        }
        for (DistributionPair& bin : bins)
        {
            if (randomVal >= bin.minValue && std::floor(randomVal) <= bin.maxValue)
            {
                bin.count++;
            }
        }
    }
    return bins;
}

std::vector<DistributionPair> generatePoissonDistribution(std::uint32_t howMany, std::uint8_t howOften, std::uint8_t numberBins)
{
    std::vector<DistributionPair> bins;

    std::uint32_t min = 0;
    int max = (howOften * 3) - 1;

    int binRange = static_cast<int>((max - min) + 1) / (numberBins);

    auto minValue{ 0 };
    auto maxValue = static_cast<std::uint32_t>(binRange + minValue - 1);

    for (std::uint32_t i = 0; i < numberBins; i++)
    {
        if (static_cast<std::uint8_t>(i) == (numberBins - 1))
        {
            maxValue = max;
        }

        maxValue = binRange + minValue - 1;
        bins.push_back(DistributionPair(static_cast<std::uint32_t>(minValue), static_cast<std::uint32_t>(maxValue)));
        minValue = maxValue + 1;
    }

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::poisson_distribution<std::uint32_t> poisson(howOften);

    for (std::uint32_t i = 0; i < howMany; i++)
    {
        std::uint32_t randomVal = poisson(engine);

        if (randomVal < min)
        {
            randomVal = 0;
        }
        else if (randomVal > binRange + maxValue - 1)
        {
            randomVal = numberBins - 1;
        }
        for (DistributionPair& bin : bins)
        {
            if (bin.minValue <= randomVal && bin.maxValue >= randomVal)
            {
                bin.count++;
            }
        }
    }
    return bins;
}

void plotDistributions(std::string title, const std::vector<DistributionPair>& distribution, const std::uint8_t maxPlotLineSize)
{
    unsigned int maxCount = 0;
    for (const DistributionPair& pair : distribution)
    {
        if (pair.count > maxCount)
        {
            maxCount = pair.count;
        }
    }

    std::cout << title << std::endl;

    for (size_t i = 0; i < distribution.size(); i++)
    {
        std::cout << std::format("[{:3d}, {:3d}] : ", distribution[i].minValue, distribution[i].maxValue);

        int ratio = ((distribution[i].count * maxPlotLineSize) / maxCount);
        std::cout << std::format("{:<{}}", std::string(ratio, '*'), maxPlotLineSize) << std::endl;
    }
}
