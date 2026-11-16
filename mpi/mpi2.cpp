#include<iostream>
#include <bits/stdc++.h>
#include<mpi.h>
using namespace std;

double func(double x){
return 4.0/(1.0+x*x);
}

int main(int argc, char* argv[]){
int noInterval;
int myRank;
int numProcess;
const int root=0;
double localPi;
double pi;
double h;
double localSum=0.0;
const double actualPi=3.1415926535;
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&numProcess);
MPI_Comm_rank(MPI_COMM_WORLD,&myRank);

if(myRank==root)
{
cout<<"enter the number of intervals: ";
cin>>noInterval;
}
MPI_Bcast(&noInterval,1,MPI_INT,root,MPI_COMM_WORLD);
if(noInterval<=0)
{
if(myRank==root)
{
cout<<"invalid interval no."<<endl;
}
MPI_Finalize();
return 1;
}
h=1.0/static_cast<double>(noInterval);
for(int i=myRank+1;i<=noInterval;i+=numProcess)
{
double x=h*(static_cast<double>(i)-0.5);
localSum+=func(x);
}
localPi=h*localSum;

MPI_Reduce(&localPi,&pi,1,MPI_DOUBLE,MPI_SUM,root,MPI_COMM_WO
RLD);
if(myRank==root)
{
cout<<"value of pi is: "<<pi<<endl;
cout<<"error is: "<<fabs(pi-actualPi)<<endl;
}
MPI_Finalize();
return 0;
}