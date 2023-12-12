#include "mass.hpp"

#include "gtest/gtest.h"
#include <cstdint>
#include <ratio>

constexpr auto kEpsilon = 1E-6;
constexpr auto oneEpsilon = 1.0;

using TestMass = usu::mass<std::ratio<2, 1>, std::uint32_t>;
using BigMass = usu::mass<std::ratio<4, 1>, std::uint32_t>;
using SmallMass = usu::mass<std::ratio<1, 3>, std::uint32_t>;
using DoubleBigMass = usu::mass<std::ratio<4, 1>, double>;
using DoubleSmallMass = usu::mass<std::ratio<1, 3>, double>;

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Mass_Constructor, DefaultInitializesCountToZero)
{
    TestMass mass;

    EXPECT_EQ(0u, mass.count());
}

TEST(Mass_Constructor, OverloadedConstructorInitializesCount)
{
    TestMass mass1(12);
    EXPECT_EQ(12u, mass1.count());

    TestMass mass2(44);
    EXPECT_EQ(44u, mass2.count());
}

TEST(Mass_Operator, CanAddOtherMass)
{
    TestMass mass1(11);
    TestMass mass2(4);

    TestMass result = mass1 + mass2;
    EXPECT_EQ(15u, result.count());
    EXPECT_EQ(11u, mass1.count());
    EXPECT_EQ(4u, mass2.count());

    result = mass1 + TestMass(0);
    EXPECT_EQ(11u, result.count());
    EXPECT_EQ(11u, mass1.count());
}

TEST(Mass_Operator, CanAddOtherMassOfDifferentTypes)
{
    SmallMass mass1(23);
    BigMass mass2(3);

    SmallMass result1 = mass1 + mass2;
    EXPECT_EQ(59u, result1.count());
    EXPECT_EQ(23u, mass1.count());
    EXPECT_EQ(3u, mass2.count());

    result1 = mass1 + BigMass(1);
    EXPECT_EQ(35u, result1.count());
    EXPECT_EQ(23u, mass1.count());

    TestMass mass3(4);
    BigMass mass4(6);
    SmallMass mass5(48);

    TestMass result2 = mass3 + mass4 + mass5;
    EXPECT_EQ(24u, result2.count());
    EXPECT_EQ(4u, mass3.count());
    EXPECT_EQ(6u, mass4.count());
    EXPECT_EQ(48u, mass5.count());

    result2 = mass3 + BigMass(12) + SmallMass(60);
    EXPECT_EQ(38u, result2.count());
    EXPECT_EQ(4u, mass3.count());
}

TEST(Mass_Operator, AdditionAssignment)
{
    TestMass mass1(13);
    TestMass mass2(3);

    mass1 += mass2;
    EXPECT_EQ(16u, mass1.count());
    EXPECT_EQ(3u, mass2.count());

    TestMass mass3(7);
    TestMass mass4(8);
    TestMass mass5(14);
    mass3 += mass4 + mass5;
    EXPECT_EQ(29u, mass3.count());
    EXPECT_EQ(8u, mass4.count());
    EXPECT_EQ(14u, mass5.count());
}

TEST(Mass_Operator, AdditionAssignmentMultipleMassTypes)
{
    TestMass mass1(13);
    SmallMass mass2(6);

    mass1 += mass2;
    EXPECT_EQ(14u, mass1.count());
    EXPECT_EQ(6u, mass2.count());

    SmallMass mass3(2);
    BigMass mass4(40);
    TestMass mass5(10);
    mass3 += mass4 + mass5;
    EXPECT_EQ(542u, mass3.count());
    EXPECT_EQ(40u, mass4.count());
    EXPECT_EQ(10u, mass5.count());
}

