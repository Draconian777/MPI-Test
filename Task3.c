//Parallel Technologies - Assignment 
// Issa Jawabreh - OAFE3F

/*Description: The task of this program is to parse a datafile containing the numbers of an NxN matrix. 
It will them calculate the average value of each column of the matrix, then try to find a value in each column
that matches the calculated average. The output would be the indices of the columns
that satisfies this criteria
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <mpi.h>


int numOfLines(FILE* Matrixfile) {  // no const here BTW !!
	int c, count;
	count = 0;
	for (;; ) {
		c = fgetc(Matrixfile);
		if (c == EOF)
			break;          // enf of file => we quit

		if (c == '\n')
			++count;        // end of line => increment line counter
	}
	rewind(Matrixfile);

	return count;
}

int main(int argc, char** argv)
{



	//------------------------Generate-Data-------------------------------
	// Note: The data generation prompt is skipped the MPI variant of this program

	//printf("\Generate random data? (Y / N): \t");
	/**/
	char randomcheck;
	//scanf("%c", &randomcheck);
	randomcheck = 'N'; //skip data generation
	if (randomcheck == 'Y' || randomcheck == 'y')
	{
		int n = 0;
		printf("For an NxN matrix, input N\n");
		//scanf("%d", &n);
		printf("Generated data written into data.txt\n");
		srand(time(0));
		int upperN = 2000;
		int lowerN = -2000;

		double** Matrix;
		Matrix = (double**)calloc(n, sizeof(double*));
		for (int i = 0; i < n; i++)
		{
			Matrix[i] = (double*)calloc(n, sizeof(double));
		}

		for (int row = 0; row < n; row++)
		{
			for (int col = 0; col < n; col++)
			{
				Matrix[row][col] = ((lowerN + (rand() % (upperN - lowerN + 1)))) / 1.7;
			}
		}
		for (int row = 0; row < n; row++)
		{
			for (int col = 0; col < n; col++)
			{
				//printf("%lf\t", Matrix[row][col]);
				//if (col == n - 1)
				//	printf("\n");
			}
		}


		//==========================================
		//write a file
		//==========================================

		//char datafile[100];
		//printf("Save file as : ");
		//scanf("%s", &datafile);
		//datafile = "testfile.txt";
		FILE* fptr;
		fptr = fopen("testfile", "w");

		if (fptr == NULL)
		{
			printf("Error!");
			exit(1);
		}

		for (int row = 0; row < n; row++)
		{
			for (int col = 0; col < n; col++)
			{
				fprintf(fptr, "%lf\t", Matrix[row][col]);
				if (col == n - 1)
					fprintf(fptr, "\n");
			}
		}
		//deallocate memory
		for (int i = 0; i < n; i++)
			free(Matrix[i]);
		free(Matrix);
		fclose(fptr);
	}


	//==========================================
	//Read File
	//==========================================


	//note, if openMP version, the user would receive a prompt for the file name to open.
	//this is no longer the case, this program will look for a file named 'data'


	// Opening the Matrix File
	FILE* Matrixfile;
	Matrixfile = fopen("data", "r");
	//Matrixfile = fopen("dataread.txt", "r");
	if (Matrixfile == NULL)
		return 1;

	int Msize = numOfLines(Matrixfile);

	//printf("%d\n\n", Msize);


	// Reading text file into 2D array
	int i, j;


	//Allocating array memory
	double** Matrix2;
	Matrix2 = (double**)calloc(Msize, sizeof(double*));
	for (int i = 0; i < Msize; i++)
		Matrix2[i] = (double*)calloc(Msize, sizeof(double));


	//==========================================
	//Parising Data
	//==========================================

	//omp_set_num_threads(NT);
//#pragma omp parallel for private (j)			// Attempting to parallelize the data parsing code increased the execution time substantially with higher thread count
	for (i = 0; i < Msize; i++) {
		for (j = 0; j < Msize; j++) {
			fscanf(Matrixfile, "%lf", &Matrix2[i][j]);
		}

		char eol;
		fscanf(Matrixfile, "%c", &eol);     //read \n character
	}

	fclose(Matrixfile);
	double* Averages;
	Averages = (double*)calloc(Msize, sizeof(double));






	//------------------------Initialize-MPI-------------------------------
	int id, nproc;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	//Get my rank
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	//Get the Total number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);


	int sum, startval, endval, accum;

	startval = Msize * id / nproc + 1;
	endval = Msize * (id + 1) / nproc;
