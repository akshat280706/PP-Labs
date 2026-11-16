#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int num_steps = 10000000, i;
    double aux, pi;
    double step = 1.0 / (double)num_steps;
    double x = 0.0, sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel private(i, x, aux) shared(sum)
    {
        #pragma omp for schedule(static)
        for (i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            aux = 4.0 / (1.0 + x * x);

            #pragma omp critical
            {
                sum = sum + aux;
            }
        }
    }

    pi = step * sum;

    double end_time = omp_get_wtime();

    cout << "The Value of PI is: " << pi << endl;
    cout << "Time taken: " << end_time - start_time
         << " seconds" << endl;

    return 0;
}