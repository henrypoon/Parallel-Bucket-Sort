#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>

struct BucketSort {
	
	// vector of numbers
	std::vector<unsigned int> numbersToSort;

	std::vector<unsigned int> bucket[10];

	unsigned GetBucketIndex(unsigned i);
	void sort(unsigned int numCores);
	void sort_bucket(size_t idx);
};

#endif /* BUCKET_SORT_H */