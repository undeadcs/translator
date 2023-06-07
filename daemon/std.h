/**
 * 	Типы данных от стандартных
 *
 */

#ifndef STD_H
#define STD_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <string>
#include <stack>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <iconv.h>
#include <errno.h>
#include <signal.h>

#ifdef FOR_WIN32

#include <windows.h>
#include <psapi.h>
#include <winsock2.h>

//#define WINSOCK_VERSION 0x0101
#define WINSOCK_VERSION				MAKEWORD( 2, 2 )
#define GET_SOCKET_ERROR			( WSAGetLastError( ) )
#define CLOSE_SOCKET( hSocket )		{ closesocket( hSocket ); hSocket = INVALID_SOCKET; }

#else

#include <sys/socket.h>
#include <syslog.h>
#include <netinet/in.h>
#include <unistd.h>

typedef int SOCKET;

#define INVALID_SOCKET				( -1 )
#define SOCKET_ERROR				( -1 )
#define GET_SOCKET_ERROR			( errno )
#define CLOSE_SOCKET( hSocket )		{ close( hSocket ); hSocket = INVALID_SOCKET; }

#endif

using namespace std;

typedef basic_string< udDWord >				udStlStr;
typedef FILE								udStdFile, *udPStdFile, **udPPStdFile;

typedef vector< udStlStr		>			udArrWord;
typedef vector< UdSDK::udPNode	>			udArrPNode;

typedef map< udDWord,	udDWord			>	udMapWord;
typedef map< udDWord,	UdSDK::udPNode	>	udMapNode;
typedef map< udDWord,	UdSDK::udPTree	>	udMapTree;
typedef map< udDWord,	UdSDK::udPLog	>	udMapLog;
typedef map< string,	UdSDK::udPTree	>	udStrMapTree;

#endif
