#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>
#include <math.h>

using namespace std;

const size_t n = 3000;

vector<double> CreateRandomVector(size_t size)
{
    vector<double> res (size);
    for (size_t i = 0; i < size; i++)
    {
        res[i] = rand() % 100 + 1;
    }
    return res;
}

vector<double> MultiplyVector(vector<double> A, vector<double> X)
{
    vector<double> result (n);

    double sum = 0;

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            sum += X[j] * A[i*n+j];
        }
        result[i] = sum;
        sum = 0;
    }
    return result;
}

vector<double> SEQGauss(vector<double> a, vector<double> b)
{
    vector<double> x(n);
    double s;
    auto start = chrono::steady_clock::now();

    for (size_t z = 0; z < n-1; z++)
    {
        double p = a[z * n + z];
        for(int i = z + 1; i < n; i++)
        {
            double l = a[i*n+z] / p;
            for (size_t j = z; j < n; j++)
            {
                a[i * n + j] -= l * a[z*n + j];
            }
            b[i] -= l * b[z];
        }
    }
    for (int z = n - 1; z >= 0; z--)
    {
        s = 0;
        for (size_t i = z+1; i < n; i++)
        {
            s += a[z * n + i] * x[i];
        }
        x[z] = (b[z] - s) / a[z * n + z];
    }
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << endl << "Затрачено: " << elapsed.count() << "мс" << endl;
    return x;
}

vector<double> PARGauss(vector<double> a, vector<double> b)
{
    vector<double> x(n);
    double s;
    auto start = chrono::steady_clock::now();
    #pragma omp parallel
    {
        for (size_t z = 0; z < n-1; z++)
        {
            double p = a[z * n + z];
            #pragma omp   for
            for(int i = z + 1; i < n; i++)
            {
                double l = a[i*n+z] / p;
                for (size_t j = z; j < n; j++)
                {
                    a[i * n + j] -= l * a[z*n + j];
                }
                b[i] -= l * b[z];
            }
            #pragma omp barrier
        }
        for (int z = n - 1; z >= 0; z--)
        {
            s = 0;
           #pragma omp barrier
           #pragma omp for reduction(+ : s)
            for (size_t i = z+1; i < n; i++)
            {
                s += a[z * n + i] * x[i];
            }
           #pragma omp single
            x[z] = (b[z] - s) / a[z * n + z];
       }
    }
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << endl << "Затрачено: " << elapsed.count() << "мс" << endl;
    return x;
}

bool Compare(vector<double> a, vector<double> b)
{
    if(a.size() != b.size())
        return false;

    for (size_t i = 0; i < a.size(); i++)
    {
        if(round(a[i]) != round(b[i]))
            return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    vector<double> A = CreateRandomVector(n*n);     // Коэффициенты
    vector<double> X = CreateRandomVector(n);       // Неизвестные
    vector<double> B = MultiplyVector(A,X);         // Свободные члены
    
    vector<double> answSEQ = SEQGauss(A,B);
    cout << boolalpha << "\nx (rand) == x (gauss) ? " << Compare(X, answSEQ) << endl;

    vector<double> answPAR = PARGauss(A,B);
    cout << boolalpha << "\nx (rand) == x (gauss) ? " << Compare(X, answPAR) << endl;

    return 0;
}
