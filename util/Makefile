OBJ = 
BASEOBJ = ../netlib/base/Logging.o

testheap:$(OBJ) $(BASEOBJ) testheap.o
	g++ $(OBJ) $(BASEOBJ) testheap.o -o testheap

testheap.o:testheap.cpp
	g++ -c testheap.cpp

clean:
	rm *.o
