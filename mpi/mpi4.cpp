#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[])
{
MPI_Init(&argc, &argv);
int rank;
int numProcesses;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
if (numProcesses != 6)
{
if (rank == 0) cerr << "run program with 6 processes"<< endl;
MPI_Finalize();
return 1;
}

int array[6] = {100, 600, 300, 800, 250, 720};
int inputNumber = array[rank];
int maxNumber = 0;
cout << "Local input for process " << rank<< " is " <<
inputNumber << endl;
MPI_Reduce(&inputNumber, &maxNumber, 1, MPI_INT, MPI_MAX,
0, MPI_COMM_WORLD);
if (rank == 0) cout << "Maximum of all is: " << maxNumber<< endl;
MPI_Finalize();
return 0;
}