#include "../headers/piggy.h"

/* Parses command line to create initial conn_t struct */
struct connection_t argCheck (int argc, char* argv[]) {
	int i;
	conn_t co;

	/* Basic struct initialization */
	co.NO_LEFT = 0;
	co.NO_RIGHT = 0;
	co.DISPLAY_LEFT_RIGHT = 1;
	co.DISPLAY_RIGHT_LEFT = 0;
	co.LOOP_LEFT = 0;
	co.LOOP_RIGHT = 0;

	co.RIGHT_REMOTE_PORT = DEFAULT_PORT;
	co.LEFT_REMOTE_PORT = 0;
	co.LEFT_LOCAL_PORT = DEFAULT_PORT;

	co.RIGHT_REMOTE_ADDR = NULL;
	co.RIGHT_REMOTE_DNS = NULL;
	co.LEFT_REMOTE_ADDR = NULL;
	co.LEFT_REMOTE_DNS = NULL;

	co.RIGHT_FACING_SOCKET = 0;
	co.LEFT_FACING_SOCKET = 0;

	co.OUTPUT_DIRECTION = 1;
	co.COMMAND_MODE = 0;

	/* Find non-valued flags first */
	for (i = 0; i < argc; i++) {

		if (!strcmp(argv[i], "-noleft")) {
			co.NO_LEFT = 1;
			co.DISPLAY_LEFT_RIGHT = 0;
			co.DISPLAY_RIGHT_LEFT = 1;
		}

		if (!strcmp(argv[i], "-noright")) {
			co.NO_RIGHT = 1;
			co.DISPLAY_LEFT_RIGHT = 1;
			co.DISPLAY_RIGHT_LEFT = 0;
		}

		if (!strcmp(argv[i], "-dsplr") && !co.NO_LEFT && !co.NO_RIGHT) {
			co.DISPLAY_LEFT_RIGHT = 1;
			co.DISPLAY_RIGHT_LEFT = 0;
		}

		if (!strcmp(argv[i], "-dsprl") && !co.NO_LEFT && !co.NO_RIGHT) {
			co.DISPLAY_LEFT_RIGHT = 0;
			co.DISPLAY_RIGHT_LEFT = 1;
		}

		if (!strcmp(argv[i], "-loopr"))
			co.LOOP_RIGHT = 1;

		if (!strcmp(argv[i], "-loopl"))
			co.LOOP_LEFT = 1;
	}

	/* Find all other flags */
	for (i = 0; i < argc; i++) {
		if (!co.NO_RIGHT) {

			if (!strcmp(argv[i], "-rraddr")) { 
				if (argv[i+1] != NULL) {

					/* Resolve DNS name to IP address */
					co.RIGHT_REMOTE_ADDR = ipbydns(argv[i+1]);
					co.RIGHT_REMOTE_DNS = argv[i+1]; 

					if (!strcmp(co.RIGHT_REMOTE_DNS, co.RIGHT_REMOTE_ADDR)) 
						co.RIGHT_REMOTE_DNS = NULL;
				}
			}

			if (!strcmp(argv[i], "-rrport")) {
				if (argv[i+1] != NULL) {

					/* Validate port entry */
					int rrport = atoi(argv[i+1]);
					if (rrport < 1 || rrport > 65535)
						error("Specified port out of range [1 - 65535]");

					co.RIGHT_REMOTE_PORT = rrport;
				}
			}

		} 

		if (!co.NO_LEFT) {

			if (!strcmp(argv[i], "-lraddr")) {
				if(argv[i+1] != NULL) {

					/* Resolve DNS name to IP address */
					co.LEFT_REMOTE_ADDR = ipbydns(argv[i+1]);
					co.LEFT_REMOTE_DNS = argv[i+1];

					if (!strcmp(co.LEFT_REMOTE_DNS, co.LEFT_REMOTE_ADDR)) 
						co.LEFT_REMOTE_DNS = NULL;
				}
			}

			if (!strcmp(argv[i], "-lrport")) {
				if (argv[i+1] != NULL) {

					if (strcmp(argv[i+1], "*")) {

						/* Validate port entry */
						int lrport = atoi(argv[i+1]);
						if (lrport < 1 || lrport > 65535)
							error("Specified port out of range [1 - 65535]");

						co.LEFT_REMOTE_PORT = lrport;
					} 
				}
			}

			if (!strcmp(argv[i], "-llport")) {
				if (argv[i+1] != NULL) {

					/* Validate port entry */
					int llport = atoi(argv[i+1]);
					if (llport < 1 || llport > 65535)
						error("Specified port out of range [1 - 65535]");

					co.LEFT_LOCAL_PORT = llport;
				}
			}
		} 
	}

	/* Launch flag error checking */

	/* Giving both defaults to RIGHT TO LEFT */
	if (co.DISPLAY_RIGHT_LEFT && co.DISPLAY_LEFT_RIGHT) {
		co.DISPLAY_LEFT_RIGHT = 1;
		co.DISPLAY_RIGHT_LEFT = 0;
	}

	/* Giving both defaults to neither */
	if (co.LOOP_LEFT && co.LOOP_RIGHT) {
		co.LOOP_RIGHT = 0;
		co.LOOP_LEFT = 0;
	}

	/* Resolve LocalHost ip address */
	if (co.RIGHT_REMOTE_ADDR != NULL && !strcmp(co.RIGHT_REMOTE_ADDR, "0.0.0.0"))
		co.RIGHT_REMOTE_ADDR = getlocalip();

	/* Resolve LocalHost ip address */
	if (co.LEFT_REMOTE_ADDR != NULL && !strcmp(co.LEFT_REMOTE_ADDR, "0.0.0.0"))
		co.LEFT_REMOTE_ADDR = getlocalip();

	/* Cannot launch standalone */
	if (argc <= 1) 
		error ("Must have either left or right connection");

	/* Cannot launch without right address specification */
	if (!co.RIGHT_REMOTE_ADDR && !co.NO_RIGHT)
		error("Must specify a right remote address or [-noright]");

	/* Cannot launch with both -noleft and -noright */
	if (co.NO_RIGHT && co.NO_LEFT) 
		error ("Cannot set both [-noleft] and [-noright]");

	return co;
}