TEST(Mass_Operator, CanSubtractOtherMass)
{
    TestMass mass1(11);
    TestMass mass2(4);

    TestMass result = mass1 - mass2;
    EXPECT_EQ(7u, result.count());
    EXPECT_EQ(11u, mass1.count());
    EXPECT_EQ(4u, mass2.count());

    result = mass1 - TestMass(0);
    EXPECT_EQ(11u, result.count());
    EXPECT_EQ(11u, mass1.count());
}

TEST(Mass_Operator, CanSubtractOtherMassOfDifferentTypes)
{
    BigMass mass1(14);
    SmallMass mass2(96);

    BigMass result1 = mass1 - mass2;
    EXPECT_EQ(6u, result1.count());
    EXPECT_EQ(14u, mass1.count());
    EXPECT_EQ(96u, mass2.count());

    result1 = mass1 - SmallMass(120);
    EXPECT_EQ(4u, result1.count());
    EXPECT_EQ(14u, mass1.count());

    SmallMass mass3(60);
    TestMass mass4(2);
    BigMass mass5(1);

    SmallMass result2 = mass3 - mass4 - mass5;
    EXPECT_EQ(36u, result2.count());
    EXPECT_EQ(60u, mass3.count());
    EXPECT_EQ(2u, mass4.count());
    EXPECT_EQ(1u, mass5.count());

    result2 = mass3 - BigMass(4) - TestMass(1);
    EXPECT_EQ(6u, result2.count());
    EXPECT_EQ(60u, mass3.count());
}

TEST(Mass_Operator, SubtractionAssignment)
{
    TestMass mass1(8);
    TestMass mass2(3);

    mass1 -= mass2;
    EXPECT_EQ(5u, mass1.count());
    EXPECT_EQ(3u, mass2.count());

    TestMass mass3(38);
    TestMass mass4(9);
    TestMass mass5(6);
    mass3 -= mass4 - mass5;
    EXPECT_EQ(35u, mass3.count());
    EXPECT_EQ(9u, mass4.count());
    EXPECT_EQ(6u, mass5.count());
}

TEST(Mass_Operator, SubtractionAssignmentMultipleMassTypes)
{
    TestMass mass1(27);
    SmallMass mass2(18);

    mass1 -= mass2;
    EXPECT_EQ(24u, mass1.count());
    EXPECT_EQ(18u, mass2.count());

    SmallMass mass3(61);
    BigMass mass4(6);
    TestMass mass5(2);
    mass3 -= mass4 - mass5;
    EXPECT_EQ(1u, mass3.count());
    EXPECT_EQ(6u, mass4.count());
    EXPECT_EQ(2u, mass5.count());
}

TEST(Mass_Operator, CanMultiplyScalarMass)
{
    TestMass myMass(14);

    TestMass result = 3 * myMass;
    EXPECT_EQ(3u * 14u, result.count());
    EXPECT_EQ(14u, myMass.count());
}

TEST(Mass_Operator, CanMultiplyMassScalar)
{
    TestMass myMass(4);

    TestMass result = myMass * 5;
    EXPECT_EQ(4u * 5u, result.count());
    EXPECT_EQ(4u, myMass.count());
}

TEST(Mass_Operator, MultiplicationAssignment)
{
    TestMass myMass(8);

    myMass *= 3;

    EXPECT_EQ(24u, myMass.count());

    myMass *= 4;

    EXPECT_EQ(96u, myMass.count());

    myMass *= 3 - 1;

    EXPECT_EQ(192u, myMass.count());
}

TEST(Mass_Operator, CanDivideMassScalar)
{
    TestMass myMass(18);

    TestMass result1 = myMass / 3;
    EXPECT_EQ(18u / 3u, result1.count());
    EXPECT_EQ(18u, myMass.count());

    TestMass result2 = myMass / 2;
    EXPECT_EQ(18u / 2u, result2.count());
    EXPECT_EQ(18u, myMass.count());
}

TEST(Mass_Operator, DivisionAssignment)
{
    TestMass myMass(400);

    myMass /= 2;

    EXPECT_EQ(200u, myMass.count());

    myMass /= 20;

    EXPECT_EQ(10u, myMass.count());

    myMass /= 4 + 1;

    EXPECT_EQ(2u, myMass.count());
}

