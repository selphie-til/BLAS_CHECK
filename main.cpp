#include <iostream>
#include <chrono>
#include "cblas.h"
#include <random>

int main() {
    std::printf("m-n-k,GFLOPS\n");
    auto A = new double[1024*1024];
    auto B = new double[1024*1024];
    auto C = new double[1024*1024];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int i=64; i<=1024; i+=16)
    {
        long long sum = 0;
        for (int j=0; j<10; ++j)
        {
            for (int k=0; k<i*i; ++k)
            {
                A[k] = dist(gen);
                B[k] = dist(gen);
                C[k] = dist(gen);
            }
            // 開始時刻を記録
            auto start = std::chrono::high_resolution_clock::now();
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, i, i, i, 1.0, A, i, B, i, 0.0, C, i);

            // 終了時刻を記録
            auto end = std::chrono::high_resolution_clock::now();

            // 経過時間を計算
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            // 結果を出力
            sum += duration.count();
        }
        double avg_time_seconds = (static_cast<double>(sum) / 10.0) / 1000000.0;  // 平均時間（秒）
        double operations = 2.0 * i * i * i;  // DGEMM演算回数
        double gflops = operations / avg_time_seconds / 1000000000.0;
        std::printf("%d,%f\n", i, gflops);
    }



    return 0;
}