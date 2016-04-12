#include "../headers/piggy1.h"

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
	fprintf(stdout, " RRADDR: '%s' \n", co.RIGHT_REMOTE_ADDR);
	fprintf(stdout, "  RRDNS: '%s' \n", co.RIGHT_REMOTE_DNS);
	fprintf(stdout, " RRPORT: '%d' \n", co.RIGHT_REMOTE_PORT);
	fprintf(stdout, " LLPORT: '%d' \n\n", co.LEFT_LOCAL_PORT);
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