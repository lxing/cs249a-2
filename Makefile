CXXFLAGS = -Wall -g

OBJECTS = Instance.o

default:	test1 example engine

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

engine:	Engine.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h NamedInterface.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h NamedInterface.h
Engine.o: Engine.cpp Instance.h PtrInterface.h Ptr.h NamedInterface.h Engine.h
