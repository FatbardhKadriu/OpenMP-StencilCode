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
#include <cmath>

using namespace std;

constexpr auto NR_ROWS = 120;
constexpr auto NR_COLS = 200;

float sequential_matrix [NR_ROWS][NR_COLS];
float parallel_matrix   [NR_ROWS][NR_COLS];

void execute_sequential();
void execute_parallel  ();
bool isSolutionCorrect ();

int main(int argc, char *argv[])
{
    for (int i = 0; i < NR_ROWS; i++)
      {
        sequential_matrix [i][0] = 150;
        parallel_matrix   [i][0] = 150;
      }
    for (int i = 0; i < NR_COLS; i++)
      {
        sequential_matrix [0][i] = 250;
        parallel_matrix   [0][i] = 250;
      }

    sequential_matrix [0][0] = 0;
    parallel_matrix   [0][0] = 0;

    execute_sequential();
    execute_parallel();

    if (isSolutionCorrect())
      {
        cout << "Solution is correct\n";
      } 
    else 
      {
        cout << "Solution is not correct\n";
      }

    return 0;
}

/***************************************************************************//** 
 * @brief Sequential execution
 *
 * This functions calculates matrix in the sequential way
 *
 * @retval void
 ******************************************************************************/
void execute_sequential()
  {
    for (int i = 1; i < NR_ROWS; i++)
      {
        for (int j = 1; j < NR_COLS; j++)
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
void execute_parallel()
  {
    for (int i = 2; i < NR_ROWS + NR_COLS - 1; i++)
      {
  #pragma omp parallel for num_threads(4)
        for (int j = 1; j < i; j++)
          {
            int row = i - j;
            if (row < NR_ROWS && j < NR_COLS)
              {
                parallel_matrix[row][j] = (fabs(sin(parallel_matrix[row][j - 1])) +
                                           fabs(sin(parallel_matrix[row - 1][j - 1])) +
                                           fabs(sin(parallel_matrix[row - 1][j]))) *
                                          100;
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
 * @retval void
 ******************************************************************************/
bool isSolutionCorrect()
  {
    bool solutionCorrect = true;
    for (int r = 0; r < NR_ROWS; r++)
      {
        for (int c = 0; c < NR_COLS; c++)
          {
            if (sequential_matrix[r][c] != parallel_matrix[r][c])
              {
                solutionCorrect = false;
              }
          }
      }

    return solutionCorrect;
  }
