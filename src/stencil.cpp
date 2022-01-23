/**
 * Copyright (C) 2021 Arbena Musa, Fatbardh Kadriu. All rights reserved
 *
 * @file  stencil.cpp
 *
 * @brief Implements the main functionality
 * 
 * @authors Arbena Musa, Fatbardh Kadriu ({arbena.musa, fatbardh.kadriu}@student.uni-pr.edu)
 * @date   $Date: Thur, Jan 06, 2022$
 */
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>

using namespace std;

/* defines the number of threads for parallel execution */
int threads[5]    = { 2, 4, 8, 16, 32 };
/* defines the matrix dimensions */
int dimensions[8] = { 360, 720, 1440, 2880, 5760, 11520, 23040, 46080 };

float **sequential_matrix;
float **parallel_matrix  ;

void initialize_matrices (int nr_rows, int nr_cols);
void execute_sequential  (int nr_rows, int nr_cols);
void execute_parallel    (int nr_rows, int nr_cols, int threads);
bool isSolutionCorrect   (int nr_rows, int nr_cols);

int main(int argc, char *argv[])
{
    for(int t : threads)
      {
        cout << "Threads: " << t << endl;
        for(int d : dimensions)
          {
            cout << "Matrix dimensions: [" << d << "][" << d << "]" << endl;
            initialize_matrices(d, d);

            auto start_time_sequential = chrono::high_resolution_clock::now();
            execute_sequential(d, d);
            auto end_time_sequential = chrono::high_resolution_clock::now();

            float elapsed_time_sequential = chrono::duration<float, std::milli>
                                                    (end_time_sequential - start_time_sequential).count();

            cout << "Time measured (sequential): " 
                << int (elapsed_time_sequential) 
                << " milliseconds" << endl;

            auto start_time_parallel = chrono::high_resolution_clock::now();
            execute_parallel(d, d, t);
            auto end_time_parallel = chrono::high_resolution_clock::now();
            
            float elapsed_time_parallel = chrono::duration<float, std::milli>
                                                    (end_time_parallel - start_time_parallel).count();
            cout << "Time measured   (parallel): " 
                << int (elapsed_time_parallel) 
                << " milliseconds" << endl;

            float speedup = (float)(elapsed_time_sequential / elapsed_time_parallel);

            cout << "Checking for solution correctness..." << endl;
            if (isSolutionCorrect(d, d))
              {
                cout << "Solution is correct" << endl;
                cout << "Speedup: " << setprecision(3) << speedup << endl;
              } 
            else 
              {
                cout << "Solution is not correct\n";
              }
          }
      }

    return 0;
}

/***************************************************************************//** 
 * @brief Matrices initialization
 *
 * This functions initialize matrices in which calculation are performed
 *
 * @retval void
 ******************************************************************************/
void initialize_matrices(int nr_rows, int nr_cols)
  {
    sequential_matrix = new float *[nr_rows];
    parallel_matrix   = new float *[nr_rows];

    for (int i = 0; i < nr_rows; i++)
      {
        float *seq_row  = new float[nr_cols];
        float *par_row = new float[nr_cols];
        sequential_matrix[i] = seq_row ;
        parallel_matrix  [i] = par_row;

        sequential_matrix [i][0] = 150;
        parallel_matrix   [i][0] = 150;
      }

    for (int i = 0; i < nr_cols; i++)
      {
        sequential_matrix [0][i] = 250;
        parallel_matrix   [0][i] = 250;
      }

    sequential_matrix [0][0] = 0;
    parallel_matrix   [0][0] = 0;
  }

/***************************************************************************//** 
 * @brief Sequential execution
 *
 * This functions calculates matrix in the sequential way
 *
 * @retval void
 ******************************************************************************/
void execute_sequential(int nr_rows, int nr_cols)
  {
    for (int i = 1; i < nr_rows; i++)
      {
        for (int j = 1; j < nr_cols; j++)
          {
            sequential_matrix[i][j] = (fabs(sin(sequential_matrix[i - 1][j - 1])) +
                                       fabs(sin(sequential_matrix[i][j - 1])) +
                                       fabs(sin(sequential_matrix[i - 1][j]))) *
                                      100;
          }
      }
  }


/***************************************************************************//** 
 * @brief Parallel execution
 *
 * This functions calculates matrix in the parallel way
 *
 * @retval void
 ******************************************************************************/
void execute_parallel(int nr_rows, int nr_cols, int threads)
  {
    // iterate through diagionals
    for (int i = 2; i < nr_rows + nr_cols - 1; i++)
      {
  #pragma omp parallel for num_threads(threads)
        for (int j = 1; j < i; j++)
          {
            int row = i - j;
            if (row < nr_rows && j < nr_cols)
              {
                parallel_matrix[row][j] = (fabs(sin(parallel_matrix[row    ][j - 1]))  +
                                           fabs(sin(parallel_matrix[row - 1][j - 1]))  +
                                           fabs(sin(parallel_matrix[row - 1][j    ]))) * 100;
              }
          }
      }
  }


/***************************************************************************//** 
 * @brief Check for solution correctness
 *
 * This functions check two matrices that are generated from sequential execution
 * and parallel exectuion if they are equal
 *
 * @retval true if the solution is correct
 * @retval false otherwise
 ******************************************************************************/
bool isSolutionCorrect(int nr_rows, int nr_cols)
  {
    bool solutionCorrect = true;
    for (int r = 1; r < nr_rows; r++)
      {
        for (int c = 1; c < nr_cols; c++)
          {
            if (sequential_matrix[r][c] != parallel_matrix[r][c])
              {
                solutionCorrect = false;
              }
          }
      }

    return solutionCorrect;
  }
