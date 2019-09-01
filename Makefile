CXX			= gpp
RM			= del

CXXFLAGS	+= -O0 -std=c++14 -march=i686 -DNDEBUG -m32 -fpermissive -DCpcDos -I .


all: ExeLoader.obj MemoryModule.obj

%.obj: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	$(RM) *.obj

re: clean all

.PHONY: all clean re