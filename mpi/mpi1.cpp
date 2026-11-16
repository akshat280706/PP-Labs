#include<iostream>
#include<mpi.h>
using namespace std;

int main(int argc,char* argv[])
{
int rank;
int size;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
cout << "hello from process: " << rank << " of: " << size
<< endl;
MPI_Finalize();
return 0;
}