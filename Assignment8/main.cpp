#include "vector.hpp"

#include <format>
#include <iostream>

template <typename T>
void report(T& vector)
{
    std::cout << std::format("size    : {}\ncapacity: {}\nvalues  : ", vector.size(), vector.capacity());

    for (auto i = vector.begin(); i != vector.end(); i++)
    {
        std::cout << *i << ", ";
    }
    std::cout << std::endl;
}

// ------------------------------------------------------------------
//
// Final project, a vector with a bool specialization
//
// ------------------------------------------------------------------
int main()
{

    // Simple vector construction
    usu::vector<int> v1;
    usu::vector<int> v2(20);
    usu::vector v3{ 1, 2, 3, 5, 7, 11 };

    std::cout << "\n-- v1 --\n";
    report(v1);

    std::cout << "\n-- v2 --\n";
    report(v2);

    std::cout << "\n-- v3 --\n";
    report(v3);

    // Simple vector add/insert/remove
    v3.add(23);
    std::cout << "\n-- add --\n";
    report(v3);

    v3.insert(0, 13);
    std::cout << "\n-- insert at 0 --\n";
    report(v3);

    v3.insert(4, 17);
    std::cout << "\n-- insert at 4 --\n";
    report(v3);

    v3.insert(9, 19);
    std::cout << "\n-- insert at 9 --\n";
    report(v3);

    v3.remove(0);
    std::cout << "\n-- remove at 0 --\n";
    report(v3);

    v3.remove(3);
    std::cout << "\n-- remove at 3 --\n";
    report(v3);

    v3.remove(7);
    std::cout << "\n-- remove at 7 --\n";
    report(v3);

    // Insert until new capacity is required
    v3.insert(0, 29);
    v3.insert(0, 31);
    v3.insert(0, 37);
    std::cout << "\n-- maxed capacity --\n";
    report(v3);
    v3.insert(0, 41);
    std::cout << "\n-- updated capacity --\n";
    report(v3);

    // resize with custom lambda
    usu::vector<int> v4({ 1, 2, 3, 5, 7, 11, 13, 17, 19 }, [](auto size)
                        {
                            return static_cast<usu::vector<int>::size_type>(size * 1.5);
                        });
    std::cout << "\n-- initial capacity --\n";
    report(v4);
    v4.add(23);
    std::cout << "\n-- maxed capacity --\n";
    report(v4);
    v4.add(29);
    std::cout << "\n-- updated capacity --\n";
    report(v4);

    std::cout << "\n -- iteration --\n";

    // Simple iteration - postfix
    for (auto i{ v4.begin() }; i != v4.end(); i++)
    {
        std::cout << *i << ", ";
    }
    std::cout << std::endl;

    // Simple iteration - prefix
    for (auto i{ v4.begin() }; i != v4.end(); ++i)
    {
        std::cout << *i << ", ";
    }
    std::cout << std::endl;

    // Simple iteration - decrement
    auto j{ v4.begin() };
    ++j;
    ++j;
    std::cout << *j << std::endl;
    --j;
    std::cout << *j << std::endl;
    j--;
    std::cout << *j << std::endl;

    // for-each iteration
    for (auto&& value : v4)
    {
        std::cout << value << ", ";
    }
    std::cout << std::endl;

    return 0;
}