TEST(Mass_Cast, CanDoIdentityCast)
{
    usu::mass<std::ratio<5, 1>> mass1(10); // 50 units
    decltype(mass1) result1 = usu::mass_cast<decltype(mass1)>(mass1);

    EXPECT_EQ(10u, mass1.count());
    EXPECT_EQ(10u, result1.count());

    usu::mass<std::ratio<1, 2>> mass2(5); // 2.5 units
    decltype(mass2) result2 = usu::mass_cast<decltype(mass2)>(mass2);

    EXPECT_EQ(5u, mass2.count());
    EXPECT_EQ(5u, result2.count());
}

TEST(Mass_Cast, CanCastToSmallerUnit)
{
    BigMass bigMass(15);

    auto result = usu::mass_cast<SmallMass>(bigMass);

    EXPECT_EQ(15u, bigMass.count());
    EXPECT_EQ(180u, result.count());
}

TEST(Mass_Cast, CanCastToLargerUnit)
{
    SmallMass smallMass(24);

    auto result = usu::mass_cast<BigMass>(smallMass);

    EXPECT_EQ(24u, smallMass.count());
    EXPECT_EQ(2u, result.count());
}

TEST(Mass_Cast, CanCastToLargerUnitWithPrecisionLoss)
{
    SmallMass smallMass(39);

    auto result = usu::mass_cast<BigMass>(smallMass);

    EXPECT_EQ(39u, smallMass.count());
    EXPECT_EQ(3u, result.count());
}

TEST(Mass_Cast, CanCastToLargerUnitWithoutPrecisionLoss)
{
    DoubleSmallMass smallMass(39);

    auto result = usu::mass_cast<DoubleBigMass>(smallMass);

    EXPECT_EQ(39u, smallMass.count());
    EXPECT_NEAR(3.25, result.count(), kEpsilon);
}

TEST(Mass_Cast, CanCastToLargerUnitWithoutPrecisionLossDifferentStorageTypes)
{
    SmallMass smallMass(39);

    auto result = usu::mass_cast<DoubleBigMass>(smallMass);

    EXPECT_EQ(39u, smallMass.count());
    EXPECT_NEAR(3.25, result.count(), kEpsilon);
}

TEST(Logical_Operators, EqualityInequality)
{
    SmallMass a(1);
    SmallMass b(1);
    SmallMass c(2);
    BigMass d(1);
    BigMass e(2);

    EXPECT_EQ(a == b, true);
    EXPECT_EQ(a == d, false);
    EXPECT_EQ(a == c, false);
    EXPECT_EQ(a == e, false);

    EXPECT_EQ(a != b, false);
    EXPECT_EQ(a != d, true);
    EXPECT_EQ(a != c, true);
    EXPECT_EQ(a != e, true);
}

TEST(Logical_Operators, LessThanLessThanEqual)
{
    SmallMass a(1);
    SmallMass b(1);
    SmallMass c(2);
    BigMass d(1);

    EXPECT_EQ(a < b, false);
    EXPECT_EQ(a < d, true);
    EXPECT_EQ(a < c, true);
    EXPECT_EQ(c < a, false);

    EXPECT_EQ(a <= b, true);
    EXPECT_EQ(a <= c, true);
    EXPECT_EQ(c <= a, false);
    EXPECT_EQ(a <= d, true);
}

TEST(Logical_Operators, GreaterThanGreaterThanEqual)
{
    SmallMass a(1);
    SmallMass b(1);
    SmallMass c(2);
    BigMass d(1);

    EXPECT_EQ(a > b, false);
    EXPECT_EQ(a > d, false);
    EXPECT_EQ(a > c, false);
    EXPECT_EQ(c > a, true);

    EXPECT_EQ(a >= b, true);
    EXPECT_EQ(a >= c, false);
    EXPECT_EQ(c >= a, true);
    EXPECT_EQ(a >= d, false);
}

