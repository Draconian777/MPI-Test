//calculation of PI by Monte Carlo method
#include<cstdlib>
#include<ctime>
#include<math.h>
#include<iostream> 
#include<mpi.h>
//#include <corecrt_math_defines.h>
//#define _USE_MATH_DEFINES
using namespace std;

//#define M_PI       3.14159265358979323846   // pi


int main(int argc, char** argv)
{

	int id, nproc;
	MPI_Status status;
	double x, y, Pi, error;
	long long allsum;
	const long long iternum= 1000000000;

	//initialize MPI
	MPI_Init(&argc, &argv);

	//get rank
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	//Get total number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	srand((unsigned)time(0));
	cout.precision(12);

	long long sum = 0;
	

	//QUESTION: Should the part below should be parellelized as well?
	for (long long i = 0; i < iternum; i++)
	{
		x = (double)rand() / RAND_MAX;
		y = (double)rand() / RAND_MAX;
		if (x * x + y * y < 1)
			sum++;

	}


	//Slave
	if (id != 0)
	{
		MPI_Send(&sum, 1, MPI_LONG_LONG, 0, 1, MPI_COMM_WORLD);
	}

	//Master
	else
	{
		allsum = sum;
		for (int i = 1; i < nproc; ++i)
		{
			MPI_Recv(&sum, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		}
		allsum += sum;
	}

	//Calculate PI and compare with math.h
	Pi = (4.0 * allsum) / (iternum * nproc);
	error = fabs(Pi - M_PI);

	cout << "Pi: \t\t" << M_PI << endl;
	cout << "Pi by MC: \t\t" << Pi<< endl;
	cout << "Error: \t\t" << fixed << error << endl;

	//Terminate MPI
	MPI_Finalize();
	return 0;

}


