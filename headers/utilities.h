#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

/* Color parameters */
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define COLOR_SIZE 50

/* Declared twice for error suppression */
#ifndef INIT
#define INIT

typedef struct connection_t {
	int   NO_RIGHT;
	int   NO_LEFT;
	int   DISPLAY_LEFT_RIGHT;
	int   DISPLAY_RIGHT_LEFT;
	int   LOOP_RIGHT;
	int   LOOP_LEFT;
	int   OUTPUT_DIRECTION;
	int   RIGHT_REMOTE_PORT;
	int   LEFT_REMOTE_PORT;
	int   LEFT_LOCAL_PORT;
	char* RIGHT_REMOTE_ADDR;
	char* RIGHT_REMOTE_DNS;
	char* LEFT_REMOTE_ADDR;
	char* LEFT_REMOTE_DNS;
	int   RIGHT_FACING_SOCKET;
	int   LEFT_FACING_SOCKET;
	int   COMMAND_MODE;

	int   MAX_FD;
} conn_t;

#endif

#ifndef UTILITIES
#define UTILITIES

void error (char* mod);
char* ipbydns(char* dns);
int getboundport (int socket_fd);
int socketisopen(int socket_fd);
char* itoa (int x);
void display(struct connection_t co);
char* colorize(char* color, char* string);
char* getremoteip(int socket);
char* getlocalip();
int getremoteport(int socket);
int getlocalport(int socket);

#endif