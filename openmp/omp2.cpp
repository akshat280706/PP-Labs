#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int x = 0;
    int i = 0;

    #pragma omp parallel shared(i, x)
    {
        if (omp_get_thread_num() == 0) {
            cout << "Master Thread with id: "
                 << omp_get_thread_num() << endl;

            cout << "Producer is adding data to the buffer" << endl;

            i += 10;

            #pragma omp atomic write
            x = 1;
        }
        else {
            int ready = 0;

            while (ready == 0) {
                #pragma omp atomic read
                ready = x;
            }

            #pragma omp critical
            {
                if (i > 0) {
                    cout << "Data consumed by thread with id: "
                         << omp_get_thread_num() << endl;
                    i -= 5;
                }
                else {
                    cout << "Couldn't find data for thread with id: "
                         << omp_get_thread_num() << endl;
                }
            }
        }
    }

    return 0;
}