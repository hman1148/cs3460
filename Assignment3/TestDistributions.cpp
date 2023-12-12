#include "distributions.hpp"

#include "gtest/gtest.h"
#include <numeric>

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

using Bins = std::vector<DistributionPair>;

Bins generateBins(const std::uint32_t min, const std::uint32_t max, const std::uint8_t numberBins)
{
    const auto binRange = (max - min) / numberBins;
    auto minBin = min;
    auto maxBin = min + binRange;

    Bins results;
    results.reserve(numberBins);
    for (auto bin = 0u; bin < numberBins; bin++)
    {
        results.emplace_back(minBin, maxBin);
        minBin = maxBin + 1;
        maxBin = minBin + binRange;
    }

    return results;
}

Bins generatePoissonBins(std::uint32_t howOften, const std::uint8_t numberBins)
{
    auto maxValue = howOften * 3 - 1;
    auto binRange = maxValue / numberBins;
    auto minBin = 0;
    auto maxBin = minBin + binRange;

    Bins results;
    results.reserve(numberBins);
    for (auto bin = 0u; bin < numberBins; bin++)
    {
        results.emplace_back(minBin, maxBin);
        minBin = maxBin + 1;
        maxBin = minBin + binRange;
    }

    return results;
}

void checkBins(const Bins& expected, const Bins& actual)
{
    ASSERT_EQ(expected.size(), actual.size()) << "Wrong number of bins";
    for (auto i = 0u; i < expected.size(); i++)
    {
        EXPECT_EQ(expected[i].minValue, actual[i].minValue) << "Wrong minimum value for bin " << i;
        EXPECT_EQ(expected[i].maxValue, actual[i].maxValue) << "Wrong maximum value for bin " << i;
    }
}

void checkTotal(const std::uint32_t expected, const Bins& bins)
{
    const auto add_counts = [](const std::uint32_t total, const DistributionPair& bin)
    {
        return total + bin.count;
    };
    const auto actual = std::accumulate(bins.cbegin(), bins.cend(), 0u, add_counts);
    EXPECT_EQ(expected, actual) << "Wrong number of elements across all bins";
}

TEST(UniformDistribution, ReturnsExpectedBins)
{
    const auto bins = generateUniformDistribution(0, 0, 79, 40);
    checkBins(generateBins(0, 79, 40), bins);
}

TEST(UniformDistribution, HasCorrectTotalAcrossAllBins)
{
    const auto bins = generateUniformDistribution(100000, 0, 79, 40);
    checkTotal(100000, bins);
}

TEST(UniformDistribution, HasCorrectZeroTotalAcrossAllBins)
{
    const auto bins = generateUniformDistribution(0, 0, 79, 40);
    checkTotal(0, bins);
}

TEST(UniformDistribution, ReturnsExpectedBinsWithMinAboveZero)
{
    const auto bins = generateUniformDistribution(0, 50, 59, 10);
    checkBins(generateBins(50, 59, 10), bins);
}

TEST(UniformDistribution, HasCorrectTotalAcrossAllBinsWithMinAboveZero)
{
    const auto bins = generateUniformDistribution(100000, 50, 59, 5);
    checkTotal(100000, bins);
}

TEST(UniformDistribution, HasCorrectTotalWithOneBin)
{
    const auto bins = generateUniformDistribution(100000, 0, 79, 1);
    checkTotal(100000, bins);
}

TEST(NormalDistribution, ReturnsExpectedBins)
{
    const auto bins = generateNormalDistribution(0, 50, 5, 40);
    checkBins(generateBins(30, 69, 40), bins);
}

TEST(NormalDistribution, HasCorrectTotalAcrossAllBins)
{
    const auto bins = generateNormalDistribution(100000, 50, 5, 40);
    checkTotal(100000, bins);
}

TEST(NormalDistribution, HasCorrectZeroTotalAcrossAllBins)
{
    const auto bins = generateNormalDistribution(0, 50, 5, 40);
    checkTotal(0, bins);
}

TEST(NormalDistribution, ReturnsExpectedBinsWithDecimalStats)
{
    const auto bins = generateNormalDistribution(0, 20.5, 1.125, 9);
    checkBins(generateBins(16, 24, 9), bins);
}

TEST(NormalDistribution, HasCorrectTotalAcrossAllBinsWithDecimalStats)
{
    const auto bins = generateNormalDistribution(100000, 20.5, 1.125, 9);
    checkTotal(100000, bins);
}

TEST(NormalDistribution, HasCorrectTotalAcrossAllBinsWithBinRangeAboveOne)
{
    const auto bins = generateNormalDistribution(100000, 50, 5, 20);
    checkTotal(100000, bins);
}

TEST(PoissonDistribution, ReturnsExpectedBins)
{
    const auto bins = generatePoissonDistribution(0, 80, 40);
    checkBins(generatePoissonBins(80, 40), bins);

    const auto bins2 = generatePoissonDistribution(0, 20, 20);
    checkBins(generatePoissonBins(20, 20), bins2);

    const auto bins3 = generatePoissonDistribution(0, 20, 10);
    checkBins(generatePoissonBins(20, 10), bins3);
}

TEST(PoissonDistribution, HasCorrectTotalAcrossAllBins)
{
    const auto bins = generatePoissonDistribution(100000, 40, 20);
    checkTotal(100000, bins);
}

TEST(PoissonDistribution, HasCorrectZeroTotalAcrossAllBins)
{
    const auto bins = generatePoissonDistribution(0, 40, 20);
    checkTotal(0, bins);
}

TEST(PoissonDistribution, HasCorrectTotalWithLargeHowOftenValue)
{
    const auto bins = generatePoissonDistribution(100000, 200, 60);
    checkTotal(100000, bins);
}