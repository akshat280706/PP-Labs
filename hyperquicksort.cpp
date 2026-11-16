#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
using namespace std;
constexpr int N = 10000000;
int main(int argc, char** argv) {
MPI_Init(&argc, &argv);
int rank = 0;
int size = 0;
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);
if(size!=4){
if(rank==0){
cerr<<"error, use only 4 process\n";
}
MPI_Finalize();
return 1;
}
vector<int>global_data;
const int local_n=N/size;
vector<int>local_data(local_n);
double seq_time=0.0;
if(rank==0){
global_data.resize(N);
mt19937 rng(42);
uniform_int_distribution<int>dist(0,999999);
for(int i=0;i<N;++i) global_data[i]=dist(rng);

vector<int>seq_copy=global_data;
double seq_start=MPI_Wtime();
sort(seq_copy.begin(),seq_copy.end());
seq_time=MPI_Wtime()-seq_start;
cout<<"seq sort time: "<<seq_time<<" seconds\n";
}
MPI_Barrier(MPI_COMM_WORLD);
double par_start=MPI_Wtime();
MPI_Scatter(global_data.data(),local_n,MPI_INT,local_data.data(),local_n,M
PI_INT,0,MPI_COMM_WORLD);
#pragma omp parallel
{
#pragma omp single nowait
sort(local_data.begin(),local_data.end());
}
for (int d=0;d<2;++d) {
const int mask=1<<(1-d);
const int partner=rank^mask;
const int root=rank & ~((1<<(2-d))-1);
int pivot = 0;
if (rank==root) pivot=local_data[local_data.size()/2];
MPI_Bcast(&pivot, 1, MPI_INT, root, MPI_COMM_WORLD);
auto
split_it=upper_bound(local_data.begin(),local_data.end(),pivot);
vector<int>keep_part;
vector<int>send_part;
if(rank<partner){
keep_part.assign(local_data.begin(),split_it);
send_part.assign(split_it,local_data.end());
} else {
send_part.assign(local_data.begin(),split_it);
keep_part.assign(split_it,local_data.end());
}

int send_size=static_cast<int>(send_part.size());
int recv_size=0;
MPI_Sendrecv(&send_size,1,MPI_INT,partner,0,&recv_size,1,MPI_INT,partner,0
,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
vector<int>recv_part(recv_size);
MPI_Sendrecv(send_part.data(),send_size,
MPI_INT,partner,1,recv_part.data(),recv_size,MPI_INT,partner,1,MPI_COMM_WO
RLD,MPI_STATUS_IGNORE);
local_data.resize(keep_part.size()+recv_part.size());
merge(keep_part.begin(),keep_part.end(),recv_part.begin(),recv_part.end(),
local_data.begin());
}
#pragma omp parallel
{
#pragma omp single nowait
sort(local_data.begin(),local_data.end());
}
int local_size=static_cast<int>(local_data.size());
vector<int>recv_counts(4,0);
vector<int>displs(4,0);
MPI_Gather(&local_size,1,MPI_INT,recv_counts.data(),1,MPI_INT,0,MPI_COMM_W
ORLD);
if(rank==0){
displs[0]=0;
for (int i=1;i<4;++i) {
displs[i]=displs[i-1]+recv_counts[i-1];
}
global_data.resize(displs[3]+recv_counts[3]);
}

MPI_Gatherv(local_data.data(),local_size,MPI_INT,global_data.data(),recv_c
ounts.data(),displs.data(),MPI_INT,0,MPI_COMM_WORLD);
double par_time=MPI_Wtime()-par_start;

if(rank==0){
cout<<"Parallel Time:"<<par_time<<" seconds\n";
cout<<"Speedup:"<<(seq_time/par_time)<<"x\n";
bool
is_sorted_flag=is_sorted(global_data.begin(),global_data.end());
cout<<"is the arr sorted? : "<<(is_sorted_flag ? "Yes" :
"No")<<"\n";
}
MPI_Finalize();
return 0;
}