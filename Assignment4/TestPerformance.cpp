#include "sortutils.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <random>

constexpr auto MIN_VALUE = -10'000'000;
constexpr auto MAX_VALUE = 10'000'000;

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

SourceArray generateRandomArray()
{
    std::default_random_engine engine{ 0u };
    std::uniform_int_distribution<> dist{ MIN_VALUE, MAX_VALUE };

    SourceArray array;
    std::generate(array.begin(), array.end(), [&]
                  {
                      return dist(engine);
                  });
    return array;
}

TEST(OrganPipeArray, DoesGenerateCorrectly)
{
    auto sourceArray = generateRandomArray();
    std::sort(sourceArray.begin(), sourceArray.end());
    auto array = sourceArray;
    organPipeStdArray(array);

    for (auto i = 0ul; i < sourceArray.size() / 2; i++)
    {
        ASSERT_EQ(sourceArray[i], array[i]) << "Organ pipe changed first half of array at position " << i;
    }

    for (std::size_t first = 0ul, second = array.size() - 1; first < second; first++, second--)
    {
        ASSERT_EQ(array[first], array[second]) << "Organ pipe not symmetric at positions " << first << " and " << second;
    }
}

TEST(RawArray, DoesInitializeFromStdArrayCorrectly)
{
    const auto sourceArray = generateRandomArray();
    int dest[HOW_MANY_ELEMENTS];
    initializeRawArrayFromStdArray(sourceArray, dest);

    for (auto i = 0ul; i < sourceArray.size(); i++)
    {
        ASSERT_EQ(sourceArray[i], dest[i]) << "Raw array differs from source array at position " << i;
    }
}
