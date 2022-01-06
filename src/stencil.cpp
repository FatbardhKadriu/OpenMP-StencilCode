#include <omp.h>
#include <iostream>
#include <cmath>

constexpr auto N = 20;

using namespace std;

int main(int argc, char *argv[])
{
    float sequential_matrix[N][N];
    float parallel_matrix[N][N];
    for (int i = 0; i < N; i++)
    {
        sequential_matrix[0][i] = 250;
        sequential_matrix[i][0] = 150;
        parallel_matrix[0][i] = 250;
        parallel_matrix[i][0] = 150;
    }
    sequential_matrix[0][0] = 0;
    parallel_matrix[0][0] = 0;

    for (int i = 1; i < N; i++)
    {
        for (int j = 1; j < N; j++)
        {
            sequential_matrix[i][j] = (fabs(sin(sequential_matrix[i - 1][j - 1])) +
                                       fabs(sin(sequential_matrix[i][j - 1])) +
                                       fabs(sin(sequential_matrix[i - 1][j]))) *
                                      100;
        }
    }

    for (int i = 2; i < N * 2; i++)
    {
#pragma omp parallel for num_threads(4)
            for (int j = 1; j < i; j++)
            {
                int row = i - j;
                if (row < N && j < N)
                {
                    parallel_matrix[row][j] = (fabs(sin(parallel_matrix[row][j - 1])) +
                                               fabs(sin(parallel_matrix[row - 1][j - 1])) +
                                               fabs(sin(parallel_matrix[row - 1][j]))) *
                                              100;
                }
            }
    }

    bool solutionValid = true;
    for (int k = 0; k < N; k++)
    {
        for (int l = 0; l < N; l++)
        {
            if (sequential_matrix[k][l] != parallel_matrix[k][l])
            {
                solutionValid = false;
            }
        }
    }

    if (solutionValid)
    {
        cout << "Solution is valid." << endl;
    }
    else
    {
        cout << "Wrong!! Matrices are not equal." << endl;
    }

    return 0;
}