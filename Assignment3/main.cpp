#include "distributions.hpp"

int main()
{
    auto uniform = generateUniformDistribution(100000, 0, 79, 40);
    plotDistributions("--- Uniform ---", uniform, 80);

    auto normal = generateNormalDistribution(100000, 50, 5, 40);
    plotDistributions("--- Normal ---", normal, 80);

    auto poisson = generatePoissonDistribution(100000, 6, 40);
    plotDistributions("--- Poisson ---", poisson, 80);

    return 0;
}
