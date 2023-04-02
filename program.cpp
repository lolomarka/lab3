#include <iostream>
#include <chrono>
#include <omp.h>

using namespace std;

size_t** CreateMatrix(size_t n, size_t m)
{
    size_t** arr = new size_t*[n];
    for (size_t i = 0; i < n; i++)
    {
        arr[i] = new size_t[m];
        for (size_t j = 0; j < m; j++)
        {
            arr[i][j] = rand() % 100 + 1;
        }
    }
    return arr;    
}

void PrintMatrix(size_t** arr, int n, int m)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            cout << arr[i][j] << "\t";
        }
        cout << endl;        
    }
}

int main(int argc, char const *argv[])
{
    size_t n1 = 1000;
    size_t m1 = 1000;
    
    size_t n2 = 1000;
    size_t m2 = 1000;

    size_t** A = CreateMatrix(n1, m1);
    size_t** B = CreateMatrix(n2, m2);
    // size_t** C;

    PrintMatrix(A, n1,m1);
    PrintMatrix(B, n2, m2);

    return 0;
}
