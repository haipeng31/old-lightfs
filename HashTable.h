#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <list>
#include <string>
using std::list;
using std::string;

namespace myfs {

class INode;

class HashTable {

public:
	HashTable();
	~HashTable();

	void insert(string, INode *);
	void erase(string);
	INode *search(string);
	int size();

private:
	list<INode *>::iterator lookFor(list<INode *> &, string);
	int hashCode(string);
	static const int kTableSize = 10007;
	list<INode *> table_[kTableSize];
	int size_;
};
}
#endif
