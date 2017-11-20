#include "BucketSort.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <chrono>

/**
 * get the length of one number
 * @param  i number
 * @return  length of a number(how many digits)
 */
unsigned GetNumberOfDigits (unsigned int i)
{
    return i > 0 ? (int) log10 ((double) i) + 1 : 1;
}

/**
 * get the number of specific dight in a number
 * @param  i   number
 * @param  bit the index of a digit (from MSD)
 * @return   the number of specific digit
 */
signed GetIndex(unsigned int i, int bit) {
    int numOfDigit = GetNumberOfDigits(i);
    if (bit > numOfDigit - 1)
        return -1;
    else
        return i / static_cast<int>(pow(10, numOfDigit - 1 - bit)) % 10;
}

/**
 * get number of MSD
 * @param  i number
 * @return   number of MSD
 */
unsigned GetBucketIndex(unsigned int i) {
    return i/pow(10,static_cast<int>(log10(i)));
}

/**
 * recursive bucket sort
 */
std::vector<unsigned int> bucket_sort(std::vector<unsigned int> v, int left, int right, int bit)
{
    std::vector<unsigned int> result;
    std::vector<unsigned int> bucket[11];
    for(int i = left; i <= right; ++i) {              //divide into 11 buckets  
        int idx = GetIndex(v[i], bit);
        if (idx == -1)
            bucket[0].push_back(v[i]);
        else
            bucket[idx + 1].push_back(v[i]);
    }

    if (bucket[0].size() > 0) {     //don't have to sort the number with nothing in current digit eg: second MSD for 5
        std::move(std::begin(bucket[0]), std::end(bucket[0]), std::back_inserter(result));      
    }

    for(int i = 1; i < 11; ++i) {   //recursively sort rest of them by folloing
        if (bucket[i].size() > 0) {
            unsigned int temp = bucket[i][0];
            bool allSame = true;
            for (unsigned j = 0; j < bucket[i].size(); ++j) {
                if (bucket[i][j] != temp) {
                    std::vector<unsigned int> vec = bucket_sort(bucket[i], 0, bucket[i].size() - 1, bit + 1);
                    std::move(std::begin(vec), std::end(vec), std::back_inserter(result));      
                    allSame = false;
                    break;
                }
            }
            if (allSame) {  //if all number in a bucket are the small, put them back and terminate
                std::move(std::begin(bucket[i]), std::end(bucket[i]), std::back_inserter(result));      
            }
        }
        bucket[i].clear();
    }
    return result;
}

void BucketSort::sort(unsigned int numCores) {
    std::vector<std::thread> threads;           //define vector of thread for assigning
    size_t idx;
    for (unsigned int n : numbersToSort) {      //divide numbers into 10 buckets by MSD
        idx = GetBucketIndex(n);
        bucket[idx].push_back(n);
    }

    int digits = 9;
    std::mutex sortMutex;
    auto sort_push = [this, &sortMutex, &digits](unsigned int id) {     //assign each worker this sort buckets 
        int idx;
        while (true) {
            {
                std::lock_guard<std::mutex> guard(sortMutex);
                idx = digits;
                --digits;
            }
            if (idx < 1)
                break;
            auto temp = bucket_sort(bucket[idx], 0, bucket[idx].size() - 1, 1);
            bucket[idx].clear();
            std::move(std::begin(temp), std::end(temp), std::back_inserter(bucket[idx]));
            temp.clear();
        }
    };
    for (size_t i = 1; i < numCores; ++i) {
        threads.push_back(std::thread(sort_push, i));
    }
    sort_push(0);
    for (auto &t: threads) {
        t.join();
    }
    numbersToSort.clear();

    digits = 1;
    for (size_t i = 1; i < 10; ++i) {               //push all buckets back to numbersToSort vector
        std::move(std::begin(bucket[i]), std::end(bucket[i]), std::back_inserter(numbersToSort));
    }
}

