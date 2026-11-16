#include <iostream>
#include <omp.h>
using namespace std;

int IsPrime(int number) {
    if (number < 2)
        return 0;

    for (int i = 2; i < number; i++) {
        if (number % i == 0)
            return 0;
    }
    return 1;
}

int main() {
    int noOfThreads, valueN, indexCount = 0;

    cout << "Enter the number of threads: ";
    cin >> noOfThreads;

    cout << "Enter the value of N: ";
    cin >> valueN;

    omp_set_num_threads(noOfThreads);

    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:indexCount)
    for (int tempValue = 2; tempValue <= valueN; tempValue++) {
        if (IsPrime(tempValue)) {
            indexCount++;
        }
    }

    double end_time = omp_get_wtime();

    cout << "Number of prime numbers between 2 and "
         << valueN << ": " << indexCount << endl;

    cout << "Time taken: " << end_time - start_time
         << " seconds" << endl;

    return 0;
}