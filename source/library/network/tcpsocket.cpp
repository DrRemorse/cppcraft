#include "tcpsocket.hpp"

#ifdef __linux__
    #include <sys/socket.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <iostream>
    #include <sys/types.h>
    #include <stropts.h>
    #include <arpa/inet.h>
	//#include <sys/filio.h>
	#define INVALID_SOCKET 0
	#define SOCKET_ERROR  -1
	#define SD_SEND SHUT_WR
#else
    #include <winsock2.h>
	typedef int socklen_t;
#endif

#include "hostaddr.hpp"
#include <cstring>

namespace library
{
	
	TcpSocket::TcpSocket(void* userdata) : Socket(userdata)
	{
		onConnect      = nullptr;
		onConnected    = nullptr;
		onDisconnect   = nullptr;
		onError        = nullptr;
		onDataReceived = nullptr;
		
		this->connecting = false;
		this->connected  = false;
		this->listening  = false;
		
		this->readq = "";
		this->sendq = "";
		
		createTCPsocket();
	}
	
	TcpSocket::~TcpSocket()
	{
		this->disconnect(true);
	}
	
	bool TcpSocket::createTCPsocket()
	{
		// create socket
		this->socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		// handle socket creation error, if something happened
		if (this->socketId == INVALID_SOCKET)
		{
			handleErrors(STAGE_INIT, false);
			return false;
		}
		return true;
	}
	
