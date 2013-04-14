#ifndef DIRENT_H
#define DIRENT_H

#include <string>
using std::string;

namespace myfs {
class Dirent {
public:
	string name;
	int type;
};
}
#endif
