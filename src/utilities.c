#include "../headers/piggy.h"

/* Prints RED error message to STDERR and exit(1) */
void error (char* mod) {
	fprintf(stderr, "%s %s \n", colorize(RED, "Error:"), mod);
	exit(1);
}

/* Resloves DNS name into dotted decimal IP address on success, returns original string otherwise */
char* ipbydns(char* dns) {
	struct hostent *he;
	struct in_addr **addr_list;
	int i;

	if ((he = gethostbyname(dns)) == NULL) 
		return dns;

	addr_list = (struct  in_addr **) he->h_addr_list;

	for (i = 0; addr_list[i] != NULL; i++) {
		return strdup(inet_ntoa(*addr_list[i]));
	}

	return dns;
}

/* Returns current port bound on socket file descriptor */
int getboundport(int socket_fd) {
	struct sockaddr_in local;
	socklen_t len = sizeof(local);
    getsockname(socket_fd, (struct sockaddr *) &local, &len);
    return ntohs(local.sin_port);
}

/* Displays needed values of connection_t struct */
void display(struct connection_t co) {
	fprintf(stdout, "\n NOLEFT: '%d' \n", co.NO_LEFT);
	fprintf(stdout, "NORIGHT: '%d' \n", co.NO_RIGHT);
	fprintf(stdout, "  LOOPR: '%d' \n", co.LOOP_RIGHT);
	fprintf(stdout, "  LOOPL: '%d' \n", co.LOOP_LEFT);
	fprintf(stdout, "  DSPRL: '%d' \n", co.DISPLAY_RIGHT_LEFT);
	fprintf(stdout, "  DSPLR: '%d' \n", co.DISPLAY_LEFT_RIGHT);
	fprintf(stdout, " RRADDR: '%s' \n", co.RIGHT_REMOTE_ADDR);
	fprintf(stdout, "  RRDNS: '%s' \n", co.RIGHT_REMOTE_DNS);
	fprintf(stdout, " LRADDR: '%s' \n", co.LEFT_REMOTE_ADDR);
	fprintf(stdout, "  LRDNS: '%s' \n", co.LEFT_REMOTE_DNS);
	fprintf(stdout, " RRPORT: '%d' \n", co.RIGHT_REMOTE_PORT);
	fprintf(stdout, " LLPORT: '%d' \n", co.LEFT_LOCAL_PORT);
	fprintf(stdout, " LRPORT: '%d' \n", co.LEFT_REMOTE_PORT);
	fprintf(stdout, " RSOCKE: '%d' \n", co.RIGHT_FACING_SOCKET);
	fprintf(stdout, " LSOCKE: '%d' \n", co.LEFT_FACING_SOCKET);
	fprintf(stdout, " STDINF: '%d' \n", STDIN_FILENO);
	fprintf(stdout, " MAX_FD: '%d' \n\n", co.MAX_FD);
}

/* Shitty way of converting integer to string */
char* itoa (int x) {
	static char str[15];
	sprintf(str, "%d", x);
	return str;
}

/* Converts string to colored version using preprocessor directives in 'utilities.h'
		color   is color to be converted too
		string  is the string to be colorized */
char* colorize(char* color, char* string) {
	char storage[COLOR_SIZE];
	snprintf(storage, COLOR_SIZE, "%s%s%s", color, string, RESET);
	return strdup(storage);
}

/* Returns the ip address of peer connected on socket */
char* getremoteip(int socket) {
	struct sockaddr_in s;
	socklen_t len = sizeof(s);
	getpeername(socket, (struct sockaddr *) &s, &len);

	if (!strcmp(inet_ntoa(s.sin_addr), "127.0.0.1"))
		return getlocalip();

	return strdup(inet_ntoa(s.sin_addr));
}

/* Returns host ip address */
char* getlocalip() {
	char hostname[128];
	gethostname(hostname, sizeof(hostname));
	return ipbydns(hostname);
}

/* Returns port number of peer connected on socket */
int getremoteport(int socket) {
	struct sockaddr_in s;
	socklen_t len = sizeof(s);
	getpeername(socket, (struct sockaddr *) &s, &len);
	return ntohs(s.sin_port);
}

/* Returns port number of local socket */
int getlocalport(int socket) {
	struct sockaddr_in s;
	socklen_t len = sizeof(s);
	getsockname(socket, (struct sockaddr *) &s, &len);
	return ntohs(s.sin_port);
}