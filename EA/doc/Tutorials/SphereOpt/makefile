CPP := $(shell find . -name "*.cpp")

all: libopenea_SO.so

libopenea_SO.so: $(CPP)
	g++ -std=c++14 -shared -fPIC -O3 -o libopenea_SO.so $(CPP) -lopenea

clean:
	rm -f libopenea_SO.so
