#include "socket.hpp"

#ifdef __linux__
    #include <sys/socket.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <iostream>
    #include <sys/types.h>
    #include <stropts.h>
    //#include <sys/filio.h>
	#define INVALID_SOCKET 0
	#define SOCKET_ERROR  -1
	#include <cstring>
#else
    #include <winsock2.h>
	typedef int socklen_t;
#endif

namespace library
{
	bool Socket::init = false;
	
	// used by all socket constructors
	Socket::Socket(void* userdata)
	{
		if (init == false)
		{
			#ifdef __linux__
				
			#else
				WORD wversion = MAKEWORD(2, 2);
				WSADATA wsaData;
				
				if(WSAStartup(wversion, &wsaData) != 0)
				{
					handleErrors(STAGE_INIT, false);
					return;
				}
			#endif
			init = true;
		}
		
		// sockets are blocking by default
		this->blocking = true;
		
		// communication struct, port etc.
		clientAddr = new sockaddr_in;
		// set socket fd to invalid value
		this->socketId = (int)INVALID_SOCKET;
		
		// user-provided data
		this->userData = userdata;
	}
	
	Socket::~Socket()
	{
		if (this->socketId != INVALID_SOCKET)
		{
			#ifdef __linux__
				close(socketId);
			#else
				closesocket(socketId);
			#endif
		}
		if (clientAddr) delete ((sockaddr_in*) clientAddr);
	}
	
	bool Socket::socketOption(int option, int value)
	{
		if (setsockopt(
				socketId, 
				SOL_SOCKET, 
				option,  // <-- option goes here
				(char*) &value, 
				sizeof(value)
			) == SOCKET_ERROR)
		{
			handleErrors(STAGE_OPTION, false);
			return false;
		}
		return true;
	}
	
	bool Socket::setKeepAlive(int value)
	{
		return socketOption(SO_KEEPALIVE, value);
	}
	
	bool Socket::setSendBufSize(int sendBufSize)
	{
		return socketOption(SO_SNDBUF, sendBufSize);
	} 
	
	bool Socket::setReceiveBufSize(int receiveBufSize)
	{
		return socketOption(SO_RCVBUF, receiveBufSize);
	}
	
	bool Socket::setSocketBlocking(bool block)
	{
		this->blocking = block;
		unsigned long blocking = (this->blocking) ? 0 : 1;
		
		#ifdef _WIN32
			if (ioctlsocket(socketId, FIONBIO, &blocking) == SOCKET_ERROR)
			{
				handleErrors(STAGE_OPTION, false);
				return false;
			}
		#else
			if (ioctl(socketId, O_NONBLOCK, (char*) &blocking) == SOCKET_ERROR)
			{
				handleErrors(STAGE_OPTION, false);
				return false;
			}
		#endif
		
		return true;
	}

	bool Socket::setReuseAddr(int value)
	{
		return socketOption(SO_REUSEADDR, value);
	} 
	
