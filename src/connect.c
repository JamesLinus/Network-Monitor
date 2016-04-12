#include "../headers/piggy1.h"

#define DEBUG 0
#define BUFFER_SIZE 256

#define DOMAIN   AF_INET
#define TYPE     SOCK_STREAM
#define PROTOCOL 0
#define INADDR   INADDR_ANY
#define SOCKOPTS SOL_SOCKET, SO_REUSEADDR
#define SENDFLAGS 0
#define RECVFLAGS 0

/* Runs main networking routine */
int activate (struct connection_t co) {
	char buffer[BUFFER_SIZE];

	/* If the chain has an open left side */
	if (!co.NO_LEFT) 
		establishleft(&co);

	#if DEBUG
		display(co);
	#endif

	/* If the chain has an open right side */
	if (!co.NO_RIGHT) 
		establishright(&co);	    	

	/* Running loop */
	while (1) {

		/* TAIL */
		if (co.NO_RIGHT) {
			/* Read from left and display */
			fprintf(stdout, "%s", datafromleft(&co));
		}

		/* HEAD */
		if (co.NO_LEFT) {
			bzero(buffer, BUFFER_SIZE);

			/* Read from STDIN and send to right */
			fgets(buffer, BUFFER_SIZE-1, stdin);
			datatoright(buffer, &co);
		}

		/* MIDDLE */
		if (!co.NO_LEFT && !co.NO_RIGHT) {
			bzero(buffer, BUFFER_SIZE);

			/* Read from left and display */
			strcpy(buffer, datafromleft(&co));
			fprintf(stdout, "%s", buffer);

			/* Send from left to right */
			datatoright(buffer, &co);
		}
	}

	/* Close active sockets */
	close(co.LEFT_FACING_SOCKET);
	close(co.RIGHT_FACING_SOCKET);

	return 1;
}

/* Establishes a basic left side connection */
void establishleft(struct connection_t* co) {

	/* Setup variables */
	int left_socket_fd = 0;
	socklen_t connecting_len;
	struct sockaddr_in this_address, received_address;

	/* Create socket */
	left_socket_fd = socket(DOMAIN, TYPE, PROTOCOL);

	if (left_socket_fd < 0) 
		error ("Could not open [left] socket");

	/* Define bind parameters */
	bzero((char*) &this_address, sizeof(this_address));

	this_address.sin_family      = DOMAIN;
	this_address.sin_addr.s_addr = INADDR;
	this_address.sin_port        = htons(DEFAULT_PORT);

	/* Set socket options */
	int flag = 1;
	if (setsockopt (left_socket_fd, SOCKOPTS, &flag, sizeof(int)) < 0)
		error ("Could not set [left] socket options");

	/* Bind to default port, or any available if not possible */
	if (bind(left_socket_fd, (struct sockaddr*) &this_address, sizeof(this_address)) < 0) {
		this_address.sin_port = htons(0);
		if (bind(left_socket_fd, (struct sockaddr*) &this_address, sizeof(this_address)) < 0) 
			error("Could not bind [left] socket");
	}

	/* Update conn_t LEFT_LOCAL_PORT value */
	co->LEFT_LOCAL_PORT = getboundport(left_socket_fd);

	/* Colorize output and resolve colorized port display */
	char* insert = "";
	char* port = colorize(GREEN, itoa(co->LEFT_LOCAL_PORT));
	if (co->LEFT_LOCAL_PORT == DEFAULT_PORT)
		insert = colorize(YELLOW, "DEFAULT ");

	fprintf(stdout, "\n[Left socket bound on %sport %s]\n\n", insert, port);

	/* Wait for incoming connection request */
	listen(left_socket_fd, 5);

	/* Accept connection and update conn_t LEFT_FACING_SOCKET file descriptor */
	connecting_len = sizeof(received_address);
	co->LEFT_FACING_SOCKET = accept(left_socket_fd, (struct sockaddr*) &received_address, &connecting_len);

	if (co->LEFT_FACING_SOCKET < 0) 
		error("Could not accept [left] connection");

	/* Colorize output */
	fprintf(stdout, "%s\n", colorize(CYAN, "[Receiving]"));
}

/* Establishes a basic right side connection */
void establishright(struct connection_t* co) {
	/* Setup variables */
	int right_socket_fd = 0;
	struct sockaddr_in connecting_address;

	/* Create socket */
	right_socket_fd = socket(DOMAIN, TYPE, PROTOCOL);

	if (right_socket_fd < 0)
		error("Could not open [right] socket");

	/* Define connection parameters */
	bzero((char*) &connecting_address, sizeof(connecting_address));

	connecting_address.sin_family = AF_INET;
	connecting_address.sin_addr.s_addr = inet_addr(co->RIGHT_REMOTE_ADDR);
	connecting_address.sin_port = htons(co->RIGHT_REMOTE_PORT);

	/* Attempt connection by parameters */
	if (connect(right_socket_fd, (struct sockaddr*) &connecting_address, sizeof(connecting_address)) < 0)
		error("Could not connect [right] socket");

	/* Update conn_t RIGHT_FACING_SOCKET file descriptor */
	co->RIGHT_FACING_SOCKET = right_socket_fd;

	/* Colorize output for HEAD node */
	if (co->NO_LEFT) 
    	fprintf(stdout, "%s\n", colorize(CYAN, "\nWhat would you like to transmit?"));	
}

/* Writes buffer to RIGHT_FACING_SOCKET defined by conn_t */
void datatoright (char* buffer, struct connection_t* co) {
	char output[BUFFER_SIZE];
	bzero (output, BUFFER_SIZE);

	if (send(co->RIGHT_FACING_SOCKET, buffer, strlen(buffer), SENDFLAGS) < 0)
		error("Could not write to [right] socket");
}

/* Returns data read from LEFT_FACING_SOCKET defined by conn_t */
char* datafromleft (struct connection_t* co) {
	static char output[BUFFER_SIZE];
	bzero(output, BUFFER_SIZE);

	if (recv(co->LEFT_FACING_SOCKET, output, BUFFER_SIZE-1, RECVFLAGS) < 0)
		error("Could not read from [left] socket");

	return output;
}