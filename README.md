# Network-Monitor
C Program designed to monitor network traffic.  Based on "Piggy, A Tool for Rapid Prototyping, Testing, Monitoring, Protecting, and Logging of Network Applications, Computer Applications in Industry and Engineering (CAINE)" by Dr. Michael Meehan

## Launch Flags

### -noleft
Indicates that there will be no incoming left-side connection.  Creates a "head" of a chain that will read from standard in and write to the right side connection.

### -noright
Indicates that there will be no right side that will be connected to.  Creates a "tail" of a chain that will read from the left side connection and write to standard out.

### -rraddr [address]
Used to specify the address of the node that a connection will be attempted on.  The address can be a direct IP address in dotted decimal notation or a DNS name that will be resolved to such.  This must always be specified unless -noright is being used.

### -rrport [port]
Used to specify the port of the node that a connection will be attempted on.  If this parameter is not given then it will default to 36755.
