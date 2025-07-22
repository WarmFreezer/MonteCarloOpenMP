#include <omp.h>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

void Serial(int numTosses);
void Parallel(int i, int numTosses);

int main()
{
	string input;
	long numTosses;

	bool inIsNumber = false;
	while (!inIsNumber)
	{
		cout << "Enter a number of tosses: ";
		cin >> input;
		cout << endl;

		for (char x : input)
		{
			if (!isdigit(x))
			{
				inIsNumber = 0;
				break;
			}
			else
			{
				inIsNumber = 1;
			}
		}

		if (inIsNumber)
		{
			numTosses = stol(input);
		}
	}

	Serial(numTosses);

	for (int i = 1; i <= 12; i*=2)
	{
		Parallel(i, numTosses);
	}

	return 0;
}

void Serial(int numTosses)
{
	cout << "Serial program\n";

	srand(static_cast<unsigned int>(time(0)));
	int numCircle = 0;

	double start = omp_get_wtime();
	for (int toss = 0; toss < numTosses; toss++)
	{
		double x = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
		double y = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
		double distSqr = x * x + y * y;
		if (distSqr <= 1)
		{
			numCircle++;
		}
	}
	double time = omp_get_wtime() - start;

	double piEst = 4 * numCircle / ((double)numTosses);

	cout << time << " seconds\nPi Estimate: " << piEst << "\n\n";
}

void Parallel(int i, int numTosses)
{
	cout << "Parallel using " << i << " thread(s)\n";

	omp_set_num_threads(i);
	int numCircle = 0;

	double start = omp_get_wtime();
	#pragma omp parallel 
	{
		int localNumCircle = 0;

		#pragma omp for schedule(static)
		for (int toss = 0; toss < numTosses; toss++)
		{
			double x = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
			double y = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
			double distSqr = x * x + y * y;
			if (distSqr <= 1)
			{
				localNumCircle++;
			}
		}

		#pragma omp critical
		{
			numCircle += localNumCircle;
		}
	}
	double time = omp_get_wtime() - start;
	
	double piEst = 4 * numCircle / ((double)numTosses);

	cout << time << " seconds\nPi Estimate: " << piEst << "\n\n";
}