#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <iostream>
#include "Logging.h"
#include <cmath>           // for sqrt()
#include <assert.h>	       // for assert()
using std::vector;
using std::ostream;

namespace mynet {

/*
template <typename T>
class Heap;

template <typename T>
ostream &oeprator<< <T>(ostream &os, const Heap<T> &heap);
*/
template <typename T>
class Heap {
public:
	Heap();
	Heap(vector<T> &vec);
	void insert(const T &value);
	T extract();
	T min();
	void update(int index, const T &value);
	int size();

	/* for debug */
	template <typename D>
	friend ostream &operator<< (ostream &os, const Heap<D> &heap);
#if 0
	{
		for (int i = 0; i < heap.vec_.size(); i++) {
			os << heap.vec_[i] << " ";
		}

		return os;
	}
#endif
	
private:
	void minHeapify(int index);
	int left(int index);
	int right(int index);
	int parent(int index);
	void swap(int, int);
	vector<T> vec_;
};

template <typename T>
Heap<T>::Heap()
{
}

template <typename T>
Heap<T>::Heap(vector<T> &vec)
 :  vec_(vec)
{
#ifdef TRACE
	LOG_TRACE << "create heap from a vector";
#endif
	/* end == log(size)'s upper bound */
	int end, size;
	size = vec.size();
	if ((size & (size - 1)) == 0) {
		end = sqrt(size);
	} else {
		end = sqrt(size) + 1;
	}

	for (int i = end; i >= 0; i--) {
		minHeapify(i);
	}
}

template <typename T>
void Heap<T>::insert(const T &value)
{
#ifdef TRACE
	LOG_TRACE << "insert" << value;
#endif
	vec_.push_back(value);

	int i = vec_.size() - 1;
	int p = parent(i);
	while ((p >= 0) && (vec_[i] < vec_[p])) {
		swap(i, p);
		i = p;
		p = parent(i);
	}
}

template <typename T>
T Heap<T>::extract()
{
#ifdef TRACE
	LOG_TRACE << "extract min value";
#endif
	assert(vec_.size() > 0);
	T rt = vec_[0];

	/* put the last element on position 0 */
	swap(0, vec_.size() - 1);
	vec_.pop_back();
	minHeapify(0);
	return rt;
}

template <typename T>
T Heap<T>::min()
{
	assert(vec_.size() > 0);
	return vec_[0];
}

template <typename T>
void Heap<T>::update(int index, const T &value)
{
#ifdef TRACE
	LOG_TRACE << "update ele at position index to " << value;
#endif
	assert((index >= 0) && (index < vec_.size()));

	if (value < vec_[index]) {
		int i = index;
		int p = parent(i);
		while ((p >= 0) && (vec_[i] < vec_[p])) {
			swap(i, p);
			i = p;
			p = parent(i);
		}
	} else if (value > vec_[index]) {
		minHeapify(index);
	} else {
		// do nothing
	}
}

template <typename T>
int Heap<T>::size()
{
	return vec_.size();
}

template <typename T>
void Heap<T>::minHeapify(int index)
{
	int i, l, r, min;
	i = index;

	while (1) {
		min = i;
		l = left(i);
		if (l >= vec_.size()) {
			break;
		}
		if (vec_[min] > vec_[l]) {
			min = l;
		}

		r = right(i);
		if (r >= vec_.size()) {
			break;
		}
		if (vec_[min] > vec_[r]) {
			min = r;
		}

		if (min == i) {
			break;
		} else {
			swap(i, min);
			i = min;
		}
	}
}

template <typename T>
int Heap<T>::left(int index)
{
	return 2 * index + 1;
}

template <typename T>
int Heap<T>::right(int index)
{
	return 2 * (index + 1); 
}

template <typename T>
int Heap<T>::parent(int index)
{
	if (index == 0) {
		return -1;
	} else {
		return (index - 1) / 2;
	}
}

template <typename T>
void Heap<T>::swap(int l, int r)
{
	assert(l >= 0 && l < vec_.size());
	assert(r >= 0 && r < vec_.size());
	T temp = vec_[l];
	vec_[l] = vec_[r];
	vec_[r] = temp;
}


template <typename T>
ostream &operator<<(ostream &os, const Heap<T> &heap)
{
	for (int i = 0; i < heap.vec_.size(); i++) {
		os << heap.vec_[i] << " ";
	}

	return os;
}

}
#endif

