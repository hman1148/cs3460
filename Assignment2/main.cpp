#include <format>
#include <iostream>
#include <random>
#include <vector>

class DistributionPair
{
  public:
    DistributionPair(std::uint32_t minValue, std::uint32_t maxValue) :
        minValue(minValue),
        maxValue(maxValue),
        count(0)
    {
    }

    std::uint32_t minValue;
    std::uint32_t maxValue;
    std::uint32_t count;
};

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
        minValue = minValue + 1;
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
    int max = numberBins - 1;

    int binRange = static_cast<int>((max - min) + 1) / (numberBins);

    auto minValue{ 0 };
    auto maxValue = static_cast<std::uint32_t>(binRange + minValue - 1);

    for (std::uint32_t i = 0; i < numberBins; i++)
    {
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

void plotDistribution(std::string title, const std::vector<DistributionPair>& distribution, const std::uint8_t maxPlotLineSize)
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

    for (auto i = 0; i < distribution.size(); i++)
    {
        std::cout << std::format("[{:3d}, {:3d}] : ", distribution[i].minValue, distribution[i].maxValue);

        int ratio = ((distribution[i].count * maxPlotLineSize) / maxCount);
        std::cout << std::format("{:<{}}", std::string(ratio, '*'), maxPlotLineSize) << std::endl;
    }
}

void test();
int main()
{
    auto uniform = generateUniformDistribution(100000, 0, 79, 40);
    plotDistribution("--- Uniform ---", uniform, 80);

    auto normal = generateNormalDistribution(100000, 50, 5, 40);
    plotDistribution("--- Normal ---", normal, 80);

    auto poisson = generatePoissonDistribution(100000, 6, 40);
    plotDistribution("--- Poisson ---", poisson, 80);

    test();
    return 0;
}

// ------------------------------------------------------------------
//
// Testing Code
//
// ------------------------------------------------------------------
#include <functional>
#include <iostream>
#include <numeric>
#include <string>

namespace testing::detail
{
    using namespace std::string_literals;

    using Bins = std::vector<std::pair<std::uint32_t, std::uint32_t>>;
    using DistFunc = std::function<std::vector<DistributionPair>()>;

#define CS3460_ASSERT_EQ(expected, actual, message)                    \
    if (expected != actual)                                            \
    {                                                                  \
        fail(message, "[ Expected", expected, "but got", actual, "]"); \
        return;                                                        \
    }

#define CS3460_CASE(x) \
    [] {               \
        return x;      \
    };                 \
    std::cout << " Case " << #x << "\n";

    template <typename Message>
    void failInternal(const Message& message)
    {
        std::cout << message << " ";
    }

    template <typename Message, typename... Messages>
    void failInternal(const Message& message, const Messages&... messages)
    {
        failInternal(message);
        failInternal(messages...);
    }

    template <typename... Messages>
    void fail(const Messages&... messages)
    {
        std::cout << "  Assertion failed: ";
        failInternal(messages...);
        std::cout << "\n";
    }

    Bins generateBins(const std::uint32_t min, const std::uint32_t max, const std::uint8_t numberBins)
    {
        const auto binRange = (max - min) / numberBins;
        auto minBin = min;
        auto maxBin = min + binRange;

        Bins results(numberBins);
        for (std::uint8_t bin = 0u; bin < numberBins; bin++)
        {
            results[bin] = { minBin, maxBin };
            minBin = maxBin + 1;
            maxBin = minBin + binRange;
        }

        return results;
    }

    void returnsTheExpectedBins(const DistFunc& func, const Bins& bins)
    {
        const auto result = func();
        CS3460_ASSERT_EQ(bins.size(), result.size(), "Wrong number of bins");
        for (auto i = 0u; i < bins.size(); i++)
        {
            CS3460_ASSERT_EQ(bins[i].first, result[i].minValue, "Wrong minimum value for bin "s + std::to_string(i));
            CS3460_ASSERT_EQ(bins[i].second, result[i].maxValue, "Wrong maximum value for bin "s + std::to_string(i));
        }
    }

    void hasTheCorrectTotalAcrossAllBins(const DistFunc& func, const std::uint32_t howMany)
    {
        const auto result = func();
        const auto add_counts = [](std::uint32_t total, const DistributionPair& bin)
        {
            return total + bin.count;
        };
        CS3460_ASSERT_EQ(howMany, std::accumulate(result.cbegin(), result.cend(), 0u, add_counts),
                         "Wrong number of elements across all bins");
    }

    void testUniformDistribution()
    {
        std::cout << "Testing generateUniformDistribution\n";
        auto func = CS3460_CASE(generateUniformDistribution(100000, 0, 79, 40));
        returnsTheExpectedBins(func, generateBins(0, 79, 40));
        hasTheCorrectTotalAcrossAllBins(func, 100000);

        auto func2 = CS3460_CASE(generateUniformDistribution(0, 50, 59, 10));
        returnsTheExpectedBins(func2, generateBins(50, 59, 10));

        auto func3 = CS3460_CASE(generateUniformDistribution(0, 0, 79, 40));
        hasTheCorrectTotalAcrossAllBins(func3, 0);

        auto func4 = CS3460_CASE(generateUniformDistribution(100000, 0, 79, 1));
        hasTheCorrectTotalAcrossAllBins(func4, 100000);
    }

    void testNormalDistribution()
    {
        std::cout << "Testing generateNormalDistribution\n";
        auto func = CS3460_CASE(generateNormalDistribution(100000, 50, 5, 40));
        returnsTheExpectedBins(func, generateBins(30, 69, 40));
        hasTheCorrectTotalAcrossAllBins(func, 100000);

        auto func2 = CS3460_CASE(generateNormalDistribution(0, 50, 5, 40));
        hasTheCorrectTotalAcrossAllBins(func2, 0);

        auto func3 = CS3460_CASE(generateNormalDistribution(100000, 20.5, 1.125, 9));
        returnsTheExpectedBins(func3, generateBins(16, 24, 9));
        hasTheCorrectTotalAcrossAllBins(func3, 100000);
    }

    void testPoissonDistribution()
    {
        std::cout << "Testing generatePoissonDistribution\n";
        auto func = CS3460_CASE(generatePoissonDistribution(100000, 6, 40));
        returnsTheExpectedBins(func, generateBins(0, 39, 40));
        hasTheCorrectTotalAcrossAllBins(func, 100000);

        auto func2 = CS3460_CASE(generatePoissonDistribution(0, 6, 40));
        hasTheCorrectTotalAcrossAllBins(func2, 0);

        auto func3 = CS3460_CASE(generatePoissonDistribution(100000, 255, 40));
        hasTheCorrectTotalAcrossAllBins(func3, 100000);
    }
} // namespace testing::detail

void test()
{
    using namespace testing::detail;

    testUniformDistribution();
    testNormalDistribution();
    testPoissonDistribution();

    std::cout << "\n\n";
}