TEST(Logical_Operators, SpaceshipStrong)
{
    SmallMass a(1);
    SmallMass b(1);
    SmallMass c(2);
    SmallMass d(12);
    BigMass e(1);

    EXPECT_EQ(a <=> b, std::strong_ordering::equal);
    EXPECT_EQ(a <=> c, std::strong_ordering::less);
    EXPECT_EQ(d <=> e, std::strong_ordering::equal);
    EXPECT_EQ(d <=> c, std::strong_ordering::greater);
}

TEST(Logical_Operators, SpaceshipPartial)
{
    DoubleSmallMass a(1);
    DoubleSmallMass b(1);
    DoubleSmallMass c(2);
    DoubleSmallMass d(12);
    DoubleBigMass e(1);

    EXPECT_EQ(a <=> b, std::partial_ordering::equivalent);
    EXPECT_EQ(a <=> c, std::partial_ordering::less);
    EXPECT_EQ(d <=> e, std::partial_ordering::equivalent);
    EXPECT_EQ(d <=> c, std::partial_ordering::greater);
}

TEST(Mass_Unit, CorrectUnitsDefined)
{
    EXPECT_EQ(std::micro::num, usu::microgram::conversion::num);
    EXPECT_EQ(std::micro::den, usu::microgram::conversion::den);
    EXPECT_EQ(usu::gram::conversion::num, usu::gram::conversion::den);
    EXPECT_EQ(std::kilo::num, usu::kilogram::conversion::num);
    EXPECT_EQ(std::kilo::den, usu::kilogram::conversion::den);
    EXPECT_EQ((std::ratio<28349523125, 1000000000>::num), usu::ounce::conversion::num);
    EXPECT_EQ((std::ratio<28349523125, 1000000000>::den), usu::ounce::conversion::den);
    EXPECT_EQ((std::ratio<45359237, 100000>::num), usu::pound::conversion::num);
    EXPECT_EQ((std::ratio<45359237, 100000>::den), usu::pound::conversion::den);
    EXPECT_EQ((std::ratio<90718474, 100>::num), usu::ton::conversion::num);
    EXPECT_EQ((std::ratio<90718474, 100>::den), usu::ton::conversion::den);
}

TEST(Mass_Unit, CastToGrams)
{
    usu::microgram ug{ 1000000 };
    usu::kilogram kg{ 10 };
    usu::ounce oz{ 20 };
    usu::pound lb{ 5 };
    usu::ton ton{ 3 };

    EXPECT_EQ(1, usu::mass_cast<usu::gram>(ug).count());
    EXPECT_EQ(10000, usu::mass_cast<usu::gram>(kg).count());
    EXPECT_NEAR(567, static_cast<double>(usu::mass_cast<usu::gram>(oz).count()), oneEpsilon);
    EXPECT_NEAR(2267, static_cast<double>(usu::mass_cast<usu::gram>(lb).count()), oneEpsilon);
    EXPECT_NEAR(2721554, static_cast<double>(usu::mass_cast<usu::gram>(ton).count()), oneEpsilon);
}

TEST(Mass_Unit, CastFromGrams)
{
    usu::gram g(1000);

    EXPECT_EQ(1000000000, usu::mass_cast<usu::microgram>(g).count());
    EXPECT_EQ(1, usu::mass_cast<usu::kilogram>(g).count());
    EXPECT_NEAR(35.27396195, static_cast<double>(usu::mass_cast<usu::ounce>(g).count()), kEpsilon);
    EXPECT_NEAR(2.2046226218, static_cast<double>(usu::mass_cast<usu::pound>(g).count()), kEpsilon);
    EXPECT_NEAR(0.0011023113109, static_cast<double>(usu::mass_cast<usu::ton>(g).count()), kEpsilon);
}