	int Socket::getReuseAddr()
	{
		socklen_t myOption;        
		socklen_t myOptionLen = sizeof(myOption);

		if (getsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, (char*) &myOption, &myOptionLen) == SOCKET_ERROR)
		{
			handleErrors(STAGE_OPTION, false);
			return SOCKET_ERROR;
		}
		return myOption;
	}

	int Socket::getKeepAlive()
	{
		socklen_t myOption;        
		socklen_t myOptionLen = sizeof(myOption);

		if (getsockopt(socketId, SOL_SOCKET, SO_KEEPALIVE, (char*) &myOption, &myOptionLen) == SOCKET_ERROR)
		{
			handleErrors(STAGE_OPTION, false);
			return SOCKET_ERROR;
		}
		return myOption;    
	}
	
	int Socket::getSendBufSize()
	{
		socklen_t sendBuf;
		socklen_t myOptionLen = sizeof(sendBuf);

		if (getsockopt(socketId, SOL_SOCKET, SO_SNDBUF, (char*) &sendBuf, &myOptionLen) == SOCKET_ERROR)
		{
			handleErrors(STAGE_OPTION, false);
			return SOCKET_ERROR;
		}
		return sendBuf;
	}    

	int Socket::getReceiveBufSize()
	{
		socklen_t rcvBuf;
		socklen_t myOptionLen = sizeof(rcvBuf);
		
		if (getsockopt(socketId, SOL_SOCKET, SO_RCVBUF, (char*) &rcvBuf, &myOptionLen) == SOCKET_ERROR)
		{
			handleErrors(STAGE_OPTION, false);
			return SOCKET_ERROR;
		}
		return rcvBuf;
	}
	
	// checks for errors, and fires onError event
	// no side-effects
	bool Socket::handleErrors(Socket::socket_stage_t stage, bool ignoreBlock)
	{
		#ifdef _WIN32
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK && ignoreBlock == true) return false;
			
			this->error = error;
	retryError:
			switch (this->error)
			{
				case 0: // apparently no error, try getting it with SO_ERROR instead
					; /* SCOPE */ {
						int retv = 0, retvlen = sizeof(retv);
						getsockopt(this->socketId, SOL_SOCKET, SO_ERROR, (char*) &retv, &retvlen);
						// avoid circular loops
						if (retv != 0) this->error = retv;
					}
					goto retryError;
					break;
				case WSA_INVALID_HANDLE:
					this->errorString = "Invalid handle";
					break;
				case WSA_NOT_ENOUGH_MEMORY:
					this->errorString = "Not enough memory";
					break;
				case WSA_INVALID_PARAMETER:
					this->errorString = "Invalid parameter";
					break;
				case WSA_OPERATION_ABORTED:
					this->errorString = "Operation aborted";
					break;
				case WSA_IO_INCOMPLETE:
					this->errorString = "IO incomplete (not yet completed)";
					break;
				case WSA_IO_PENDING:
					this->errorString = "IO pending (will complete later)";
					break;
				case WSAEINTR:
					this->errorString = "Interrupted function call";
					break;
				case WSAEBADF:
					this->errorString = "File handle is not valid";
					break;
				case WSAEACCES:
					this->errorString = "Permission denied";
					break;
				case WSAEFAULT:
					this->errorString = "Bad address";
					break;
				case WSAEINVAL:
					this->errorString = "Invalid argument";
					break;
				case WSAEMFILE:
					this->errorString = "Too many open sockets";
					break;
				case WSAEWOULDBLOCK:
					this->errorString = "Operation would block";
					break;
				case WSAEINPROGRESS:
					this->errorString = "Blocking operation already executing";
					break;
				case WSAEALREADY:
					this->errorString = "Operation already in progress";
					break;
				case WSAENOTSOCK:
					this->errorString = "Socket operation on non-socket";
					break;
				case WSAEDESTADDRREQ:
					this->errorString = "Destination address required";
					break;
				case WSAEMSGSIZE:
					this->errorString = "Message size too long";
					break;
				case WSAEPROTOTYPE:
					this->errorString = "Protocol wrong type for socket";
					break;
				case WSAENOPROTOOPT:
					this->errorString = "Unsupported or invalid protocol option";
					break;
				case WSAEPROTONOSUPPORT:
					this->errorString = "Protocol not supported";
					break;
				case WSAEOPNOTSUPP:
					this->errorString = "Operation not supported";
					break;
				case WSAEAFNOSUPPORT:
					this->errorString = "Address family not supported";
					break;
				case WSAEADDRINUSE:
					this->errorString = "Address already in use";
					break;
				case WSAEADDRNOTAVAIL:
					this->errorString = "Address not available";
					break;
				case WSAENETDOWN:
					this->errorString = "Network is down";
					break;
				case WSAENETUNREACH:
					this->errorString = "Network is unreachable";
					break;
				case WSAENETRESET:
					this->errorString = "Network dropped connection on reset";
					break;
				case WSAECONNABORTED:
					this->errorString = "Network caused connection abort";
					break;
				case WSAECONNRESET:
					this->errorString = "Connection reset by peer";
					break;
				case WSAENOBUFS:
					this->errorString = "No buffer space available";
					break;
				case WSAEISCONN:
					this->errorString = "Socket is already connected";
					break;
				case WSAENOTCONN:
					this->errorString = "Socket is not connected";
					break;
				case WSAESHUTDOWN:
					this->errorString = "Socket is shutting down";
					break;
				case WSAETOOMANYREFS:
					this->errorString = "Too many references";
					break;
				case WSAETIMEDOUT:
					this->errorString = "Connection timed out";
					break;
				case WSAECONNREFUSED:
					this->errorString = "Connection refused";
					break;
				case WSAELOOP:
					this->errorString = "Cannot translate name";
					break;
				case WSAENAMETOOLONG:
					this->errorString = "Name too long";
					break;
				case WSAEHOSTDOWN:
					this->errorString = "Host is down";
					break;
				case WSAEHOSTUNREACH:
					this->errorString = "No route to host";
					break;
				case WSAENOTEMPTY:
					this->errorString = "Directory not empty";
					break;
				case WSAEPROCLIM:
					this->errorString = "Too many processes";
					break;
				case WSAEUSERS:
					this->errorString = "User quota exceeded";
					break;
				case WSAEDQUOT:
					this->errorString = "Disk quota exceeded";
					break;
				case WSAESTALE:
					this->errorString = "Stale file handle reference";
					break;
				case WSAEREMOTE:
					this->errorString = "Item is remote";
					break;
				case WSASYSNOTREADY:
					this->errorString = "Network subsystem is unavailable";
					break;
				case WSAVERNOTSUPPORTED:
					this->errorString = "Requested winsock version unsupported";
					break;
				case WSANOTINITIALISED:
					this->errorString = "Winsock not yet initialized";
					break;
				case WSAEDISCON:
					this->errorString = "Graceful shutdown in progress";
					break;
				case WSAENOMORE:
					this->errorString = "No more results";
					break;
				case WSAECANCELLED:
					this->errorString = "Call has been cancelled";
					break;
				case WSAEINVALIDPROCTABLE:
					this->errorString = "Procedure call table is invalid";
					break;
				case WSAEINVALIDPROVIDER:
					this->errorString = "Service provider is invalid";
					break;
				case WSAEPROVIDERFAILEDINIT:
					this->errorString = "Service provider failed to initialize";
					break;
				case WSASYSCALLFAILURE:
					this->errorString = "System call failure";
					break;
				case WSASERVICE_NOT_FOUND:
					this->errorString = "Service not found";
					break;
				case WSATYPE_NOT_FOUND:
					this->errorString = "Class type not found";
					break;
				case WSA_E_NO_MORE:
					this->errorString = "No more results";
					break;
				case WSA_E_CANCELLED:
					this->errorString = "Call was cancelled";
					break;
				case WSAEREFUSED:
					this->errorString = "Database query was refused";
					break;
				case WSAHOST_NOT_FOUND:
					this->errorString = "Host not found";
					break;
				case WSATRY_AGAIN:
					this->errorString = "Nonauthoritative host not found";
					break;
				case WSANO_RECOVERY:
					this->errorString = "This is a nonrecoverable error";
					break;
				case WSANO_DATA:
					this->errorString = "Valid name, no data record of requested type";
					break;
					
				default:
					this->errorString = "Unknown error";
			}
		#else
			this->error = errno;
			this->errorString = strerror(errno);
		#endif
		
		// finally, fire error event
		if (this->onError) this->onError(this, stage, this->error, this->errorString);
		return true;
		
	} // handleErrors
	
	std::string Socket::stageToString(socket_stage_t stage)
	{
		switch (stage)
		{
			case Socket::STAGE_INIT:
				return "In socket initialization";
				
			case Socket::STAGE_CONNECT:
				return "In socket::connect()";
				
			case Socket::STAGE_DISCONNECT:
				return "In socket::disconnect()";
				
			case Socket::STAGE_POLL:
				return "In socket::poll()";
				
			case Socket::STAGE_LISTEN:
				return "In socket::listen()";
			case Socket::STAGE_ACCEPT:
				return "In socket::accept()";
				
			case Socket::STAGE_READ:
				return "In socket::read()";
				
			case Socket::STAGE_WRITE:
				return "In socket::write()";
				
			default:	
				return "In unknown socket stage";
		}
		
	}
	
	std::string Socket::resolve(std::string name, hosttype_t type)
	{
		if (Socket::init == false) Socket sock(nullptr);
		
		HostInfo hi(name, (HostInfo::hosttype_t) type);
		
		if (hi.good == false) return "";
		
		if (type == ADDRESS)
		{
			return std::string(hi.getHostName());
		}
		else return std::string(hi.getHostIPAddress());
	}
	
}
