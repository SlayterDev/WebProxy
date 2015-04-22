#include "globals.h"
#include "net.h"

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		fprintf(stderr, "<port>: The port to run the proxy on.\n");
		return 1;
	}

	beginServer(atoi(argv[1]));

	return 0;
}