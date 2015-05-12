#ifndef _EZSOCKETCORE_H_
#define _EZSOCKETCORE_H_

#define ENABLE_SERVER
#define ENABLE_CLIENT
#define MAX_ACCEPT_AMOUNT 20
#define IP_MAX_LENGTH 24
#define FILEPATH_MAX_LENGTH 256
#define MAX_SEND_BUFFER 4096       //MAX_SEND_BUFFER  (4KB)
#define MAX_RECV_BUFFER 4096       //MAX_SEND_BUFFER  (4KB

#ifdef _WIN32
#include <winsock.h>
#include <windows.h>
#define Delay(second) Sleep(second*1000);
#define ENABLE_WINDOWS_SYSTEMCALL
#endif

#ifdef linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define Delay(second) sleep(second);
#define ENABLE_LINUX_SYSTEMCALL
#endif

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define Delay(second) sleep(second);
#define ENABLE_LINUX_SYSTEMCALL
#endif // __APPLE__

#ifndef ENABLE_WINDOWS_SYSTEMCALL
#ifndef ENABLE_LINUX_SYSTEMCALL
#define NOTSUPPORTOS
#endif
#endif // ENABLE_WINDOWS_SYSTEMCALL

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "EZUserdef.h"
#include "EZWeb.h"

struct Address_and_Port
{
		char ip[IP_MAX_LENGTH];
		int port;
};

struct SERVICE               /* 當一個Client連上時，會建立一個SERVICE */
{
        int id;
        bool finish;
        int fd;
        struct Address_and_Port From;
        int ServerMainLoop_Type;
        void (*ServerMainLoop);   //收到資料後該如何處理
        //void (*ServerMainLoop)(struct Address_and_Port,char*,int,char*,int);   //收到資料後該如何處理
};


#define ServerMainLoop_EZUserdef  0x01
#define ServerMainLoop_EZWeb      0x02

struct EZSocketCore
{
		struct sockaddr_in client_addr;
		struct sockaddr_in server_addr;
		int socket_id;
		int client_addrlen;
		int server_addrlen;
		int (*MK_Socket)( struct EZSocketCore *pThis );
		int (*MK_Bind)( struct EZSocketCore *pThis );
		int (*MK_Listen)( struct EZSocketCore *pThis );
		int (*MK_ClientAddress)( struct EZSocketCore *pThis , struct Address_and_Port Target);
		int (*MK_ServerAddress)( struct EZSocketCore *pThis , int Port);
		int (*MK_Connect)( struct EZSocketCore *pThis );
		int (*MK_Client_timeout)( struct EZSocketCore *pThis );
        void (*ServerMainLoop);
        void (*SetServerMainLoop)( struct EZSocketCore *pThis , int type , void *functionPtr);
        void (*StartServerForever)( struct EZSocketCore *pThis );
        int ServerMainLoop_Type;
        int (*WriteToServer)( struct EZSocketCore *pThis , char *buffer , int length );
		int (*ReadFromServer)( struct EZSocketCore *pThis , char *buffer , int maxlength );
};

#endif // _EZSOCKETCORE_H_
