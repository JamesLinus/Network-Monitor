# Network-Monitor
C Program designed to monitor network traffic.  Based on "Piggy, A Tool for Rapid Prototyping, Testing, Monitoring, Protecting, and Logging of Network Applications, Computer Applications in Industry and Engineering (CAINE)" by Dr. Michael Meehan

## Launch Flags
These are all set in the command line when launching an instance of the application.  They indicate initial behavior and some can be changed interactively.

#### -noleft
Indicates that there will be no incoming left-side connection.  Creates a "head" of a chain that will read from standard in and write to the right side connection.

#### -noright
Indicates that there will be no right side that will be connected to.  Creates a "tail" of a chain that will read from the left side connection and write to standard out.

#### -rraddr [address]
Used to specify the address of the node that a connection will be attempted on.  The address can be a direct IP address in dotted decimal notation or a DNS name that will be resolved to such.  This must always be specified unless -noright is being used.

<<<<<<< HEAD
#### -lraddr [address]
Used to specify what addresses are valid when accepting a connection from the left side.  The value can be either a direct IP address in dotted decimal notation, a DNS name that will be resolved to such, or the value "*" (including quotation marks) to specify any address.  This flag is optional and defaults to accepting any address.

#### -rrport [port]
Used to specify the port of the node that a connection will be attempted on.  If this parameter is not given then it will default to 36755.

#### -lrport [port]
Similar to -lraddr, this flag is used to specify what ports are valid when accepting a connection from the left side.  The value can be either a valid port number or the value "*" (including quotation marks) to specify any port.  This flag is optional and defaults to accepting any port.

#### -llport [port]
This option is used to indicate what port address is to be used to listen on the left side.  If this parameter is not given then it will default to 36755.

#### -dsplr
This option will display the data flow from left to right.  This flag is only effective when used in a middle chain node.

#### -dsprl
This option will display the data flow from right to left.  This flag is only effective when used in a middle chain node.

#### -loopr
This option takes the incoming data flow from the left side of a node and injects it into the outgoing data flow on the left side of the same node.

#### -loopl
This option takes the incoming data flow from the right side of a node and injects it into the outgoing data flow on the right side of the same node.

## Interactive Commands
Upon launch, all nodes are in command mode by default.  This means they will not be able to transmit data to each other.  Exiting command mode is done by pressing [i] and [ENTER] in immediate sequence.  Hitting the [ESC] key and [ENTER] in immediate sequence will enter command mode from input mode. 

#### :q
Terminate the program

#### :outputl
Sets the output direction of a middle node to the left.

#### :outputr
Sets the output direction of a middle node to the right.  This is the default direction.

#### :output
Displays on screen what middle node output direction is currently enabled.

#### :dsplr
Displays the data stream of a middle node moving from left to right.  This is the default direction.

#### :dsprl
Displays the data stream of a middle node moving from right to left.

#### :dsp
Displays on screen what direction the display of a middle node is currently set to.

#### :dropright
Drops the right side connection.

#### :dropleft
Drops the left side connection.

#### :right
Displays information about the right side connection of the form 
[ Local IP : Local Port : Remote IP : Remote Port ]

#### :left
Displays information about the left side connection of the form 
[ Local IP : Local Port : Remote IP : Remote Port ]

#### :lraddr
Displays the IP address of the currently connected left side.

#### :rraddr
Displays the IP address of the currently connected right side.

#### :loopr
Enables right looping mode.  This takes incoming data flow from the left side of a node and injects it into the outgoing data flow on the left side of the same node.

#### :loopl
Enables left looping mode.  This takes the incoming data flow from the right side of a node and injects it into the outgoing data flow on the right side of the same node.
=======
### -lraddr [address]
Used to specify what addresses are valid when accepting a connection from the left side.  The value can be either a direct IP address in dotted decimal notation, a DNS name that will be resolved to such, or the value "*" (including quotation marks) to specify any address.  This flag is optional and defaults to accepting any address.

### -rrport [port]
Used to specify the port of the node that a connection will be attempted on.  If this parameter is not given then it will default to 36755.

### -lrport [port]
Similar to -lraddr, this flag is used to specify what ports are valid when accepting a connection from the left side.  The value can be either a valid port number or the value "*" (including quotation marks) to specify any port.  This flag is optional and defaults to accepting any port.

### -llport [port]
This option is used to indicate what port address is to be used to listen on the left side.  If this parameter is not given then it will default to 36755.

### -dsplr
This option will display the data flow from left to right.  This flag is only effective when used in a middle chain node.

### -dsprl
This option will display the data flow from right to left.  This flag is only effective when used in a middle chain node.

### -loopr
This option takes the incoming data flow from the left side of a node and injects it into the outgoing data flow on the left side of the same node.

### -loopl
This option takes the incoming data flow from the right side of a node and injects it into the outgoing data flow on the right side of the same node.
>>>>>>> 52c7235d009955e303829bd8573d2db2a34428f7
