#include <sys/types.h>
#include <sys/select.h>

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

#ifndef INTERACT
#define INTERACT

int metacommand(struct connection_t* co, char* buffer, fd_set* read_set);

#endif