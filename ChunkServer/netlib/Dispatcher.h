#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>
using std::map;
namespace mynet {

class Dispatcher {
public:
	typedef void *(*MessageCallback)(void *, const void *, int, int *);
	void registerMsgCallback(int, MessageCallback, void *);
	static void *dispatchMessage(void *, const void *, int, int *);
private:
	
	struct CallbackStruct {
		MessageCallback messageCallback;
		void *objArg;
	};
	
	void *dispatchMessage(const void *, int, int *);
	typedef map<int, CallbackStruct *> CallbackMap;
	CallbackMap messageCallbacks_;
};
}
#endif
