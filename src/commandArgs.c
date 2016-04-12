#include "../headers/piggy1.h"

/* Parses command line to create initial conn_t struct */
struct connection_t argCheck (int argc, char* argv[]) {
	int i;
	conn_t co;

	/* Basic struct initialization */
	co.NO_LEFT = 0;
	co.NO_RIGHT = 0;
	co.RIGHT_REMOTE_PORT = DEFAULT_PORT;
	co.RIGHT_REMOTE_ADDR = NULL;
	co.RIGHT_REMOTE_DNS = NULL;

	/* Find NORIGHT and NOLEFT first */
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-noleft") == 0) {
			co.NO_LEFT = 1;
		}

		if (strcmp(argv[i], "-noright") == 0) {
			co.NO_RIGHT = 1;
		}
	}

	/* Find all other arguments */
	for (i = 0; i < argc; i++) {
		if (!co.NO_RIGHT) {

			if (strcmp(argv[i], "-rraddr") == 0) { 
				if (argv[i+1] != NULL) {

					/* Resolve DNS name to IP address */
					co.RIGHT_REMOTE_ADDR = ipbydns(argv[i+1]);
					co.RIGHT_REMOTE_DNS = argv[i+1]; 

					if (!strcmp(co.RIGHT_REMOTE_DNS, co.RIGHT_REMOTE_ADDR)) {
						co.RIGHT_REMOTE_DNS = NULL;
					}
				}
			}

			if (strcmp(argv[i], "-rrport") == 0) {
				if (argv[i+1] != NULL) {

					/* Validate port entry and */
					int port = atoi(argv[i+1]);
					if (port < 1 || port > 65535)
						error("Specified port out of range [1 - 65535]");

					co.RIGHT_REMOTE_PORT = port;
				}
			}

		} else {
			co.RIGHT_REMOTE_DNS = NULL;
			co.RIGHT_REMOTE_ADDR = NULL;
		}		
	}

	/* Launch flag error checking */

	/* Cannot launch standalone */
	if (argc <= 1) 
		error ("Must have either left or right connection");

	/* Cannot launch without right address specification */
	if (co.RIGHT_REMOTE_ADDR == 0 && !co.NO_RIGHT)
		error("Must specify a right remote address");

	/* Cannot launch with both -noleft and -noright */
	if (co.NO_RIGHT && co.NO_LEFT) 
		error ("Cannot set both [-noleft] and [-noright]");

	return co;
}
