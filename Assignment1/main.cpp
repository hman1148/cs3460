#include <string>
#include <iostream>
#include <cmath>


long nbonacci(unsigned int series, unsigned int n) 
{
	long temp = 0;

	if (n <= series)
	{
		return 1;
	}
	
	for (int i = 1; i < series + 1; i++)
	{
		temp += nbonacci(series, n - i);
	}
	return temp;
}

void computeNbonacciRatio(std::string title, unsigned int series)
{
	int count = 0;

	double difference = 1.0;
	double previous = 0.0;
	double currentRatio = 0.0;
	double currentIterationVal = 0.0;
	double lastIterationVal = 0.0;
	int start = 1;


	while (difference >= 0.000001)
	{

		count ++;
		currentIterationVal = nbonacci(series, start);
		lastIterationVal = nbonacci(series, start - 1);

		currentRatio = (currentIterationVal / lastIterationVal);
		difference = std::abs(currentRatio - previous);

		if (currentIterationVal == lastIterationVal)
		{
			difference = 1.0;
		}

		previous = currentRatio;
	    start++;

	}
	std::cout << title << " ratio approaches " << currentRatio << " after " << count - 1 << " itterations" << std::endl;
}

void reportNbonacci(unsigned int series, int itteration, std::string title)
{

	std::cout << "--- " << title << " Sequence" << " ---" << std::endl;

	for (int i = 0; i <= itteration; i++)
	{
		std::cout << nbonacci(series, i) << "  ";
	}
	std::cout << "\n" << std::endl;
}

int main() 
{
		reportNbonacci(2, 20, "Fibonacci");
		reportNbonacci(3, 20, "Tribonacci");
		reportNbonacci(4, 20, "Quabonacci");
		reportNbonacci(5, 20, "FiveBonacci");

		computeNbonacciRatio("Fibonacci", 2);
		computeNbonacciRatio("Tribonacci", 3);
		computeNbonacciRatio("Quabonacci", 4);
		computeNbonacciRatio("FiveBonacci", 5);

	return 1;

}