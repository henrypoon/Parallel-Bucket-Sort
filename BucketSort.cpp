#include "BucketSort.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>

unsigned BucketSort::GetBucketIndex(unsigned int i) {
    return i/pow(10,static_cast<int>(log10(i)));
}

void BucketSort::sort_bucket(size_t idx) {
    std::sort(bucket[idx].begin(), bucket[idx].end());
}

// TODO: replace this with a parallel version. 
void BucketSort::sort(unsigned int numCores) {
    std::mutex addMutex;
    std::vector<std::shared_ptr<std::thread>> threads;
    for (size_t i = 0; i != numCores; ++i) {
        threads.push_back(std::make_shared<std::thread>([this, &numCores, &addMutex](unsigned int id) {
            size_t idx;
            std::lock_guard<std::mutex> guard(addMutex);
            for (unsigned int n : numbersToSort) {
                idx = GetBucketIndex(n);
                // std::cout << idx % numCores << " hey" << std::endl;
                if (idx % numCores == id) {
                    bucket[idx].push_back(n);
                };
            }
        }, i));
    }


    for (auto t: threads) {
        t->join();
    }

    threads.clear();

    int digits = 9;
    std::mutex sortMutex;
    for (size_t i = 0; i != numCores; ++i) {
        threads.push_back(std::make_shared<std::thread>([this, &sortMutex, &digits]() {
            std::lock_guard<std::mutex> guard(sortMutex);
            while(digits >= 0) {
                // std::cout << digits << std::endl;
                int idx = digits;
                std::sort(bucket[idx].begin(), bucket[idx].end());
                --digits;
            }
        }));
    }

    for (auto t: threads) {
        t->join();
    }
    numbersToSort.clear();
    threads.clear();

    digits = 0;
    std::mutex putBackMutex;
    for (size_t i = 0; i != numCores; ++i) {
        threads.push_back(std::make_shared<std::thread>([this, &putBackMutex, &digits]() {
            std::lock_guard<std::mutex> guard(putBackMutex);
            while(digits < 10) {
                // std::cout << digits << std::endl;
                int idx = digits;
                std::move(std::begin(bucket[idx]), std::end(bucket[idx]), std::back_inserter(numbersToSort));
                bucket[idx].clear();
                ++digits;
            }
        }));
    }

    for (auto t: threads) {
        t->join();
    }

    threads.clear();
}



