/*
*    For WINDOWS :
*    Add link library wsock32 to your IDE or add -lwsock32 to gcc option.
*/

#include "EZSocketCore.h"

int main(int argc,char **argv)
{
    int Errorcode;
    char ErrorMsg[1024];
    struct Address_and_Port server;
    memset(&server,0x0,sizeof(struct Address_and_Port));
    if(argc==3)
    {
        strcpy(server.ip,argv[1]);
        server.port=atoi(argv[2]);
    }
    else
    {
        strcpy(server.ip,"127.0.0.1");
        server.port=9999;
    }
    struct EZSocketCore * ClientHandler = GetClientHandler(server,&Errorcode);
    if(ClientHandler==NULL)
    {
        GetClientErrorMsg(Errorcode,ErrorMsg,1024);
        printf("%s",ErrorMsg);
    }
    else
    {
        printf("Connect!\n");
        char buffer[100];
		memset(buffer,0x0,100);
		strcpy(buffer,"time?\n");
		ClientHandler->WriteToServer(ClientHandler,buffer,strlen(buffer));
		ClientHandler->ReadFromServer(ClientHandler,buffer,sizeof(buffer));
		ClientHandler->DisconnectToServer(ClientHandler);
		printf("Response From Server:\n%s\n",buffer);
    }
    return 0;
}
