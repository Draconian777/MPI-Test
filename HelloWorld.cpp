#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv)
{


	int id, nproc, id_from;
	MPI_Status status;

    //initialize MPI
    MPI_Init(&argc, &argv);

    //get my rank
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    //Get total number of processors
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);



    if (id != 0)
    {
        //i.e. Slave Processes sending greetings:

        cout << "Process id=" << id  << " Sending Greetings"<< endl;
        MPI_Send(&id, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

    }

    else
    {
        //Master Process Receiving greetings
        cout << "Master Process (id=0) receivng greetings:"<<endl;
        for (int i = 1; i < nproc; ++i)
        {
            MPI_Recv(&id_from, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            cout << "Greetings from process " << id_from << "!" << endl;
        }        
    }
    //Terminate MPIU
    MPI_Finalize();

}