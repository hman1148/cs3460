#include "LifeSimulator.hpp"
#include "Pattern.hpp"
#include "PatternAcorn.hpp"
#include "PatternBlinker.hpp"
#include "PatternBlock.hpp"
#include "PatternGlider.hpp"
#include "PatternGosperGliderGun.hpp"
#include "PatternPulsar.hpp"

#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using State = std::vector<std::vector<bool>>;

class PatternTestToad : public Pattern
{
  public:
    virtual std::uint8_t getSizeX() const override { return static_cast<std::uint8_t>(m_sizeX); }
    virtual std::uint8_t getSizeY() const override { return static_cast<std::uint8_t>(m_sizeY); }
    virtual bool getCell(std::uint8_t x, std::uint8_t y) const override
    {
        if (x > m_sizeX - 1 || y > m_sizeY - 1)
        {
            return false;
        }
        return (m_data[y][x]);
    }

  private:
    const std::array<std::array<bool, 4>, 2> m_data = {
        { { false, true, true, true },
          { true, true, true, false } }
    };
    const std::uint8_t m_sizeX = 4;
    const std::uint8_t m_sizeY = 2;
};

class PatternOneByOne : public Pattern
{
  public:
    virtual std::uint8_t getSizeX() const override { return static_cast<std::uint8_t>(m_sizeX); }
    virtual std::uint8_t getSizeY() const override { return static_cast<std::uint8_t>(m_sizeY); }
    virtual bool getCell(std::uint8_t x, std::uint8_t y) const override
    {
        if (x > m_sizeX - 1 || y > m_sizeY - 1)
        {
            return false;
        }
        return (m_data[y][x]);
    }

  private:
    const std::array<std::array<bool, 1>, 1> m_data = {
        { { true } }
    };
    const std::uint8_t m_sizeX = 1;
    const std::uint8_t m_sizeY = 1;
};

void compareStates(const LifeSimulator& simulation, const State& update, const int& updateNum)
{
    for (std::uint8_t y = 0; y < update.size(); y++)
    {
        for (std::uint8_t x = 0; x < update[y].size(); x++)
        {
            EXPECT_EQ(update[y][x], simulation.getCell(x, y))
                << "Wrong cell state at ("
                << static_cast<int>(x)
                << ", "
                << static_cast<int>(y)
                << ") at update "
                << updateNum;
        }
    }
}

TEST(LifeSimulator_Constructor, InitialStateDead)
{
    LifeSimulator simulation = LifeSimulator(3, 3);

    for (std::uint8_t y = 0; y < simulation.getSizeY(); y++)
    {
        for (std::uint8_t x = 0; x < simulation.getSizeX(); x++)
        {
            EXPECT_FALSE(simulation.getCell(x, y))
                << "Cell alive at ("
                << static_cast<int>(x)
                << ", "
                << static_cast<int>(y)
                << ")";
        }
    }
}

TEST(LifeSimulator_GetCell, CheckOutOfBounds)
{
    LifeSimulator simulation = LifeSimulator(100, 100);

    ASSERT_FALSE(simulation.getCell(0, 100));
    ASSERT_FALSE(simulation.getCell(100, 0));
    ASSERT_FALSE(simulation.getCell(100, 100));
}

TEST(LifeSimulator_InsertPattern, CanInsertArbitraryPattern)
{
    LifeSimulator simulation = LifeSimulator(6, 4);

    simulation.insertPattern(PatternTestToad(), 1, 1);

    State insertState = {
        { false, false, false, false, false, false },
        { false, false, true, true, true, false },
        { false, true, true, true, false, false },
        { false, false, false, false, false, false }
    };

    compareStates(simulation, insertState, 0);
}

TEST(LifeSimulator_InsertPattern, OutOfBoundsHandled)
{
    LifeSimulator simulation = LifeSimulator(4, 4);

    simulation.insertPattern(PatternTestToad(), 1, 3);

    State successfulInsert = {
        { false, false, false, false },
        { false, false, false, false },
        { false, false, false, false },
        { false, false, true, true }
    };

    compareStates(simulation, successfulInsert, 0);
}

TEST(LifeSimulator_Update, Underpopulation)
{
    LifeSimulator simulation = LifeSimulator(4, 4);
    State cellDeath = {
        { false, false, false, false },
        { false, false, false, false },
        { false, false, false, false },
        { false, false, false, false }
    };

    simulation.insertPattern(PatternOneByOne(), 1, 1);

    simulation.update();
    compareStates(simulation, cellDeath, 1);

    simulation.insertPattern(PatternOneByOne(), 1, 1);
    simulation.insertPattern(PatternOneByOne(), 1, 2);

    simulation.update();
    compareStates(simulation, cellDeath, 2);
}