	bool TcpSocket::bind(std::string& iface, int port)
	{
		if (this->connected || this->listening)
		{	// disconnect from old connection
			this->disconnect(false);
		}
		if (this->socketId == INVALID_SOCKET)
		{	// create new socket_fd
			this->createTCPsocket();
		}
		
		// enable re-using address
		this->setReuseAddr(1);
		
		// set local address / port
		sockaddr_in* addr = (sockaddr_in*) this->clientAddr;
		addr->sin_family = AF_INET;
		addr->sin_addr.s_addr = inet_addr(iface.c_str());
		addr->sin_port = htons(port);
		
		if (::bind(socketId, (struct sockaddr*) clientAddr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			handleErrors(STAGE_BIND, false);
			return false;
		}
		this->localAddr = iface;
		this->localPort = port;
		return true;
	}
	
	bool TcpSocket::listen(int queueSize)
	{
		// set socket as non-blocking
		this->setSocketBlocking(false);
		// listen for new connections with a given queueSize
		if (::listen(socketId, queueSize) == SOCKET_ERROR)
		{
			handleErrors(STAGE_LISTEN, false);
			this->listening = false;
			return false;
		}
		this->listening = true;
		// since the operation succeeded, fire onListening event
		if (this->onListening)
		{
			this->onListening(this, this->localAddr, this->localPort);
		}
		return true;
	}
	
	TcpSocket* TcpSocket::accept(std::string& clientHost)
	{
		// address of the client that connected
		struct sockaddr_in clientAddress;
		socklen_t clientAddressLen = sizeof(struct sockaddr_in);
		
		// accept new client connection, returned as socket fd
		int newSocket = ::accept(socketId, (struct sockaddr*) &clientAddress, &clientAddressLen);
		if (newSocket == SOCKET_ERROR)
		{
			handleErrors(STAGE_ACCEPT, false);
			return nullptr;
		}
		
		// get new clients address and resolve hostname
		std::string sAddress = std::string(inet_ntoa((struct in_addr) clientAddress.sin_addr));
		HostInfo clientInfo(sAddress, HostInfo::ADDRESS);
		char* hostName = clientInfo.getHostName();
		clientHost += std::string(hostName);
		
		// create and return the new TcpSocket
		TcpSocket* retSocket = new TcpSocket(nullptr);
		retSocket->setSocketId(newSocket);
		
		// set peer information
		retSocket->peerName = clientHost;
		retSocket->peerAddr = sAddress;
		retSocket->peerPort = clientAddress.sin_port;
		
		// set non-blocking and connected
		retSocket->setSocketBlocking(false);
		retSocket->connected = true;
		
		return retSocket;
	}
	
	bool TcpSocket::connect(std::string& hostname, int port, HostInfo::hosttype_t hType)
	{
		if (this->connected || this->listening)
		{	// disconnect from old connection
			this->disconnect(false);
		}
		if (this->socketId == INVALID_SOCKET)
		{	// create new socket_fd
			this->createTCPsocket();
		}
		
		this->connecting = true;
		
		// fire onConnect event
		if (this->onConnect) this->onConnect(this, hostname, port);
		
		// get server address
		HostInfo server(hostname, hType);
		
		if (server.good == false)
		{
			this->connecting = false;
			// Error: unable to resolve address
			this->error = SOCKET_ERROR;
			this->errorString = "Unable to resolve address";
			if (this->onError) this->onError(this, STAGE_RESOLVE, this->error, this->errorString);
			return false;
		}
		
		// set remote address / port
		struct sockaddr_in serveraddr;
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(server.getHostIPAddress());
		serveraddr.sin_port = htons(port);
		
		// set local address / port
		sockaddr_in* addr = (sockaddr_in*) this->clientAddr;
		addr->sin_family = AF_INET;
		addr->sin_addr.s_addr = htonl(INADDR_ANY);
		addr->sin_port = htons(port);
		
		// non-blocking socket
		setSocketBlocking(false);
		
		#ifdef __linux__
			// unix errors are usually not reset after successful calls
			errno = 0;
		#endif
		
		// connect to the remote address
		if (::connect(socketId, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		{
			// ignore would block error
			if (handleErrors(STAGE_CONNECT, true))
			{
				this->connecting = false;
				return false;
			}
		}
		
		// timeout struct
		timeval timer;
		timer.tv_sec  = 5; // timeout, in seconds
		timer.tv_usec = 0; //       + nanoseconds
		
		// check state
		fd_set thisSocket;
		FD_ZERO(&thisSocket);
		FD_SET(this->socketId, &thisSocket);
		
		// sleep thread until connected to server
		int ret = select(this->socketId + 1, nullptr, &thisSocket, nullptr, &timer);
		if (ret <= 0)
		{
			handleErrors(STAGE_CONNECT, false);
			this->connecting = false;
			return false;
		}
		this->connecting = false;
		
		// set local properties
		struct in_addr localaddr;
		localaddr.s_addr = INADDR_ANY;
		this->localAddr = inet_ntoa(localaddr);
		this->localPort = port;
		
		// set peer properties
		this->peerName = server.getHostName();
		this->peerAddr = server.getHostIPAddress();
		this->peerPort = port;
		
		// set socket state to connected, and fire onConnected event
		this->connected = true;
		if (this->onConnected) this->onConnected(this, hostname, port);
		return true;
	}
	
	// perform disconnect with onDisconnect event fired
	bool TcpSocket::disconnect()
	{
		return this->disconnect(false);
	}
	
	// closes connection, resets internal states
	// and invalidates the existing socket_fd
	bool TcpSocket::disconnect(bool silent)
	{
		if (this->socketId)
		{
			if (this->connected)
			{
				// graceful shutdown on our end, can return error but we don't care
				shutdown(this->socketId, SD_SEND);
				this->connected = false;
			}
			// reset flags
			this->connecting = false;
			this->listening  = false;
			// reset queues
			this->sendq = "";
			this->readq = "";
			
			// first, check if fd is still valid/alive
			#ifdef __linux__
				int r = fcntl(this->socketId, F_GETFL);
				if (r != SOCKET_ERROR)
			#else
				int rvalue = 0, rvlen  = sizeof(rvalue);
				int r = getsockopt(this->socketId, SOL_SOCKET, SO_ERROR, (char*) &rvalue, &rvlen);
				if (r != SOCKET_ERROR)
			#endif
			{
				// close socket, which destroys file descriptor
				#ifdef __linux__
					int result = close(this->socketId);
				#else
					int result = closesocket(this->socketId);
				#endif
				if (result == SOCKET_ERROR)
				{
					handleErrors(STAGE_DISCONNECT, false);
					return false;
				}
				// fire onDisconnect event
				if (silent == false)
					if (this->onDisconnect) this->onDisconnect(this);
			}
			// and, no matter what, invalidate the socket
			this->socketId = (int)INVALID_SOCKET;
			return true;
		} else return false;
	}
	
	int TcpSocket::write(std::string& message)
	{
		int bytesSent = send(socketId, (char*) message.c_str(), message.length(), 0);
		
		if (bytesSent == SOCKET_ERROR)
		{
			handleErrors(STAGE_WRITE, false);
			// perform silent disconnect
			if (this->connected) this->disconnect(true);
			return SOCKET_ERROR;
		}
		return bytesSent;
	}
	
	bool TcpSocket::writeLine(std::string data)
	{
		if (this->connected == false) return false;
		
		// append line-ending and add to write queue
		data += "\r\n";
		this->sendq.append(data);
		
		return true;
	}
	
	int TcpSocket::read(std::string& message)
	{
		char charMsg[MAX_RECV_LEN + 1];
		memset(charMsg, 0, sizeof(charMsg));
		
		#ifdef __linux__
			// retrieve the length of the message received
			int numBytes = recv(socketId, charMsg, MAX_RECV_LEN * sizeof(char), MSG_PEEK);
			if (numBytes == SOCKET_ERROR)
			{
				// error receiving message length
				if (handleErrors(STAGE_READ, true) == false)
				{
					// operation would block, nothing to read
					return 0;
				}
				// perform silent disconnect
				if (this->connected) this->disconnect(true);
				return SOCKET_ERROR;
			}
			else if (numBytes == 0)
			{
				// close connection, and fire disconnect event
				this->disconnect(false);
				return 0;
			}
			int msgLength = atoi(charMsg);
			// receive the real message
			numBytes = recv(socketId, charMsg, msgLength, 0);
		#else
			// receive the real message (win32)
			int numBytes = recv(socketId, charMsg, sizeof(char) * MAX_RECV_LEN, 0);
		#endif
		
		if (numBytes == SOCKET_ERROR)
		{
			// error receiving real message
			if (handleErrors(STAGE_READ, true) == false)
			{
				// operation would block, nothing to read
				return 0;
			}
			// perform silent disconnect
			if (this->connected) this->disconnect(true);
			return SOCKET_ERROR;
		}
		else if (numBytes == 0)
		{
			// close connection, and fire disconnect event
			this->disconnect(false);
			return 0;
		}
		message.append(charMsg, numBytes);
		return numBytes;
	}
	
	// polling function / "event handler"
	// run this from a thread loop with sleep(0)
	void TcpSocket::poll()
	{
		// if socket is neither connected nor listening, nothing to poll
		if (this->connected == false && this->listening == false)
		{
			if (this->onError)
			{
				std::string errorString = "Nothing to poll - inactive socket";
				this->onError(this, STAGE_POLL, 0, errorString);
			}
			return;
		}
		
		// timeout struct
		timeval timer;
		timer.tv_sec  = 0;     // timeout, in seconds
		timer.tv_usec = 10000; //      + microseconds
		
		// poll for read, write and error
		fd_set socketRead, socketWrite, socketError;
		FD_ZERO(&socketRead);
		FD_ZERO(&socketWrite);
		FD_ZERO(&socketError);
		FD_SET(this->socketId, &socketRead);
		FD_SET(this->socketId, &socketWrite);
		FD_SET(this->socketId, &socketError);
		
		int ret = select(this->socketId + 1, &socketRead, &socketWrite, &socketError, &timer);
		
		if (ret == SOCKET_ERROR)
		{
			handleErrors(STAGE_POLL, false);
			return;
		}
		else if (ret == 0) return; // no fd's were ready
		
		// socket has an error/exception detected
		if (FD_ISSET(this->socketId, &socketError))
		{
			handleErrors(STAGE_POLL, false);
			if (this->connected)
			{
				// perform silent disconnect
				this->disconnect(false);
			}
			return;
		}
		
		// socket is readable
		if (FD_ISSET(this->socketId, &socketRead))
		{
			if (this->listening)
			{
				// accept a new client, not read data from socket
				if (this->onAccept)
				{
					std::string clientHostname;
					TcpSocket* newClient = this->accept(clientHostname);
					if (newClient)
					{
						this->onAccept(this, this->localPort, newClient);
					}
				}
				return;
			}
			
			// read incoming data
			int bytes = this->read(this->readq);
			
			// if we got data, inrease received bytes
			if (bytes > 0)
			{
				this->totalRx += bytes;
			}
		}
		
		// socket is writable
		if (FD_ISSET(this->socketId, &socketWrite))
		{
			if (this->sendq.length() > 0)
			{
				int bytes = this->write(this->sendq);
				if (bytes > 0)
				{
					this->totalTx += bytes;
					this->sendq.erase(this->sendq.begin(), this->sendq.begin() + bytes);
				}
			}
		}
		
		// if the socket is still alive, handle read queue manually, as text
		if (this->connected)
		{
			if (this->readq.length()) parseLine(this->readq);
		}
		
	}
	
	// read text into a buffer, parse as needed
	void TcpSocket::parseLine(std::string& text)
	{
		// parse text into lines (while removing line endings)
		while (this->connected)
		{
			// find first of any line-ending characters
			std::size_t found = text.find_first_of("\r\n");
			if (found != std::string::npos)
			{
				this->buffer = text.substr(0, found);
				
				if (this->buffer.length()) // buffer length != 0
				if (this->onDataReceived)  // socket has read event
				{
					// fire readLine event
					this->onDataReceived(this, this->buffer);
					
					// exit if we got disconnected early
					if (this->connected == false) return;
				}
				// remove line (NOTE: 1 char, not 2 chars!)
				text.erase(text.begin(), text.begin() + (found + 1));
			}
			else return; // no more lines to break
		}
		
	}
	
}
