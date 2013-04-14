#include "DirTree.h"
#include "INode.h"
#include <list>
#include <iostream>
#include <sstream>
using std::list;
using std::cout;
using std::stringstream;
using namespace myfs;

int main()
{
	DirTree tree;
	tree.addDir("/", "haipeng31");
	tree.addDir("/", "linzhenze");

	tree.addFile("/haipeng31", "fileA");
	tree.addFile("/haipeng31", "fileB");
	
	for (int i = 0; i < 1000; i++) {
		stringstream ss;
		ss << i;
		tree.addFile("/haipeng31", ss.str());
	}
	list<INode *> children = tree.readDir("/haipeng31");
	
	list<INode *>::iterator it;
	for (it = children.begin(); it != children.end(); it++) {
		cout << (*it)->key() << "\n";
	}
	
	for (int i = 0; i < 500; i++) {
		stringstream ss;
		ss << i;
		tree.delFile("/haipeng31", ss.str());
	}
	
	children = tree.readDir("/haipeng31");
	
	for (it = children.begin(); it != children.end(); it++) {
		cout << (*it)->key() << "\n";
	}
	return 0;
}
