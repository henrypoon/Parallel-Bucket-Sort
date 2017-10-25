#include "BucketSort.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <chrono>

unsigned GetNumberOfDigits (unsigned int i)
{
    return i > 0 ? (int) log10 ((double) i) + 1 : 1;
}

signed GetIndex(unsigned int i, int bit) {
    int numOfDigit = GetNumberOfDigits(i);
    if (bit > numOfDigit - 1)
        return -1;
    else
        return i / static_cast<int>(pow(10, numOfDigit - 1 - bit)) % 10;
}

std::vector<unsigned int> bucket_sort(std::vector<unsigned int> v, int left, int right, int bit)
{
    std::vector<unsigned int> result;
    std::vector<unsigned int> bucket[11];
    for(int i = left; i <= right; ++i) {
        int idx = GetIndex(v[i], bit);
        if (idx == -1)
            bucket[0].push_back(v[i]);
        else
            bucket[idx + 1].push_back(v[i]);
    }

    if (bucket[0].size() > 0) {
        std::move(std::begin(bucket[0]), std::end(bucket[0]), std::back_inserter(result));      
    }

    for(int i = 1; i < 11; ++i) {
        if (bucket[i].size() > 0) {
            // std::cout << i << " bucket_sort" << std::endl;
            // exit(0);
            
            int temp = bucket[i][0];
            bool allSame = true;
            for (int j = 0; j < bucket[i].size(); ++j) {
                if (bucket[i][j] != temp) {
                    std::vector<unsigned int> vec = bucket_sort(bucket[i], 0, bucket[i].size() - 1, bit + 1);
                    std::move(std::begin(vec), std::end(vec), std::back_inserter(result));      
                    allSame = false;
                    break;
                }
            }
            if (allSame) {
                std::move(std::begin(bucket[i]), std::end(bucket[i]), std::back_inserter(result));      
            }
        }
        // exit(0);
        bucket[i].clear();
    }
    return result;
}


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
    for (size_t i = 1; i != numCores; ++i) {
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
    for (size_t i = 1; i != numCores; ++i) {
        // std::cout << "sdfsfd " << i << std::endl;
        threads.push_back(std::make_shared<std::thread>([this, &sortMutex, &digits, &i]() {
            int idx;
            while (true) {
                std::lock_guard<std::mutex> guard(sortMutex);
                {
                    idx = digits;
                    --digits;
                }
                if (idx < 0)
                    break;
                std::thread::id this_id = std::this_thread::get_id();
                std::cout << "thread " << this_id << " is processing " << idx << " bucket" <<std::endl;

                auto temp = bucket_sort(bucket[idx], 0, bucket[idx].size() - 1, 1);
                bucket[idx].clear();
                std::move(std::begin(temp), std::end(temp), std::back_inserter(bucket[idx]));
                temp.clear();

                // std::sort(bucket[idx].begin(),bucket[idx].end(), [](const unsigned int& x, const unsigned int& y){
                //     return aLessB(x,y,0);
                // });
                // for (auto ite = bucket[2].begin(); ite != bucket[2].end(); ++ite) {
                //     std::cout << *ite << " ";
                // }
                
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
    for (size_t i = 1; i != numCores; ++i) {
        threads.push_back(std::make_shared<std::thread>([this, &putBackMutex, &digits, &i]() {
            int idx;
            while(true) {
                std::lock_guard<std::mutex> guard(putBackMutex);
                {
                    idx = digits;
                    // bucket[idx].clear();
                    ++digits;
                }
                if (idx > 9)
                    break;
                std::move(std::begin(bucket[idx]), std::end(bucket[idx]), std::back_inserter(numbersToSort));
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
    // std::sort(numbersToSort.begin(),numbersToSort.end(), [](const unsigned int& x, const unsigned int& y){
    //         return aLessB(x,y,0);
    // } );
    // for (auto ite = numbersToSort.begin(); ite != numbersToSort.end(); ++ite) {
    //     std::cout << *ite << " ";
    // }
    auto test = bucket_sort(numbersToSort, 0, numbersToSort.size() - 1, 0);
    numbersToSort.clear();
    std::move(std::begin(test), std::end(test), std::back_inserter(numbersToSort));
}


