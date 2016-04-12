#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

#include "../headers/commandArgs.h"
#include "../headers/utilities.h"
#include "../headers/connect.h"

#define DEFAULT_PORT 36755