CXX=g++ -std=c++0x
CXXFLAGS=`pkg-config libgvc --cflags` -g -Wall $(EXTRA)
LDFLAGS=`pkg-config libgvc --libs`

build: simulation

simulation: main.cpp sim.cpp router.cpp 
	$(CXX) $(CXXFLAGS) -o $@ $^ -lapi -L . $(LDFLAGS)  -lgvc -lcgraph -lcdt

clean:
	rm -f -R simulation *.o