TEST(LifeSimulator_Update, Survival)
{
    LifeSimulator simulation = LifeSimulator(4, 4);

    simulation.insertPattern(PatternOneByOne(), 1, 1);
    simulation.insertPattern(PatternOneByOne(), 2, 1);
    simulation.insertPattern(PatternOneByOne(), 2, 2);

    simulation.update();
    EXPECT_TRUE(simulation.getCell(1, 1));
    EXPECT_TRUE(simulation.getCell(2, 1));
    EXPECT_TRUE(simulation.getCell(2, 2));

    simulation.insertPattern(PatternOneByOne(), 1, 2);

    simulation.update();
    EXPECT_TRUE(simulation.getCell(1, 1));
    EXPECT_TRUE(simulation.getCell(2, 1));
    EXPECT_TRUE(simulation.getCell(2, 2));
    EXPECT_TRUE(simulation.getCell(1, 2));
}

TEST(LifeSimulator_Update, Overpopulation)
{
    LifeSimulator simulation = LifeSimulator(5, 5);

    simulation.insertPattern(PatternOneByOne(), 2, 1);
    simulation.insertPattern(PatternOneByOne(), 3, 1);
    simulation.insertPattern(PatternOneByOne(), 3, 2);
    simulation.insertPattern(PatternOneByOne(), 2, 2);
    simulation.insertPattern(PatternOneByOne(), 1, 3);

    simulation.update();
    EXPECT_FALSE(simulation.getCell(2, 2));
}

TEST(LifeSimulator_Update, Reproduction)
{
    LifeSimulator simulation = LifeSimulator(4, 4);

    simulation.insertPattern(PatternOneByOne(), 2, 1);
    simulation.insertPattern(PatternOneByOne(), 2, 2);
    simulation.insertPattern(PatternOneByOne(), 1, 1);

    simulation.update();
    EXPECT_TRUE(simulation.getCell(1, 2));
}

TEST(LifeSimulator_Update, OutOfBoundsCellsAreDead)
{
    LifeSimulator simulation = LifeSimulator(5, 5);

    simulation.insertPattern(PatternBlock(), 0, 0);
    simulation.insertPattern(PatternBlock(), 0, 3);
    simulation.insertPattern(PatternBlock(), 3, 0);
    simulation.insertPattern(PatternBlock(), 3, 3);

    simulation.update();

    State updated = {
        { true, true, false, true, true },
        { true, true, false, true, true },
        { false, false, false, false, false },
        { true, true, false, true, true },
        { true, true, false, true, true }
    };

    compareStates(simulation, updated, 1);
}

TEST(LifeSimulator_UpdatePattern, Acorn)
{
    LifeSimulator simulation = LifeSimulator(10, 6);

    simulation.insertPattern(PatternAcorn(), 1, 1);

    const State update1 = {
        { false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false },
        { false, true, true, true, false, true, true, false, false, false },
        { false, false, false, false, false, true, true, false, false, false },
        { false, false, false, false, false, false, true, false, false, false },
        { false, false, false, false, false, false, false, false, false, false }
    };

    const State update2 = {
        { false, false, false, false, false, false, false, false, false, false },
        { false, false, true, false, false, false, false, false, false, false },
        { false, false, true, false, true, true, true, false, false, false },
        { false, false, true, false, true, false, false, true, false, false },
        { false, false, false, false, false, true, true, false, false, false },
        { false, false, false, false, false, false, false, false, false, false }
    };

    const State update3 = {
        { false, false, false, false, false, false, false, false, false, false },
        { false, false, false, true, false, true, false, false, false, false },
        { false, true, true, false, true, true, true, false, false, false },
        { false, false, false, false, true, false, false, true, false, false },
        { false, false, false, false, false, true, true, false, false, false },
        { false, false, false, false, false, false, false, false, false, false }
    };

    simulation.update();
    compareStates(simulation, update1, 1);

    simulation.update();
    compareStates(simulation, update2, 2);

    simulation.update();
    compareStates(simulation, update3, 3);
}

TEST(LifeSimulator_UpdatePattern, Blinker)
{
    LifeSimulator simulation = LifeSimulator(6, 6);

    simulation.insertPattern(PatternBlinker(), 2, 2);

    const State update1 = {
        { false, false, false, false, false, false },
        { false, false, false, false, false, false },
        { false, false, false, false, false, false },
        { false, true, true, true, false, false },
        { false, false, false, false, false, false },
        { false, false, false, false, false, false }
    };

    const State update2 = {
        { false, false, false, false, false, false },
        { false, false, false, false, false, false },
        { false, false, true, false, false, false },
        { false, false, true, false, false, false },
        { false, false, true, false, false, false },
        { false, false, false, false, false, false }
    };

    const State update3 = {
        { false, false, false, false, false, false },
        { false, false, false, false, false, false },
        { false, false, false, false, false, false },
        { false, true, true, true, false, false },
        { false, false, false, false, false, false },
        { false, false, false, false, false, false }
    };

    simulation.update();
    compareStates(simulation, update1, 1);

    simulation.update();
    compareStates(simulation, update2, 2);

    simulation.update();
    compareStates(simulation, update3, 3);
}

