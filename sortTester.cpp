#include <iostream>
#include <random>
#include <thread>
#include <cstring>
#include <string>
#include <fstream>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <cstdlib>

#include "BucketSort.h"

// std::vector<unsigned int> test = {48, 4, 2,2,454,2,1,10,24,1021,12,154};


// unsigned GetNumberOfDigits (unsigned i)
// {
//     return i > 0 ? (int) log10 ((double) i) + 1 : 1;
// }

// signed GetIndex(unsigned int i, int bit) {
// 	int numOfDigit = GetNumberOfDigits(i);
// 	if (bit > numOfDigit - 1)
// 		return -1;
// 	else
// 	    return i / static_cast<int>(pow(10, numOfDigit - 1 - bit)) % 10;
// }

// std::vector<unsigned int> sort(std::vector<unsigned int> v, int left, int right, int bit)
// {

//     std::vector<unsigned int> result;
//     std::vector<unsigned int> bucket[11];
//     for(int i = left; i <= right; ++i) {
//         int idx = GetIndex(v[i], bit);
//         if (idx == -1)
//             bucket[0].push_back(v[i]);
//         else
//             bucket[idx + 1].push_back(v[i]);
//     }

//     if (bucket[0].size() > 0) {
//         std::move(std::begin(bucket[0]), std::end(bucket[0]), std::back_inserter(result));      
//     }

//     for(int i = 1; i < 11; ++i) {
//         if (bucket[i].size() > 0) {
//             // exit(0);
            
//             int temp = bucket[i][0];
//             bool allSame = true;
//             for (int j = 0; j < bucket[i].size(); ++j) {
//                 if (bucket[i][j] != temp) {
//                     std::vector<unsigned int> vec = sort(bucket[i], 0, bucket[i].size() - 1, bit + 1);
//                     std::move(std::begin(vec), std::end(vec), std::back_inserter(result));      
//                    	allSame = false;
//                     break;
//                 }
//             }
//             if (allSame) {
//             	std::move(std::begin(bucket[i]), std::end(bucket[i]), std::back_inserter(result));      
//             }
//         }
//         // exit(0);
//         bucket[i].clear();
//     }
//     return result;
// }


int main() {
	
	// auto v = sort(test, 0, test.size() - 1, 0);

	// for (auto ite = v.begin(); ite != v.end(); ++ite) {
	// 	std::cout << *ite << " " << std::endl;
	// }

	unsigned int totalNumbers =	500000;
	unsigned int printIndex =	259000;
	
	// use totalNumbers required as the seed for the random
	// number generator. 
	std::mt19937 mt(totalNumbers);
	std::uniform_int_distribution<unsigned int> dist(1, std::numeric_limits<unsigned int>::max());

	// create a sort object
	BucketSort pbs;
	// BucketSort single_test;

	// std::ofstream ofs1("out1");
	// std::ofstream ofs2("out2");

	// insert random numbers into the sort object
	for (unsigned int i=0; i < totalNumbers; ++i) {
		pbs.numbersToSort.push_back(dist(mt));
	} 


	// for (size_t i = 0; i < totalNumbers; ++i) {
		// single_test.numbersToSort.push_back(pbs.numbersToSort.at(i));
	// }

	// call sort giving the number of cores available.
    const unsigned int numCores = std::thread::hardware_concurrency();
    pbs.sort(numCores);
    // single_test.single_sort(numCores);

	// std::copy(pbs.numbersToSort.begin(), pbs.numbersToSort.end(), std::ostream_iterator<unsigned int>(ofs1, " "));
	// std::copy(single_test.numbersToSort.begin(), single_test.numbersToSort.end(), std::ostream_iterator<unsigned int>(ofs2, " "));

    std::cout << "number of cores used: " << numCores << std::endl;
	
	// print certain values from the buckets
	std::cout << "Demonstrating that all the numbers that start with 1 come first" << std::endl;
	std::cout << pbs.numbersToSort[0] << " " << pbs.numbersToSort[printIndex - 10000]
		<< " " << pbs.numbersToSort[printIndex] << " " << pbs.numbersToSort[pbs.numbersToSort.size() - 1] 
		<< std::endl;
	





	// std::cout << "Demonstrating that all the numbers that start with 1 come first" << std::endl;
	// std::cout << single_test.numbersToSort[0] << " " << single_test.numbersToSort[printIndex - 10000]
	// 	<< " " << single_test.numbersToSort[printIndex] << " " << single_test.numbersToSort[single_test.numbersToSort.size() - 1] 
	// 	<< std::endl;


	// BucketSort b;
	// // b.numbersToSort.push_back(0);
	// // b.numbersToSort.push_back(100);
	// b.numbersToSort.push_back(3);
	// // b.numbersToSort.push_back(65);
	// b.numbersToSort.push_back(25);
	// b.numbersToSort.push_back(2321);
	// b.numbersToSort.push_back(2);
	// // b.numbersToSort.push_back(254654);
	// b.numbersToSort.push_back(8);
	// b.numbersToSort.push_back(888);
	// b.numbersToSort.push_back(4545);
	// // b.numbersToSort.push_back(9);
	// // b.numbersToSort.push_back(95);


	// b.sort(4);

	// for (size_t i = 0; i < 10; ++i) {
	// 	std::cout << i << " bucket has: ";
	// 	if (b.bucket[i].size() > 0) {
	// 		for (auto ite = b.bucket[i].begin(); ite != b.bucket[i].end(); ++ite) {
	// 			std::cout << *ite << " ";
	// 		}
	// 	}
	// 	std::cout << std::endl;
	// }

	// std::cout << "The whole buckets has: ";
	// for (auto ite = b.numbersToSort.begin(); ite != b.numbersToSort.end(); ++ite) {
	// 	std::cout << *ite << " ";
	// }
	// std::cout << std::endl;
	// int data[] = { 170, 45, 75, -90, -802, 24, 2, 66 };
	// std::vector<int> v = {1, 2};

	// sort(v.begin(), v.end());

	// for (auto ite = v.begin(); ite != v.end(); ++ite) {
	// 	std::cout << *ite << std::endl;
		
	// }
	

}