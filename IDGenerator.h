#ifndef IDGENERATOR_H
#define IDGENERATOR_H

namespace myfs {
class IDGenerator {
public:
	static long long genId() { return id++; };
private:
	static long long id;
};

long long IDGenerator::id = 1;
}
#endif
