#include "../headers/piggy.h"

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
	fd_set master_set, read_set;

	/* Clear file descriptor sets */
	FD_ZERO(&master_set);
	FD_ZERO(&read_set);

	/* If the chain has an open left side */
	if (!co.NO_LEFT) 
		establishleft(&co);

	/* If the chain has an open right side */
	if (!co.NO_RIGHT) 
		establishright(&co);	

	/* Add needed file descriptors to master set */
	FD_SET(co.LEFT_FACING_SOCKET, &master_set);
	FD_SET(co.RIGHT_FACING_SOCKET, &master_set);
	FD_SET(STDIN_FILENO, &master_set);

	/* Establish largest file descriptor */
	if (co.LEFT_FACING_SOCKET > co.RIGHT_FACING_SOCKET) 
		co.MAX_FD = co.LEFT_FACING_SOCKET;
	else
		co.MAX_FD = co.RIGHT_FACING_SOCKET;

	/* Running loop */
	while (1) {
		if (co.COMMAND_MODE) {
			fprintf(stdout, ":");
			fflush(stdout);

			if (read(STDIN_FILENO, buffer, BUFFER_SIZE) < 0)
				error ("Could not read from Standard In");

			/* Rudimentary 'i' key check (requires ENTER) */
			if (buffer[0] == 'i' && buffer[1] == '\n')
				co.COMMAND_MODE = 0;

			/* Run given interactice command */
			metacommand(&co, buffer, &read_set);

			bzero(buffer, BUFFER_SIZE);
		}

		if (!co.COMMAND_MODE) {
			read_set = master_set;

			if (select(co.MAX_FD+ 1, &read_set, NULL, NULL, NULL) == -1) 
				error ("Could not select from read_set");

			/* Keyboard Input Handling */
			if (FD_ISSET(STDIN_FILENO, &read_set)) {
				bzero(buffer, BUFFER_SIZE);

				if (read(STDIN_FILENO, buffer, BUFFER_SIZE) < 0)
					error ("Could not read from Standard In");

				/* Rudimentary ESC key check (requires ENTER) */
				if (buffer[0] == 27) {
					bzero(buffer, BUFFER_SIZE);
					co.COMMAND_MODE = 1;
					continue;
				}

				if (!co.NO_RIGHT && (co.OUTPUT_DIRECTION || co.NO_LEFT))
					datatoright(buffer, &co);

				if (!co.NO_LEFT && (!co.OUTPUT_DIRECTION || co.NO_RIGHT)) 
					datatoleft(buffer, &co);
			}

			/* Incoming Left Transmission Handling */
			if (FD_ISSET(co.LEFT_FACING_SOCKET, &read_set) && !co.NO_LEFT) {
				bzero(buffer, BUFFER_SIZE);
				strcpy(buffer, datafromleft(&co));

				if (co.DISPLAY_LEFT_RIGHT)
					fprintf(stdout, "%s", buffer);

				if (!co.NO_RIGHT)
					datatoright(buffer, &co);

				if (co.LOOP_RIGHT && !co.NO_LEFT)
					datatoleft(buffer, &co);
			}

			/* Incoming Right Transmission Handling */
			if (FD_ISSET(co.RIGHT_FACING_SOCKET, &read_set) && !co.NO_RIGHT) {
				bzero(buffer, BUFFER_SIZE);
				strcpy(buffer, datafromright(&co));

				if (co.DISPLAY_RIGHT_LEFT)
					fprintf(stdout, "%s", buffer);

				if (!co.NO_LEFT)
					datatoleft(buffer, &co);

				if (co.LOOP_LEFT && !co.NO_RIGHT)
					datatoright(buffer, &co);
			}
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
	this_address.sin_port        = htons(co->LEFT_LOCAL_PORT);

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

	/* Verify left side connection */
	if (co->LEFT_REMOTE_ADDR != NULL && strcmp(co->LEFT_REMOTE_ADDR, "*") != 0 && strcmp(getremoteip(co->LEFT_FACING_SOCKET), co->LEFT_REMOTE_ADDR) != 0) {
		close(co->LEFT_FACING_SOCKET);
		error("Invalid left remote address");
	}

	if (co->LEFT_REMOTE_PORT != 0 && strcmp(itoa(co->LEFT_REMOTE_PORT), "*") != 0 && getremoteport(co->LEFT_FACING_SOCKET) != co->LEFT_REMOTE_PORT) {
		close(co->LEFT_FACING_SOCKET);
		error("Invalid left remote port");
	}

	/* Colorize output */
	fprintf(stdout, "%s\n", colorize(CYAN, "[Left Side Online]"));
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

	/* Colorize output */
	fprintf(stdout, "%s\n", colorize(CYAN, "[Right Side Online]"));
}

/* Writes buffer to RIGHT_FACING_SOCKET defined by conn_t */
void datatoright (char* buffer, struct connection_t* co) {
	if (send(co->RIGHT_FACING_SOCKET, buffer, strlen(buffer), SENDFLAGS) < 0)
		error("Could not write to [right] socket");
}

/* Writes buffer to LEFT_FACING_SOCKET defined by conn_t */
void datatoleft (char* buffer, struct connection_t* co) {
	if (send(co->LEFT_FACING_SOCKET, buffer, strlen(buffer), SENDFLAGS) < 0)
		error("Could not write to [left] socket");
}

/* Returns data read from LEFT_FACING_SOCKET defined by conn_t */
char* datafromleft (struct connection_t* co) {
	char output[BUFFER_SIZE];
	bzero(output, BUFFER_SIZE);

	if (recv(co->LEFT_FACING_SOCKET, output, BUFFER_SIZE-1, RECVFLAGS) < 0)
		error("Could not read from [left] socket");

	return strdup(output);
}

/* Returns data read from RIGHT_FACING_SOCKET defined by conn_t */
char* datafromright (struct connection_t* co) {
	char output[BUFFER_SIZE];
	bzero(output, BUFFER_SIZE);

	if (recv(co->RIGHT_FACING_SOCKET, output, BUFFER_SIZE-1, RECVFLAGS) < 0)
		error("Could not read from [right] socket");

	return strdup(output);
}