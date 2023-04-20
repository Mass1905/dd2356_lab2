//
// Created by daybeha on 23-4-10.
//
#include <iostream>
#include <omp.h>
#include <chrono>
#include <vector>

using namespace std;


int main(){
//    int nthreads, tid;
//    double a[100], b[100], c[100], d[100];
//
//    /* Some initializations */
//    for (int i=0; i < 100; i++){
//        a[i] = b[i] = i * 1.0;
//        c[i] = d[i] = 0.0;
//    }
//
//
//    #pragma omp parallel default(none) shared(a,b,c,d,nthreads, cout) private(tid)
//    {
//        tid = omp_get_thread_num();
//        if (tid == 0) {
//            nthreads = omp_get_num_threads();
//            cout <<"Starting matrix addition with " << nthreads << " threads"<< endl;
//        }
//        cout <<"Thread " << tid << " starting matrix addition..." << endl;
//
//        #pragma omp for schedule(static,10)
//        for (int i=0; i < 100; i++) {
//            c[i] = a[i] + b[i];
//            cout <<"Thread " << tid << " did row " << i << endl;
//        }
//
//        #pragma omp for schedule(dynamic,10)
//        for (int i=0; i < 100; i++) {
//            d[i] = a[i] + b[i];
//            cout <<"Thread " << tid << " did row " << i << endl;
//        }
//    }  /* end of parallel section */

    #pragma omp parallel default(none) shared(cout)  num_threads(4)  // This is the parallel region
    {
        int ID = omp_get_thread_num();
        int num = omp_get_num_threads();
        cout << "Hello World " << ID <<  " from " << num << endl;
    }

//    int x[12800];
    vector<float> x(100);
    vector<float> b(100);
    float sum= 0.0;
    for (int i = 0; i < 100; ++i) {
        x[i] = i+1.0;
    }
    cout << endl << "chrono:" << endl;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::cout << "Start  " << start.time_since_epoch().count() << " s"<< endl;
    #pragma omp parallel for  default(none)  shared(x) num_threads(4) reduction(+:sum)
        for (int i = 0; i < 100; ++i) {
            sum += x[i] - 1.0;
        }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "End  " << end.time_since_epoch().count() << " s"<< endl;
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time used:  " << diff.count() << " s"<< endl;


    cout << sum << endl;

    return 0;
}