//==========================================
//Calcualte Averages // exectution time measurement begins from this point
//==========================================

	clock_t begin = clock();

	/*Open MP Portion
	//omp_set_num_threads(NT);
	//#pragma omp parallel for private (j)

	for (i = 0; i < Msize; i++)				//Column parsing in relation to the matrix -- i := column index, j := row index
		{
			for (j = 0; j < Msize; j++)
			{

				Averages[i] = Averages[i] + Matrix2[j][i];

				if (j == Msize - 1)
				{
					Averages[i] = Averages[i] / Msize;

				}
			}
		}
		*/

		//for (int k = 1 + id; k <= Msize; k += nproc)




	for (int i = startval-1; i < endval; i++)
	{

		//Column parsing in relation to the matrix -- i := column index, j := row index		

		for (j = 0; j < Msize; j++)
		{

			Averages[i] = Averages[i] + Matrix2[j][i];

			if (j == (Msize - 1))
			{
				Averages[i] = Averages[i] / Msize;

			}

		}
	}

	//==========================================
	//Searching for values matching Task3 criteria
	//==========================================

	int* task3;
	task3 = (int*)calloc(Msize, sizeof(int));
	int z = 0;
	for (int i = startval - 1; i < endval; i++)
		task3[i] = 0;

	for (int i = startval-1; i < endval; i++)  //column index
	{
		for (j = 0; j < Msize; j++)		//parsing rows
		{

			if (Averages[i] == Matrix2[j][i])
			{
				task3[i] = 1;
				//z++;
			}
		}
	}


	//printf("I am processor %i worked on the elements from %i to %i, and these my results:\n", id, startval-1, endval-1);
	
	/*for (int i = 0; i < nproc; i++)
	{
		if (i % nproc != id) continue;

			printf("rank %i working on element %i\n", nproc, i);

	}
	*/


	/*
		for (i = 0; i < z; i++)
		{
			if (id != 0)
			{
				MPI_Send(&task3[i], 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
				//MPI_Send(&Buff, count, MPI_DOUBLE, dest, tag, Communicator);
			}
			else
			{
				for (int j = 1; j < nproc; j = j + 1)
				{
					//MPI_Recv(&accum,1,MPI_INT,j,tag,MPI_COMM_WORLD, &status);
					MPI_Recv(&task3[i], 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);

				}

			}
		}
		*/

	clock_t end = clock();

	//Print indices
	//if (id == 0)
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	
	char fileindex[10];
	//sprintf(fileindex, " %d", id);
	_itoa(id, fileindex, 10);
	char filename[50] = "Task3_output-rank ";
	strcat(filename, fileindex);
	strcat(filename, ".txt");

	FILE* fptr3;
	fptr3 = fopen(filename, "w");
	fprintf(fptr3, "The file contains a %i by %i matrix\n\n", Msize, Msize);

	//fclose(fptr3);

	//fptr3 = fopen("Task3_output.txt", "a");
	fprintf(fptr3, "I am processor %i of %i worked on the elements from %i to %i, for %lf seconds, and these my results:\nColumns that are matching the Task 3 Criteria are:", id+1, nproc, startval-1, endval-1, time_spent);	
	//fprintf(fptr3, "Columns that are matching the Task 3 Criteria are:\t");
	printf("The file contains a %i by %i matrix\n\n", Msize, Msize);
	printf("I am processor % i of % i worked on the elements from % i to % i, for% lf seconds, and these my results : \nColumns that are matching the Task 3 Criteria are : ", id+1, nproc, startval-1, endval-1, time_spent);	
	
	int count = 0;
	for (int i = startval-1; i < endval; i++)
	{

		if (task3[i] == 1)
		{
			printf("%d, ", i);
			fprintf(fptr3," % d, ", i);
			count++;
		}
	}
	if (count == 0)
	{
		printf("none\n");
		fprintf(fptr3, "none\n");
	}
	fclose(fptr3);

	//memory deallocations

	for (int i = 0; i < Msize; i++)
		free(Matrix2[i]);
	free(Matrix2);


	MPI_Finalize();
	printf("\nPlease see Task3_output.txt\n");

	return 0;
}
