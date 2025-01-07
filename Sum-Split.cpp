// Parallel calculation of a sum from 1 to 10000 by loop splitting

#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv)
{
	int id, nproc;
	int sum, accum, startval, endval;
	MPI_Status status;
	sum = 0;

	MPI_Init(&argc, &argv);
	//Get the Total number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	//Get my rank
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	//startval = 10000*id/nproc+1;
	//endval = 10000*(id+1)/nproc;

	for (int i = 1 + id;i <= 10000; i += nproc)
		sum = sum + i;
	cout << "I am the node " << id << "; the partial sum is: " << sum << endl;

	if (id!=0)
		MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	else
		for (int j = 1; j < nproc; j = j + 1)
		{
			//MPI_Recv(&accum,1,MPI_INT,j,tag,MPI_COMM_WORLD, &status);
			MPI_Recv(&accum, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
			// we do not wait for particular slave: recieve answer from anybody

			sum = sum + accum;
			cout << "The sum so far is: " << sum << endl;
		}
	if (id==0)
		cout << "The sum is: " << sum << endl;

	MPI_Finalize();


}