// the sum from 1 to 10000 
#include<iostream> 
#include<mpi.h>
using namespace std; 
int main(int argc, char ** argv)
{ 
	
	int id, nproc; 
	int sum, startval, endval, accum;
	MPI_Status status; MPI_Init(&argc, &argv);

	//get number of total nodes:

	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	// get id of mynode:
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	sum = 0;	//zero sum accumilation
	startval = 10000 * id / nproc + 1;
	endval = 10000 * (id + 1) / nproc;


	for (int i = startval; i <= endval; i++)
		sum = sum + i;
	cout << "I am the node " << id;

	cout << "; the partial sum is: " << sum << endl;

	if (id != 0)		//the slaves sending back the partial sums
		MPI_Send(&sum, 1, MPI_INT , 0, 1, MPI_COMM_WORLD);
	else
	{					//id==0 the master recieves the partial sums
		for (int j = 1; j < nproc; j++)
		{	
			//MPI_Recv(&var, count, MPI_INT, source, tag, Communicator, status);
			MPI_Recv(&accum, 1, MPI_INT, j, 1, MPI_COMM_WORLD, &status);
			sum = sum + accum;
			cout << "The sum so far is: " << sum << endl;
		}
	}
	if (id == 0)
		cout << "The sum from 1 to 10000 is: " << sum << endl;
	MPI_Finalize();

}