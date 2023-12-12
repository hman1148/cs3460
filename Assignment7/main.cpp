#include "mass.hpp"

#include <format>
#include <iostream>
#include <string>

template <typename LHS, typename RHS>
std::string spaceshipCompare(const LHS& lhs, const RHS& rhs)
{
    if (lhs <=> rhs == std::strong_ordering::less || lhs <=> rhs == std::partial_ordering::less)
    {
        return "less";
    }
    else if (lhs <=> rhs == std::strong_ordering::greater || lhs <=> rhs == std::partial_ordering::greater)
    {
        return "greater";
    }
    else if (lhs <=> rhs == std::strong_ordering::equal || lhs <=> rhs == std::partial_ordering::equivalent)
    {
        return "equal";
    }
    return "unordered";
}

// ------------------------------------------------------------------
//
// Operator overloading, various constructors, more templates, etc
//
// ------------------------------------------------------------------
int main()
{
    usu::microgram mcg{ 1000000 };
    usu::gram g = usu::mass_cast<usu::gram>(mcg);
    usu::pound lb = usu::mass_cast<usu::pound>(g);
    usu::ounce oz = usu::mass_cast<usu::ounce>(lb);
    usu::ton ton = usu::mass_cast<usu::ton>(lb);

    std::cout << "--- mass_cast<> From micrograms ---" << std::endl;
    std::cout << std::format("micrograms : {:>12d}\n", mcg.count());
    std::cout << std::format("grams      : {:>12d}\n", g.count());
    std::cout << std::format("lbs        : {:>12.10f}\n", lb.count());
    std::cout << std::format("ounces     : {:>12.10f}\n", oz.count());
    std::cout << std::format("tons       : {:>12.10f}\n", ton.count());

    usu::pound somePounds(2000);
    usu::microgram ugFromPounds = usu::mass_cast<usu::microgram>(somePounds);
    usu::gram gramsFromPounds = usu::mass_cast<usu::gram>(somePounds);
    usu::ounce ouncesFromPounds = usu::mass_cast<usu::ounce>(somePounds);
    usu::ton tonsFromPounds = usu::mass_cast<usu::ton>(ouncesFromPounds);

    std::cout << "\n--- mass_cast<> From pounds ---\n";
    std::cout << std::format("micrograms : {:>12d}\n", ugFromPounds.count());
    std::cout << std::format("grams      : {:>12d}\n", gramsFromPounds.count());
    std::cout << std::format("lbs        : {:>12.4f}\n", somePounds.count());
    std::cout << std::format("ounces     : {:>12.4f}\n", ouncesFromPounds.count());
    std::cout << std::format("tons       : {:>12.4f}\n", tonsFromPounds.count());

    std::cout << "\n--- Arithmetic Operators ---\n";
    // Addition
    {
        usu::pound a{ 1 };
        usu::pound b{ 0.5 };
        usu::pound c = a + b;
        std::cout << std::format("(pound + pound)  : {:.2f} + {:.2f} = {:.2f} ==> grams: {}\n",
                                 a.count(), b.count(), c.count(), usu::mass_cast<usu::gram>(c).count());

        usu::pound d{ 1 };
        usu::pound before{ d };
        d += b;
        std::cout << std::format("(pound += pound)  : {:.2f} += {:.2f} = {:.2f} ==> grams: {}\n",
                                 before.count(), b.count(), d.count(), usu::mass_cast<usu::gram>(d).count());

        usu::gram e = usu::mass_cast<usu::gram>(usu::pound{ .5 });
        usu::pound f{ 1 };
        before = f;
        f += e;
        std::cout << std::format("(pound += gram)  : {:.2f} += {} = {:.2f} ==> grams: {}\n",
                                 before.count(), e.count(), f.count(), usu::mass_cast<usu::gram>(f).count());
    }

    // Subtraction
    {
        std::cout << std::endl;
        usu::pound a{ 1 };
        usu::pound b{ 0.5 };
        usu::pound c = a - b;
        std::cout << std::format("(pound - pound)  : {:.2f} - {:.2f} = {:.2f} ==> grams: {}\n",
                                 a.count(), b.count(), c.count(), usu::mass_cast<usu::gram>(c).count());
        usu::pound d{ 1 };
        usu::pound before{ d };
        d -= b;
        std::cout << std::format("(pound -= pound)  : {:.2f} -= {:.2f} = {:.2f} ==> grams: {}\n",
                                 before.count(), b.count(), d.count(), usu::mass_cast<usu::gram>(d).count());

        usu::gram e = usu::mass_cast<usu::gram>(usu::pound{ 0.5 });
        usu::pound f{ 1 };
        before = f;
        f -= e;
        std::cout << std::format("(pound -= gram)  : {:.2f} -= {} = {:.2f} ==> grams: {}\n",
                                 before.count(), e.count(), f.count(), usu::mass_cast<usu::gram>(f).count());
    }

    // Scalar multiplication
    {
        std::cout << std::endl;
        usu::pound a{ 1 };
        usu::pound b = a * 2.2;
        usu::pound c = 3.2 * a;
        std::cout << std::format("(pound * scalar) : {:.2f} * 2.20 = {:.2f} ==> grams: {}\n",
                                 a.count(), b.count(), usu::mass_cast<usu::gram>(b).count());

        std::cout << std::format("(scalar * pound) : 3.20 * {:.2f} = {:.2f} ==> grams: {}\n",
                                 a.count(), c.count(), usu::mass_cast<usu::gram>(c).count());

        usu::pound d{ 1.5 };
        usu::pound before{ d };
        d *= 2;
        std::cout << std::format("(pound *= scalar) : {:.2f} *= 2 = {:.2f} ==> grams: {}\n",
                                 before.count(), d.count(), usu::mass_cast<usu::gram>(d).count());
    }

    // Scalar division
    {
        std::cout << std::endl;
        usu::pound a{ 2 };
        usu::pound b = a / 2;
        std::cout << std::format("(pound / scalar) : {:.2f} / 2 = {:.2f} ==> grams: {}\n",
                                 a.count(), b.count(), usu::mass_cast<usu::gram>(b).count());

        usu::gram c{ 6 };
        usu::gram d = c / 2;
        usu::gram e = c / 4;
        std::cout << std::format("(gram / scalar) : {0:d} / 2 = {1:d} ==> grams: {1:d}\n",
                                 c.count(), d.count());
        std::cout << std::format("(gram / scalar) : {0:d} / 4 = {1:d} ==> grams: {1:d}\n",
                                 c.count(), e.count());

        usu::pound f{ 3 };
        usu::pound before{ f };
        f /= 2;
        std::cout << std::format("(pound /= scalar) : {:.2f} /= 2 = {:.2f} ==> grams: {}\n",
                                 before.count(), f.count(), usu::mass_cast<usu::gram>(f).count());
    }

    // Adding mixed types
    {
        std::cout << std::endl;
        usu::gram gramResult = g + lb + oz;
        usu::ounce ozResult = oz + g + lb;
        usu::pound lbResult = lb + g + oz;

        std::cout << std::format("(gram + pound + ounce) : {:d} + {:.6f} + {:.6f} = {:d} grams\n",
                                 g.count(), lb.count(), oz.count(), gramResult.count());
        std::cout << std::format("(ounce + gram + pound) : {:.6f} + {:d} + {:.6f} = {:.6f} ounces\n",
                                 oz.count(), g.count(), lb.count(), ozResult.count());
        std::cout << std::format("(pound + gram + ounce) : {:.6f} + {:d} + {:.6f} = {:.6f} pounds\n",
                                 lb.count(), g.count(), oz.count(), lbResult.count());
        usu::gram gramPlusEqual = g;
        gramPlusEqual += lb;
        std::cout << std::format("(gram += lb) : {:d} += {:.6f} = {:d} grams\n",
                                 g.count(), lb.count(), gramPlusEqual.count());
        gramPlusEqual = g;
        gramPlusEqual += oz;
        std::cout << std::format("(gram += oz) : {:d} += {:.6f} = {:d} grams\n",
                                 g.count(), oz.count(), gramPlusEqual.count());
    }

    std::cout << "\n--- Logical Operators ---\n";
    {
        usu::ounce a{ 16 };
        usu::pound b{ 1 };
        usu::kilogram c{ 4 };
        usu::kilogram d{ 1 };

        std::cout << std::format("{} oz == {} lb : {}\n", a.count(), b.count(), a == b);
        std::cout << std::format("{} oz == {} kg : {}\n", a.count(), c.count(), a == c);
        std::cout << std::format("{} kg == {} kg : {}\n", c.count(), d.count(), c == d);

        std::cout << std::endl;
        std::cout << std::format("{} oz != {} lb : {}\n", a.count(), b.count(), a != b);
        std::cout << std::format("{} oz != {} kg : {}\n", a.count(), c.count(), a != c);
        std::cout << std::format("{} kg != {} kg : {}\n", c.count(), d.count(), c != d);

        std::cout << std::endl;
        std::cout << std::format("{} oz < {} lb : {}\n", a.count(), b.count(), a < b);
        std::cout << std::format("{} oz < {} kg : {}\n", a.count(), c.count(), a < c);
        std::cout << std::format("{} kg < {} kg : {}\n", c.count(), d.count(), c < d);

        std::cout << std::endl;
        std::cout << std::format("{} oz > {} lb : {}\n", a.count(), b.count(), a > b);
        std::cout << std::format("{} oz > {} kg : {}\n", a.count(), c.count(), a > c);
        std::cout << std::format("{} kg > {} kg : {}\n", c.count(), d.count(), c > d);

        std::cout << std::endl;
        std::cout << std::format("{} oz <= {} lb : {}\n", a.count(), b.count(), a <= b);
        std::cout << std::format("{} oz <= {} kg : {}\n", a.count(), c.count(), a <= c);
        std::cout << std::format("{} kg <= {} kg : {}\n", c.count(), d.count(), c <= d);

        std::cout << std::endl;
        std::cout << std::format("{} oz >= {} lb : {}\n", a.count(), b.count(), a >= b);
        std::cout << std::format("{} oz >= {} kg : {}\n", a.count(), c.count(), a >= c);
        std::cout << std::format("{} kg >= {} kg : {}\n", c.count(), d.count(), c >= d);

        std::cout << std::endl;
        std::cout << std::format("{} oz <=> {} lb : {}\n", a.count(), b.count(), spaceshipCompare(a, b));
        std::cout << std::format("{} oz <=> {} kg : {}\n", a.count(), c.count(), spaceshipCompare(a, c));
        std::cout << std::format("{} kg <=> {} kg : {}\n", c.count(), d.count(), spaceshipCompare(c, d));

        return 0;
    }
}
