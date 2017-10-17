#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>

struct BucketSort {
	
	// vector of numbers
	std::vector<unsigned int> numbersToSort;
	std::vector<unsigned int> bucket[10];
	void sort(unsigned int numCores);
	void single_sort(unsigned int numCores);
};

#endif /* BUCKET_SORT_H */