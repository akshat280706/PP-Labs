#include<iostream>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[])
{
int myRank;
int numProc;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &numProc);
MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
if(myRank==0)
{
int sum=0;
int value;
MPI_Status status;
for(int i=1;i<numProc;i++)
{
MPI_Recv(&value,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
sum += value;
}
cout<<"myRank: "<<myRank<<", SUM="<<sum<<endl;
}else
{
MPI_Send(&myRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}
MPI_Finalize();
return 0;
}