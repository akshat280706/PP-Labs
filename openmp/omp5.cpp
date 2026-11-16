#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <climits>
using namespace std;

int main() {
    srand(time(NULL));

    int numberOfElements, currentMax = INT_MIN;
    int arrayInput[100000];

    cout << "Enter the number of elements: ";
    cin >> numberOfElements;

    if (numberOfElements <= 0 || numberOfElements > 100000)
        return 1;

    for (int i = 0; i < numberOfElements; i++) {
        if (numberOfElements < 10)
            cin >> arrayInput[i];
        else
            arrayInput[i] = rand() % numberOfElements;
    }

    omp_set_num_threads(16);

    double start_time = omp_get_wtime();

    #pragma omp parallel for shared(currentMax)
    for (int i = 0; i < numberOfElements; i++) {
        #pragma omp critical
        {
            if (arrayInput[i] > currentMax) {
                currentMax = arrayInput[i];
            }
        }
    }

    double end_time = omp_get_wtime();

    cout << "The Maximum Element is: " << currentMax << endl;

    cout << "Time taken: " << end_time - start_time
         << " seconds" << endl;

    return 0;
}