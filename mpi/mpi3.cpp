#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char* argv[])
{
MPI_Init(&argc, &argv);
int rank;
int size;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

if (size != 4)
{
if (rank == 0)

cerr << "run this program with exactly 4processes." << endl;

MPI_Finalize();
return 1;
}
char letters[4] = {'V', 'J', 'T', 'I'};
char receivedLetter;
MPI_Scatter(letters, 1, MPI_CHAR, &receivedLetter, 1,
MPI_CHAR, 0, MPI_COMM_WORLD);

cout << "Process "<<rank<< " received: "<<receivedLetter << endl;
MPI_Finalize();
return 0;
}