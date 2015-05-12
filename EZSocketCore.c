#include "EZSocketCore.h"
#define ENABLE_SERVER
#define ENABLE_CLIENT
int Do_Socket( struct EZSocketCore *pThis )
{
		if ( (pThis->socket_id = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
		return -1;
        return 0;
}

int Do_ClientAddress( struct EZSocketCore *pThis , struct Address_and_Port Target)
{
		pThis->client_addrlen = sizeof(pThis->client_addr);
		pThis->client_addr.sin_family = AF_INET;
        pThis->client_addr.sin_port = htons(Target.port);
        pThis->client_addr.sin_addr.s_addr = inet_addr(Target.ip);
        memset((pThis->client_addr.sin_zero),'\0',8);
		return 0;
}

int Do_ServerAddress( struct EZSocketCore *pThis , int Port)
{
		pThis->server_addrlen = sizeof(pThis->server_addr);
		pThis->server_addr.sin_family = AF_INET;
        pThis->server_addr.sin_port = htons(Port);
        pThis->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        memset((pThis->server_addr.sin_zero),'\0',8);
		return 0;
}

int Do_Bind( struct EZSocketCore *pThis )  //Server才需要BIND
{
		if ( bind(pThis->socket_id, (struct sockaddr*)&pThis->server_addr, pThis->server_addrlen) < 0 )
		return -1;
		return 0;
}

int Do_Listen( struct EZSocketCore *pThis )
{
		if ( listen(pThis->socket_id, MAX_ACCEPT_AMOUNT) < 0 )
		return -1;
		return 0;

}

int Do_Connect( struct EZSocketCore *pThis )  //只有Client才要connect
{
		if ( connect(pThis->socket_id, (struct sockaddr*)&pThis->client_addr, pThis->client_addrlen) < 0 )
		{
		return -1;
		}
		return 0;
}

int Do_Client_Timeout_Control( struct EZSocketCore *pThis )
{
        #ifdef ENABLE_WINDOWS_SYSTEMCALL
        int timeout_r = 2000; /*ms*/
        int timeout_w = 2000; /*ms*/
        if (setsockopt (pThis->socket_id, SOL_SOCKET, SO_RCVTIMEO,&timeout_r,sizeof(int)) < 0)
            return -1;
        if (setsockopt (pThis->socket_id, SOL_SOCKET, SO_SNDTIMEO,&timeout_w,sizeof(int)) < 0)
            return -1;
        return 0;
        #endif //ENABLE_WINDOWS_SYSTEMCALL

        #ifdef ENABLE_LINUX_SYSTEMCALL
        struct timeval timeout_r;
        struct timeval timeout_w;
        timeout_r.tv_sec = 2;
        timeout_r.tv_usec = 0;
        timeout_w.tv_sec = 2;
        timeout_w.tv_usec=0;
        if (setsockopt (pThis->socket_id, SOL_SOCKET, SO_RCVTIMEO,&timeout_r,sizeof(struct timeval )) < 0)
            return -1;
        if (setsockopt (pThis->socket_id, SOL_SOCKET, SO_SNDTIMEO,&timeout_w,sizeof(struct timeval )) < 0)
            return -1;
        return 0;
        #endif //ENABLE_LINUX_SYSTEMCALL
}

void Do_SetServerMainLoop( struct EZSocketCore *pThis , int type , void (*functionPtr))
{
        //由於傳進來的functionPtr已經是指標，所以不用使用&functionPtr
        pThis->ServerMainLoop_Type = type;
        pThis->ServerMainLoop = functionPtr;
}

#ifdef ENABLE_WINDOWS_SYSTEMCALL
DWORD WINAPI Do_StartServerForever_ServiceThread( void *data )
{
    struct SERVICE NewData;  /*產生新的實體，防止被修改*/
    memcpy(&NewData,data,sizeof(struct SERVICE));
    struct SERVICE *Client = &NewData;
    printf("Do_StartServerForever_ServiceThread(%d)...enter\n",Client->id);
    switch(Client->ServerMainLoop_Type)
    {
        case ServerMainLoop_EZUserdef:
            while(true)
            {
                ServiceThread_EZUserdef_Loop(Client);
            }
            close(Client->fd);
            break;

        case ServerMainLoop_EZWeb:
            ServiceThread_EZWeb_Service(Client);
            printf("Close socket\n");
            close(Client->fd);
            break;

        default:
            break;
    }
    printf("Do_StartServerForever_ServiceThread(%d)...exit\n",Client->id);
    return 0;

}
#endif // ENABLE_WINDOWS_SYSTEMCALL

#ifdef ENABLE_LINUX_SYSTEMCALL
void Do_StartServerForever_ServiceThread( struct SERVICE NewData )
{
    struct SERVICE *Client = &NewData;
    printf("Do_StartServerForever_ServiceThread(%d)...enter\n",Client->id);
    switch(Client->ServerMainLoop_Type)
    {
        case ServerMainLoop_EZUserdef:
            while(true)
            {
                ServiceThread_EZUserdef_Loop(Client);
            }
            close(Client->fd);
            break;

        case ServerMainLoop_EZWeb:
            ServiceThread_EZWeb_Service(Client);
            printf("Close socket\n");
            close(Client->fd);
            break;

        default:
            break;
    }
     printf("Do_StartServerForever_ServiceThread(%d)...exit\n",Client->id);
}
#endif // ENABLE_LINUX_SYSTEMCALL

void Do_StartServerForever( struct EZSocketCore *pThis)
{
        #ifdef ENABLE_WINDOWS_SYSTEMCALL
        int timeout_r = 30000; /*ms*/
        int timeout_w = 5000; /*ms*/
        if (setsockopt (pThis->socket_id, SOL_SOCKET, SO_RCVTIMEO,&timeout_r,sizeof(int)) < 0)
        {
            printf("setsockopt failed\n");
            return;
        }
        if (setsockopt (pThis->socket_id, SOL_SOCKET, SO_SNDTIMEO,&timeout_w,sizeof(int)) < 0)
        {
            printf("setsockopt failed\n");
            return;
        }
        #endif //ENABLE_WINDOWS_SYSTEMCALL
        #ifdef ENABLE_LINUX_SYSTEMCALL
        struct timeval timeout_r;
        struct timeval timeout_w;
        timeout_r.tv_sec = 30;
        timeout_r.tv_usec = 0;
        timeout_w.tv_sec = 5;
        timeout_w.tv_usec = 0;
        if (setsockopt (pThis->socket_id, SOL_SOCKET, SO_RCVTIMEO,&timeout_r,sizeof(struct timeval)) < 0)
        {
            printf("setsockopt failed\n");
            return;
        }
        if (setsockopt (pThis->socket_id, SOL_SOCKET, SO_SNDTIMEO,&timeout_w,sizeof(struct timeval)) < 0)
        {
            printf("setsockopt failed\n");
            return;
        }
        #endif


        int count=0;
        while(true)
        {
            struct sockaddr_in Their;
            int addrlen  = sizeof(struct sockaddr_in);
            int fd = accept(pThis->socket_id , (struct sockaddr*)&Their , &addrlen );
            if(fd>0)
            {
                struct SERVICE newClient;
                newClient.fd = fd;
                newClient.ServerMainLoop = pThis->ServerMainLoop;
                newClient.ServerMainLoop_Type = pThis->ServerMainLoop_Type;
                struct Address_and_Port From;
                char *ip = inet_ntoa(Their.sin_addr);
                strcpy(newClient.From.ip,ip);
                newClient.From.port = ntohs(Their.sin_port);
                #ifdef ENABLE_WINDOWS_SYSTEMCALL
                printf("CreateThread\n");
                newClient.id = ++count;
                HANDLE thread = CreateThread(NULL, 0, Do_StartServerForever_ServiceThread, &newClient, 0, NULL);
                #endif // ENABLE_WINDOWS_SYSTEMCALL*/

                #ifdef ENABLE_LINUX_SYSTEMCALL
                printf("fork...\n");
                newClient.id =  ++count;
                int service = fork();
                if(service==0)
                {
                    close(pThis->socket_id);
                    Do_StartServerForever_ServiceThread(newClient);
                    return;
                }
                else if(service<0)
                {
                    printf("fork failed\n");
                    return;
                }
                else
                {
                    close(fd);
                }
                #endif // ENABLE_LINUX_SYSTEMCALL

            }
        }
}

int Do_WriteToServer( struct EZSocketCore *pThis , char *buffer , int length)
{
        int s=0;
        if( (s = send(pThis->socket_id,buffer,length,0)) > 0)
            return s;
        else
            return -1;
}

int Do_ReadFromServer( struct EZSocketCore *pThis , char *buffer , int maxlength)
{
        int r=0;
        memset(buffer,0x0,maxlength);
        if( (r = recv(pThis->socket_id,buffer,maxlength,0)) > 0)
            return r;
        else
            return -1;
}

struct EZSocketCore * EZSocketCore_INIT()
{
        struct EZSocketCore * pThis;
		pThis = (struct EZSocketCore *)malloc(sizeof(struct EZSocketCore));
		if(!pThis)
            return NULL;
		memset(pThis, 0x0, sizeof(struct EZSocketCore));
		pThis->MK_Socket = Do_Socket;
		pThis->MK_Bind = Do_Bind;
		pThis->MK_ServerAddress = Do_ServerAddress;
		pThis->MK_ClientAddress = Do_ClientAddress;
		pThis->MK_Connect = Do_Connect;
		pThis->MK_Listen = Do_Listen;
		pThis->MK_Client_timeout = Do_Client_Timeout_Control;
		pThis->SetServerMainLoop = Do_SetServerMainLoop;
		pThis->StartServerForever = Do_StartServerForever;
		pThis->WriteToServer = Do_WriteToServer;
		pThis->ReadFromServer = Do_ReadFromServer;
		return pThis;
}
//==========================================================================================================================================

//==========================================================================================================================================
#ifdef ENABLE_SERVER
void GetServerErrorMsg(int ERRORCODE,char *MSG,int maxlength)
{
    memset(MSG,0x0,maxlength);
    int e = ERRORCODE;
    switch(e)
    {
        case -2 :
        strcpy(MSG,"EZSocketCore_INIT");
        break;

        case -1 :
        strcpy(MSG,"NOT SUPPORT OS");
        break;

        case 0 :
        strcpy(MSG,"CORRECT");
        break;

        case 1 :
        strcpy(MSG,"WSA failed");
        break;

        case 2 :
        strcpy(MSG,"SOCKET failed");
        break;

        case 3 :
        strcpy(MSG,"BIND failed");
        break;

        case 4 :
        strcpy(MSG,"LISTEN failed");
        break;

        default :
        strcpy(MSG,"UNKNOW failed");
    }

}

struct EZSocketCore * GetServerHandler( int Port , int type , void *functionPtr , int *ERRORCODE)
{
    *ERRORCODE=0;

        #ifdef NOTSUPPORTOS
        *ERRORCODE=-1;
        return NULL;
        #endif // NOTSUPPORTOS

        struct EZSocketCore *Core;
        Core = EZSocketCore_INIT();
        if(!Core)
        {
            *ERRORCODE = -2;
            return NULL;
        }


        Core->SetServerMainLoop(Core,type,functionPtr);
        *ERRORCODE=0;

        #ifdef ENABLE_WINDOWS_SYSTEMCALL
        WSADATA wsaData ;
        if( WSAStartup(MAKEWORD(1,1) , &wsaData ) != 0 )
        {
            *ERRORCODE = 1;
            return NULL;
        }
        #endif // ENABLE_WINDOWS_SYSTEMCALL

        if( (Core->MK_Socket(Core)) < 0 )
        {
            *ERRORCODE = 2;
            return NULL;
        }

        Core->MK_ServerAddress(Core,Port);

        if( (Core->MK_Bind(Core)) < 0 )
        {
            *ERRORCODE = 3;
            return NULL;
        }

        if( (Core->MK_Listen(Core)) < 0 )
        {
            *ERRORCODE = 4;
            return NULL;
        }

        return Core;
}
#endif // ENABLE_SERVER

#ifdef ENABLE_CLIENT
void GetClientErrorMsg(int ERRORCODE,char *MSG,int maxlength)
{
    memset(MSG,0x0,maxlength);
    int e = ERRORCODE;
    switch(e)
    {
        case -2 :
        strcpy(MSG,"EZSocketCore_INIT");
        break;

        case -1 :
        strcpy(MSG,"NOT SUPPORT OS");
        break;

        case 0 :
        strcpy(MSG,"CORRECT");
        break;

        case 1 :
        strcpy(MSG,"WSA failed");
        break;

        case 2 :
        strcpy(MSG,"SOCKET failed");
        break;

        case 3 :
        strcpy(MSG,"SET_TIMEOUT failed");
        break;

        case 4 :
        strcpy(MSG,"CONNECT failed");
        break;

        default :
        strcpy(MSG,"UNKNOW failed");
    }
}

struct EZSocketCore * GetClientHandler( struct Address_and_Port Target , int *ERRORCODE)
{
    *ERRORCODE=0;
    #ifdef NOTSUPPORTOS
    *ERRORCODE=-1;
        return NULL;
    #endif // NOTSUPPORTOS


    #ifdef ENABLE_WINDOWS_SYSTEMCALL
    WSADATA wsaData ;
	if( WSAStartup(MAKEWORD(1,1) , &wsaData ) != 0 )
	{
	    *ERRORCODE = 1;
	    return NULL;
    }
    #endif // ENABLE_WINDOWS_SYSTEMCALL

    struct EZSocketCore *Core;
    Core = EZSocketCore_INIT();
    if(!Core)
    {
        *ERRORCODE = -2;
        return NULL;
    }

	if( (Core->MK_Socket(Core)) < 0 )
    {
	    *ERRORCODE = 2;
	    return NULL;
    }

	Core->MK_ClientAddress(Core,Target);

	if( (Core->MK_Client_timeout(Core)) < 0 )
	{
	    *ERRORCODE = 3;
	    return NULL;
    }

	if( (Core->MK_Connect(Core)) < 0 )
    {
	    *ERRORCODE = 4;
	    return NULL;
    }

    return Core;
}
#endif // ENABLE_CLIENT
