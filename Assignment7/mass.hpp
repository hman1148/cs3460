#pragma once
#include <compare>
#include <cstdint>
#include <ratio>

namespace usu
{
    template <typename To, typename Original>
    To mass_cast(Original original);

    template <typename R, typename C = std::uint64_t>
    class mass
    {
      public:
        mass() { m_count = 0; };
        mass(C count) { m_count = count; };

        C count() const
        {
            return m_count;
        }

        using conversion = R;
        using countType = C;

      private:
        C m_count;
    };

    template <typename To, typename Original>
    To mass_cast(Original original)
    {
        auto toCount = static_cast<To::countType>(
            (static_cast<double>(Original::conversion::num) / static_cast<double>(Original::conversion::den)) * original.count() *
            (static_cast<double>(To::conversion::den) / To::conversion::num));

        return To(toCount);
    }

    template <typename LHS, typename RHS>
    auto operator+(const LHS& lhs, const RHS& rhs)
    {
        auto converted = usu::mass_cast<LHS>(rhs);
        return LHS(lhs.count() + converted.count());
    }

    template <typename LHS, typename RHS>
    auto& operator+=(LHS& lhs, const RHS& rhs)
    {
        auto converted = usu::mass_cast<LHS>(rhs);
        lhs = lhs.count() + converted.count();
        return lhs;
    }

    template <typename LHS, typename RHS>
    auto operator-(const LHS& lhs, const RHS& rhs)
    {
        auto converted = usu::mass_cast<LHS>(rhs);
        return LHS(lhs.count() - converted.count());
    }

    template <typename LHS, typename RHS>
    auto& operator-=(LHS& lhs, const RHS& rhs)
    {
        auto convertedVal = usu::mass_cast<LHS>(rhs);
        lhs = LHS(lhs.count() - convertedVal.count());
        return lhs;
    }

    template <typename LHS, typename RHS>
    auto operator*(LHS& lhs, const RHS& rhs)
    {
        return LHS(lhs.count() * rhs);
    }

    template <typename LHS, typename RHS>
    auto operator*(const LHS& lhs, RHS& rhs)
    {
        return RHS(lhs * rhs.count());
    }

    template <typename LHS, typename C>
    auto& operator*=(LHS& lhs, const C& scalar)
    {
        lhs = lhs.count() * scalar;
        return lhs;
    }

    template <typename LHS, typename C>
    auto operator/(LHS& lhs, const C& scalar)
    {
        return LHS(lhs.count() / scalar);
    }

    template <typename LHS, typename C>
    auto& operator/=(LHS& lhs, const C& scalar)
    {
        lhs = lhs.count() / scalar;
        return lhs;
    }

    template <typename LHS, typename RHS>
    bool operator==(LHS& lhs, RHS& rhs)
    {
        auto converted = usu::mass_cast<LHS>(rhs);
        return lhs.count() == converted.count();
    }

    template <typename LHS, typename RHS>
    std::partial_ordering operator<=>(LHS& lhs, RHS& rhs)
    {
        auto converted = usu::mass_cast<LHS>(rhs);
        return lhs.count() <=> converted.count();
    }

    using microgram = mass<std::ratio<1, 1000000>, std::uint64_t>;
    using gram = mass<std::ratio<1>, std::uint64_t>;
    using kilogram = mass<std::ratio<1000, 1>, std::uint64_t>;
    using ounce = mass<std::ratio<28349523125, 1000000000>, double>;
    using pound = mass<std::ratio<45359237, 100000>, double>;
    using ton = mass<std::ratio<90718474, 100>, double>;

} // namespace usu