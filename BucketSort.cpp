#include "BucketSort.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>


bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {    
    if (x == y) return false; // if the two numbers are the same then one is not less than the other
    unsigned int a = x;
    unsigned int b = y;
    // work out the digit we are currently comparing on. 
    if (pow == 0) {
        while (a / 10 > 0) {
            a = a / 10; 
        }   
        while (b / 10 > 0) {
            b = b / 10;
        }
    } else {
        while (a / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
            a = a / 10;
        }
        while (b / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
            b = b / 10;
        }
    }

    if (a == b)
        return aLessB(x,y,pow + 1);  // recurse if this digit is the same 
    else
        return a < b;
}

unsigned GetBucketIndex(unsigned int i) {
    return i/pow(10,static_cast<int>(log10(i)));
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
                int idx = digits;
                // std::sort(bucket[idx].begin(), bucket[idx].end());
                std::sort(bucket[idx].begin(),bucket[idx].end(), [](const unsigned int& x, const unsigned int& y){
                    return aLessB(x,y,0);
                });
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
                // bucket[idx].clear();
                ++digits;
            }
        }));
    }
    for (auto t: threads) {
        t->join();
    }
    threads.clear();
}


// TODO: replace this with a parallel version. 
void BucketSort::single_sort(unsigned int numCores) {
        std::sort(numbersToSort.begin(),numbersToSort.end(), [](const unsigned int& x, const unsigned int& y){
                return aLessB(x,y,0);
        } );
}


