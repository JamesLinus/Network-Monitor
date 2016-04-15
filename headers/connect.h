#include <sys/types.h>
#include <sys/socket.h>

#ifndef INIT
#define INIT

/* Declared twice for error suppression */

typedef struct connection_t {
	int   NO_RIGHT;
	int   NO_LEFT;
	int   DISPLAY_LEFT_RIGHT;
	int   DISPLAY_RIGHT_LEFT;
	int   LOOP_RIGHT;
	int   LOOP_LEFT;
	int   RIGHT_REMOTE_PORT;
	int   LEFT_REMOTE_PORT;
	int   LEFT_LOCAL_PORT;
	char* RIGHT_REMOTE_ADDR;
	char* RIGHT_REMOTE_DNS;
	char* LEFT_REMOTE_ADDR;
	char* LEFT_REMOTE_DNS;
	int   RIGHT_FACING_SOCKET;
	int   LEFT_FACING_SOCKET;
} conn_t;

#endif

#ifndef CON
#define CON

int activate(struct connection_t co);
void establishleft(struct connection_t* co);
void establishright(struct connection_t* co);
void datatoleft (char* buffer, struct connection_t* co);
void datatoright(char* buffer, struct connection_t* co);
char* datafromleft(struct connection_t* co);
char* datafromright (struct connection_t* co);

#endif