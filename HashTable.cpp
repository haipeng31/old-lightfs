#include "HashTable.h"
#include "INode.h"
#include <assert.h>
using namespace myfs;

//const int HashTable::kTableSize = 10007;

HashTable::HashTable()
 :  size_(0)
{

}

HashTable::~HashTable()
{
}

void HashTable::insert(string key, INode *node)
{
	int code = hashCode(key);
	if (table_[code].size() == 0) {
		table_[code].push_back(node);
	} else {
		/* make sure the key doesn't exist */
		assert(lookFor(table_[code], node->key()) == table_[code].end());

		table_[code].push_back(node);
	}
	size_++;
}

void HashTable::erase(string key)
{
	int code = hashCode(key);
	list<INode *>::iterator it = lookFor(table_[code], key);
	assert(it != table_[code].end());
	table_[code].erase(it);
	size_--;
}

INode *HashTable::search(string key)
{
	int code = hashCode(key);
	list<INode *>::iterator it = lookFor(table_[code], key);
	if (it != table_[code].end()) {
		return (*it);
	} else {
		return NULL;
	}
}

int HashTable::size()
{
	return size_;
}

list<INode *>::iterator HashTable::lookFor(list<INode *> &inodeList, string key)
{
	list<INode *>::iterator it;
	for (it = inodeList.begin(); it != inodeList.end(); it++) {
		if ((*it)->key() == key) {
			return it;
		}
	}

	return inodeList.end();
}

/*
 * BKDR hash function
 */
int HashTable::hashCode(string key)
{
	unsigned int seed = 131;
	unsigned int hash = 0;

	for (unsigned int i = 0; i < key.size(); i++) {
		hash = hash * seed + key[i];
	}

	return (hash & 0x7FFFFFFF) % kTableSize;
}

