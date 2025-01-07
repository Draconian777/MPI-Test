#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv) 
{
    int id, nproc;
    
    //initialize MPI
    MPI_Init(&argc, &argv);

    //get my rank
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    //Get total number of processors
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    cout << "Process " << id << "of " << nproc << ": Hello World!" << endl;
    //Terminate MPI:
    MPI_Finalize();


    return 0;
}

    /*
    int rank;
    int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("Hello! This is process %d out of %d n\n", rank, size);
    MPI_Finalize();
    */

/*
    MPI_Send(
        void* data,
        int count,
        MPI_Datatype datatype,
        int destination,
        int tag,
        MPI_Comm communicator);

    MPI_Recv(
        void* data,
        int count,
        MPI_Datatype datatype,
        int source,                 //MPI_ANY_SOURCE
        int tag,                    //MPI_ANY_TAG
        MPI_Comm communicator,
        MPI_Status * status);


*/
    /*
// Find out rank, size
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number;
    if (world_rank == 0) {
        number = -1;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 1) {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n",
            number);
    }
    */


    /*
    int id = 0;
    int a, b;
    if (id == 0)
    {
        a = 1;
        MPI_Send(&a, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(&b, 1, MPI_INT, 1, 2, MPI_COMM_WORLD,&status);
    }
    else
    {
        b = 2;
        MPI_Send(&b, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Recv(&a, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
    */




	/*
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // Print the message
    printf("Hello World! My rank is %d\n", my_rank);
    // Finalize the MPI environment.
    MPI_Finalize();
    */