TEST(LifeSimulator_UpdatePattern, Block)
{
    LifeSimulator simulation = LifeSimulator(4, 4);

    simulation.insertPattern(PatternBlock(), 1, 1);

    const State update1 = {
        { false, false, false, false },
        { false, true, true, false },
        { false, true, true, false },
        { false, false, false, false }
    };

    const State update2 = {
        { false, false, false, false },
        { false, true, true, false },
        { false, true, true, false },
        { false, false, false, false }
    };

    const State update3 = {
        { false, false, false, false },
        { false, true, true, false },
        { false, true, true, false },
        { false, false, false, false }
    };

    simulation.update();
    compareStates(simulation, update1, 1);

    simulation.update();
    compareStates(simulation, update2, 2);

    simulation.update();
    compareStates(simulation, update3, 3);
}

TEST(LifeSimulator_UpdatePattern, Glider)
{
    LifeSimulator simulation = LifeSimulator(6, 6);

    simulation.insertPattern(PatternGlider(), 1, 1);

    State update1 = {
        { false, false, false, false, false, false },
        { false, false, true, false, false, false },
        { false, false, false, true, true, false },
        { false, false, true, true, false, false },
        { false, false, false, false, false, false },
        { false, false, false, false, false, false }
    };

    State update2 = {
        { false, false, false, false, false, false },
        { false, false, false, true, false, false },
        { false, false, false, false, true, false },
        { false, false, true, true, true, false },
        { false, false, false, false, false, false },
        { false, false, false, false, false, false }
    };

    State update3 = {
        { false, false, false, false, false, false },
        { false, false, false, false, false, false },
        { false, false, true, false, true, false },
        { false, false, false, true, true, false },
        { false, false, false, true, false, false },
        { false, false, false, false, false, false }
    };

    simulation.update();
    compareStates(simulation, update1, 1);

    simulation.update();
    compareStates(simulation, update2, 2);

    simulation.update();
    compareStates(simulation, update3, 3);
}

TEST(LifeSimulator_UpdatePattern, GosperGliderGun)
{
    LifeSimulator simulation = LifeSimulator(38, 11);
    simulation.insertPattern(PatternGosperGliderGun(), 1, 1);

    const State update1 = {
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, true, true, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, false, false, false, false, false, false, true, true, false },
        { false, true, true, false, false, false, false, false, false, false, false, true, true, false, false, false, false, true, true, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, true, true, false, false, false, false, false, false, false, true, true, true, false, false, false, false, true, true, false, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
    };

    simulation.update();
    compareStates(simulation, update1, 1);
}

TEST(LifeSimulator_UpdatePattern, Pulsar)
{
    LifeSimulator simulation = LifeSimulator(17, 17);
    simulation.insertPattern(PatternPulsar(), 2, 2);

    const State update1 = {
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false },
        { false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false },
        { false, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, true, true, true, false, false, true, true, false, true, true, false, false, true, true, true, false },
        { false, false, false, true, false, true, false, true, false, true, false, true, false, true, false, false, false },
        { false, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, false },
        { false, false, false, true, false, true, false, true, false, true, false, true, false, true, false, false, false },
        { false, true, true, true, false, false, true, true, false, true, true, false, false, true, true, true, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, false },
        { false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false },
        { false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
    };

    const State update2 = {
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, true, true, false, false, false, false, false, true, true, false, false, false, false },
        { false, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, false },
        { false, false, true, false, false, true, false, true, false, true, false, true, false, false, true, false, false },
        { false, false, true, true, true, false, true, true, false, true, true, false, true, true, true, false, false },
        { false, false, false, true, false, true, false, true, false, true, false, true, false, true, false, false, false },
        { false, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, false },
        { false, false, false, true, false, true, false, true, false, true, false, true, false, true, false, false, false },
        { false, false, true, true, true, false, true, true, false, true, true, false, true, true, true, false, false },
        { false, false, true, false, false, true, false, true, false, true, false, true, false, false, true, false, false },
        { false, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, false },
        { false, false, false, false, true, true, false, false, false, false, false, true, true, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

    };

    const State update3 = {
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, true, false, false, false, false, true, false, true, false, false, false, false, true, false, false },
        { false, false, true, false, false, false, false, true, false, true, false, false, false, false, true, false, false },
        { false, false, true, false, false, false, false, true, false, true, false, false, false, false, true, false, false },
        { false, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, false },
        { false, false, true, false, false, false, false, true, false, true, false, false, false, false, true, false, false },
        { false, false, true, false, false, false, false, true, false, true, false, false, false, false, true, false, false },
        { false, false, true, false, false, false, false, true, false, true, false, false, false, false, true, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
        { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
    };

    simulation.update();
    compareStates(simulation, update1, 1);

    simulation.update();
    compareStates(simulation, update2, 2);

    simulation.update();
    compareStates(simulation, update3, 3);
}