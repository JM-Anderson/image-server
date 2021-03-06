# Image Server
This server takes files from the disk and streams them over TCP to any clients.
It uses a multithreaded implementation with one thread dedicated to each client.
It may also be used to relay data by taking incoming TCP data from one connection and serving it to another client.

## Compilation
The server is written using Qt 5 and should be able to be built with the
standard Qt compilation process. Windows 10 and Ubuntu have both been tested to work with the server.

## Startup
This was designed to be used as a command line utility. When starting the server, there are two optional arguments:

1. -p, --port <port number>  
    Sets the port the server will listen on. Default: `1234`

2. -d, --data-directory <directory path>  
    Sets the main directory. Other paths will be relative to this directory. Default: `/data`

## Commands
Once clients connect to the server, you may issue commands to determine what to send them.
1. send <file path>  
	Note that the file path is relative to the `data-directory` set at startup. This will start streaming the
	specified file to the client.

2. relay <address> <port number>  
	This will try to connect to the specified address and port. It will then wait for data to be received from
	the new connection. Any data received will be immediately sent to the client.
	
3. current  
	Since the server may have multiple clients at once, the `current` command will output information about
	which client is currently selected
	
4. select <client ID>
	This will switch to the specified client. `client ID` is simply a zero based counter assigned to each client.
	The first client that connects has an ID of 0, the second has an ID of 1, and so on. 