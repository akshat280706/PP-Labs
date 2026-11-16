#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main() {
    int i, j, k;
    int m, n, p;

    cout << "Enter number of rows in Matrix A: ";
    cin >> m;

    cout << "Enter number of columns in Matrix A: ";
    cin >> n;

    cout << "Enter number of columns in Matrix B: ";
    cin >> p;

    vector<vector<int>> matrixA(m, vector<int>(n));
    vector<vector<int>> matrixB(n, vector<int>(p));
    vector<vector<int>> matrixC_seq(m, vector<int>(p, 0));
    vector<vector<int>> matrixC_par(m, vector<int>(p, 0));

    cout << "Enter values for Matrix A:\n";
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            cin >> matrixA[i][j];
        }
    }

    cout << "Enter values for Matrix B:\n";
    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            cin >> matrixB[i][j];
        }
    }

    // Sequential multiplication
    double start_seq = omp_get_wtime();

    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            for (k = 0; k < n; k++) {
                matrixC_seq[i][j] +=
                    matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    double end_seq = omp_get_wtime();
    double sequential_time = end_seq - start_seq;

    // Parallel multiplication
    double start_par = omp_get_wtime();

    #pragma omp parallel private(i, j, k) shared(matrixA, matrixB, matrixC_par)
    {
        #pragma omp for schedule(static)
        for (i = 0; i < m; i++) {
            for (j = 0; j < p; j++) {
                for (k = 0; k < n; k++) {
                    matrixC_par[i][j] +=
                        matrixA[i][k] * matrixB[k][j];
                }
            }
        }
    }

    double end_par = omp_get_wtime();
    double parallel_time = end_par - start_par;

    // Print result
    cout << "\nResult Matrix:\n";

    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            cout << matrixC_par[i][j] << "\t";
        }
        cout << endl;
    }

    // Compare execution times
    cout << "Sequential Execution Time: "
         << sequential_time << " seconds" << endl;

    cout << "Parallel Execution Time: "
         << parallel_time << " seconds" << endl;

    cout << "Speedup: "
         << sequential_time / parallel_time << endl;

    return 0;
}