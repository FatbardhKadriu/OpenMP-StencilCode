#include <iostream>
#include <cmath>

constexpr auto N = 4;

using namespace std;

int main() {

    float square_matrix[N][N];
    for (int i = 0; i < N; i++)
    {
        square_matrix[0][i] = 250;
        square_matrix[i][0] = 150;
    }
	square_matrix[0][0] = 0;

    for (int i = 1; i < N; i++)
    {
        for (int j = 1; j < N; j++)
        {
            square_matrix[i][j] = (fabs(sin(square_matrix[i - 1][j - 1])) +
                fabs(sin(square_matrix[i][j - 1])) +
                fabs(sin(square_matrix[i - 1][j]))) * 100;
        }
    }

    return 0;
}