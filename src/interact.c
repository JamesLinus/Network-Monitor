#include "../headers/piggy.h"

int metacommand(struct connection_t* co, char* buffer, fd_set* read_set) {
	/* Quit the program */
	if (buffer[0] == 'q')
		exit(1);

	/* Display information about current state */
	if (buffer[0] == 'd' && buffer[1] == '\n')
		display(*co);

	/* Change output direction of middle node to LEFT */
	if (!strcmp(buffer, "outputl\n")) {
		if (!co->NO_LEFT)
			co->OUTPUT_DIRECTION = 0;
		else
			co->OUTPUT_DIRECTION = 1;
	}

	/* Change output direction of middle node to RIGHT */
	if (!strcmp(buffer, "outputr\n")) {
		if (!co->NO_RIGHT)
			co->OUTPUT_DIRECTION = 1;
		else
			co->OUTPUT_DIRECTION = 0;
	}

	/* Display current output direction */
	if (!strcmp(buffer, "output\n")) {
		if (co->OUTPUT_DIRECTION)
			fprintf(stdout, "%s%s\n", "Output Direction: ", colorize(BLUE, "Right"));
		if (!co->OUTPUT_DIRECTION)
			fprintf(stdout, "%s%s\n", "Output Direction: ", colorize(BLUE, "Left"));
	}

	/* Change data display direction to L - R */
	if (!strcmp(buffer, "dsplr\n")) {
		if (!co->NO_LEFT) {
			co->DISPLAY_LEFT_RIGHT = 1;
			co->DISPLAY_RIGHT_LEFT = 0;
		} else {
			co->DISPLAY_LEFT_RIGHT = 0;
			co->DISPLAY_RIGHT_LEFT = 1;
		}
	}

	/* Change data display direction to R - L */
	if (!strcmp(buffer, "dsprl\n")) {
		if (!co->NO_RIGHT) {
			co->DISPLAY_LEFT_RIGHT = 0;
			co->DISPLAY_RIGHT_LEFT = 1;
		} else {
			co->DISPLAY_LEFT_RIGHT = 1;
			co->DISPLAY_RIGHT_LEFT = 0;
		}
	}

	/* Display current data display direction */
	if (!strcmp(buffer, "display\n")) {
		if (co->DISPLAY_LEFT_RIGHT)
			fprintf(stdout, "%s%s%s\n", "Display: Left", colorize(GREEN, " --> "), "Right");
		if (co->DISPLAY_RIGHT_LEFT)
			fprintf(stdout, "%s%s%s\n", "Display: Left", colorize(GREEN, " <-- "), "Right");
	}

	/* Drop right side connection */
	if (!strcmp(buffer, "dropr\n")) {
		if (co->LEFT_FACING_SOCKET > STDIN_FILENO)
			co->MAX_FD = co->LEFT_FACING_SOCKET;
		else
			co->MAX_FD = STDIN_FILENO;

		FD_CLR (co->RIGHT_FACING_SOCKET, read_set);
		shutdown(co->RIGHT_FACING_SOCKET, SHUT_WR);

		co->RIGHT_FACING_SOCKET = -1;
		co->NO_RIGHT = 1;

		if (!co->DISPLAY_LEFT_RIGHT) {
			co->DISPLAY_LEFT_RIGHT = 1;
			co->DISPLAY_RIGHT_LEFT = 0;
		}

		fprintf(stdout, "%s\n", colorize(MAGENTA, "Right Connection Dropped"));
	}

	/* Drop left side connection */
	if (!strcmp(buffer, "dropl\n")) {
		if (co->RIGHT_FACING_SOCKET > STDIN_FILENO)
			co->MAX_FD = co->RIGHT_FACING_SOCKET;
		else
			co->MAX_FD = STDIN_FILENO;

		FD_CLR (co->LEFT_FACING_SOCKET, read_set);
		shutdown(co->LEFT_FACING_SOCKET, SHUT_WR);

		co->LEFT_FACING_SOCKET = -1;
		co->NO_LEFT = 1;

		if (!co->DISPLAY_RIGHT_LEFT) {
			co->DISPLAY_LEFT_RIGHT = 0;
			co->DISPLAY_RIGHT_LEFT = 1;
		}

		fprintf(stdout, "%s\n", colorize(MAGENTA, "Left Connection Dropped"));
	}

	/* Display information about right side connection */
	if (!strcmp(buffer, "right\n")) {
		fprintf(stdout, "%s : %s : %s : %s \n", 
			colorize(GREEN, itoa(getlocalport(co->RIGHT_FACING_SOCKET))),
			colorize(GREEN, getlocalip()),
			colorize(BLUE, itoa(getremoteport(co->RIGHT_FACING_SOCKET))),
			colorize(BLUE, getremoteip(co->RIGHT_FACING_SOCKET)));
	}

	/* Display information about left side connection */
	if (!strcmp(buffer, "left\n")) {
		fprintf(stdout, "%s : %s : %s : %s \n", 
			colorize(GREEN, itoa(getlocalport(co->LEFT_FACING_SOCKET))),
			colorize(GREEN, getlocalip()),
			colorize(BLUE, itoa(getremoteport(co->LEFT_FACING_SOCKET))),
			colorize(BLUE, getremoteip(co->LEFT_FACING_SOCKET)));
	}

	/* Display left remote address information */
	if (!strcmp(buffer, "lraddr\n")) {
		fprintf(stdout, "%s\n", colorize(BLUE, getremoteip(co->LEFT_FACING_SOCKET)));
	}

	/* Display right remote address information */
	if (!strcmp(buffer, "rraddr\n")) {
		fprintf(stdout, "%s\n", colorize(BLUE, getremoteip(co->RIGHT_FACING_SOCKET)));
	}

	/* Enable left looping */
	if (!strcmp(buffer, "loopl\n")) {
		co->LOOP_LEFT = 1;

		if (co->LOOP_LEFT && co->LOOP_RIGHT) 
			co->LOOP_LEFT = 0;
	}

	/* Enable right looping */
	if (!strcmp(buffer, "loopr\n")) {
		co->LOOP_RIGHT = 1;

		if (co->LOOP_LEFT && co->LOOP_RIGHT) 
			co->LOOP_RIGHT = 0;
	}

	return 1;
}