#include "Heap.h"
#include "../netlib/base/Logging.h"
#include <vector>
using std::vector;

using namespace myfs;

int main()
{
	//Heap<int> testHeap;
	int array[10] = { 5, 1, 90, 0, -1, 43, 77, 33, 98, 100 };
	vector<int> ivec;
	for (int i = 0; i < 10; i++) {
		//testHeap.insert(array[i]);
		ivec.push_back(array[i]);
	}
	
	Heap<int> testHeap(ivec);
	LOG_TRACE << "Elements in the heap are " << testHeap;
	for (int i = 0; i < 10; i++) {
		int rt = testHeap.extract();
		LOG_TRACE << "Extract value " << rt;
	}
}
