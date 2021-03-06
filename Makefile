.PHONY: all
all: build

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_CXX_COMPILER=/usr/bin/g++ .. && \
	make


.PHONY: debug   
debug:
	mkdir -p build
	cd build  \
	cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ .. && \
	make

.PHONY: clean
clean:
	rm -rf build
