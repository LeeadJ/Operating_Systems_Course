CC := g++
CFLAGS := -std=c++17 -fPIC
LDFLAGS := -shared -pthread
RPATH := -Wl,-rpath=./
LIBRARY := libst_reactor.so
SERVER := react_server
SRCS := Reactor.cpp Server.cpp main.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(LIBRARY) $(SERVER)

$(LIBRARY): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER): $(LIBRARY)
	$(CC) $(CFLAGS) $(RPATH) -o $@ $^

clean:
	rm -f $(OBJS) $(LIBRARY) $(SERVER)
