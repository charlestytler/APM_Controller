
DEPS = state_estimation \
	   global_data \
	   linear_algebra

C_DEPS = $(DEPS:=.cpp)
H_DEPS = $(DEPS:=.h)
O_DEPS = $(DEPS:=.o)

CFLAGS=-D OFF_TARGET_COMPILE

all: state_estimation.so

state_estimation.so: $(O_DEPS)
	g++ -shared $(CFLAGS) $^ -o $@
	rm $(shell find *.o)

# make pattern to compile all source files
%.o: ../%.cpp
	g++ $(CFLAGS) -I.. -c -o $@ $<

clean:
	rm $(shell find *.o)
	rm $(shell find *.so)

