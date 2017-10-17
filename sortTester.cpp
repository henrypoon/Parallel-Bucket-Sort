#include <iostream>
#include <random>
#include <thread>
#include <string>
#include <fstream>

#include "BucketSort.h"

int main() {
	
	unsigned int totalNumbers =	500000;
	unsigned int printIndex =	259000;
	
	// use totalNumbers required as the seed for the random
	// number generator. 
	std::mt19937 mt(totalNumbers);
	std::uniform_int_distribution<unsigned int> dist(1, std::numeric_limits<unsigned int>::max());

	// create a sort object
	BucketSort pbs;
	BucketSort single_test;

	std::ofstream ofs1("out1");
	std::ofstream ofs2("out2");

	// insert random numbers into the sort object
	for (unsigned int i=0; i < totalNumbers; ++i) {
		pbs.numbersToSort.push_back(dist(mt));
	} 


	for (size_t i = 0; i < totalNumbers; ++i) {
		single_test.numbersToSort.push_back(pbs.numbersToSort.at(i));
	}

	// call sort giving the number of cores available.
    const unsigned int numCores = std::thread::hardware_concurrency();
    pbs.sort(numCores);
    single_test.single_sort(numCores);
    
	std::copy(pbs.numbersToSort.begin(), pbs.numbersToSort.end(), std::ostream_iterator<unsigned int>(ofs1, " "));
	std::copy(single_test.numbersToSort.begin(), single_test.numbersToSort.end(), std::ostream_iterator<unsigned int>(ofs2, " "));

    std::cout << "number of cores used: " << numCores << std::endl;
	
	// print certain values from the buckets
	std::cout << "Demonstrating that all the numbers that start with 1 come first" << std::endl;
	std::cout << pbs.numbersToSort[0] << " " << pbs.numbersToSort[printIndex - 10000]
		<< " " << pbs.numbersToSort[printIndex] << " " << pbs.numbersToSort[pbs.numbersToSort.size() - 1] 
		<< std::endl;
	
	std::cout << "Demonstrating that all the numbers that start with 1 come first" << std::endl;
	std::cout << single_test.numbersToSort[0] << " " << single_test.numbersToSort[printIndex - 10000]
		<< " " << single_test.numbersToSort[printIndex] << " " << single_test.numbersToSort[single_test.numbersToSort.size() - 1] 
		<< std::endl;


	// BucketSort b;
	// b.numbersToSort.push_back(0);
	// b.numbersToSort.push_back(100);
	// b.numbersToSort.push_back(3);
	// b.numbersToSort.push_back(65);
	// b.numbersToSort.push_back(2121);
	// b.numbersToSort.push_back(21);
	// b.numbersToSort.push_back(2);
	// b.numbersToSort.push_back(254654);
	// b.numbersToSort.push_back(8);
	// b.numbersToSort.push_back(888);
	// b.numbersToSort.push_back(4545);
	// b.numbersToSort.push_back(9);
	// b.numbersToSort.push_back(95);


	// b.single_sort(4);

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
}