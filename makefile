SOURCES := main.o net.o proxy.o

CFLAGS := -g -std=c99 -lpthread

all: $(SOURCES)
	CC $(SOURCES) -g -lpthread -o Proxy

clean:
	rm *.o Proxy
