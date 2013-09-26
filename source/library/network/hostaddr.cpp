#include "hostaddr.hpp"

#ifdef __linux__
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#else
    #include <winsock2.h>
#endif

namespace library
{
	
	HostInfo::HostInfo()
	{
		#ifdef __linux__
			openHostDb();
		#else
			char sName[HOST_NAME_LENGTH + 1];
			memset(sName, 0, sizeof(sName));
			gethostname(sName, HOST_NAME_LENGTH);
			
			hostPtr = gethostbyname(sName);
			
			good = (hostPtr != nullptr);
		#endif
	}
	
	HostInfo::HostInfo(std::string& hostname, HostInfo::hosttype_t type)
	{
		#ifdef UNIX
			searchHostDB = 0;
		#endif
		
		if (type == HOSTNAME)
		{
			// Retrieve host by name
			hostPtr = gethostbyname(hostname.c_str());
			
			good = (hostPtr != nullptr);
		}
		else if (type == ADDRESS)
		{	// Retrieve host by address
			// get entry
			unsigned long netAddr = inet_addr(hostname.c_str());
			if (netAddr == -1ul)
			{
				good = false;
				return;
			}
			// get pointer
			hostPtr = gethostbyaddr( (char*) &netAddr, sizeof(netAddr), AF_INET);
			if (hostPtr == nullptr)
			{
				good = false;
				return;
			}
		}
	}
	
	char* HostInfo::getHostIPAddress()
	{
		// the first address in the list of host addresses
		struct in_addr *addr_ptr = 
			(struct in_addr*) hostPtr->h_addr; //_list[0];
		
		// changed the address format to the Internet address in standard dot notation
		return inet_ntoa(*addr_ptr);
	}    
	
	// Retrieves the hosts name
	char* HostInfo::getHostName()
	{
		return hostPtr->h_name;
	}
	
#ifdef __linux__
	char HostInfo::getNextHost()
	{
		// winLog<<"UNIX getNextHost() is called...\n";
		// Get the next host from the database
		if (searchHostDB == 1)
		{
			 if ((hostPtr = gethostent()) == NULL)
				  return 0;
			 else
				  return 1;
		}
		return 0;
	}
#endif

}
