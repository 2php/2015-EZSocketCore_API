 /* WINDOWS :
 *        在編譯指令中加入 <-lwsock32>，或是IDE中加入Link Library <wsock32>
 *        在IDE中加入Search directories <JDK路徑\include 以及 JDK路徑\include\win32>
 *
 *
 */
#include "EZSocketCore.h"


void Loop(struct Address_and_Port From , char* RcvData , int RcvL , char *RspData , int *RspL)
{
    if(RcvL>0)
    {
        printf("Connect From %s:%d\n", From.ip , From.port);
        printf("Message:\n%s(%d)\n",RcvData,RcvL);
        strcpy(RspData,"<html>HelloWorld!</html>\nHTTP/1.1 200 OK");
        *RspL = strlen("<html>HelloWorld!</html>\nHTTP/1.1 200 OK");
    }
    else
    {
        *RspL = 0;
    }
}

void WebLoop(struct Address_and_Port From,struct EZWeb_ResourceInfo ReqFile , \
struct EZWeb_ResourceController *ResourceController,struct EZWeb_ResponseController *RsponseController)
{
    char errormsg[1024];
    int errorcode;
    printf("GetResourceByInfo\n");
    struct EZWeb_Resource *Resource = ResourceController->GetResourceByInfo(ResourceController,ReqFile,&errorcode);
    if(Resource==NULL)
    {
        GetEZWebResourceControllerErrorMsg(errorcode,errormsg,1024);
        printf("Error : %s\n",errormsg);
    }
    else
    {
        printf("SetResponseResource\n");
        RsponseController->SetResponseResource(RsponseController,Resource);
    }
};

int main(void)
{
/*
    struct Address_and_Port Target;
    int PORT = 80;  char *IP = "192.83.167.90";
	Target.port  =  PORT;
	memset(Target.ip , '\0' ,IP_MAX_LENGTH );
	strcpy(Target.ip ,   IP );
    int ERRORCODE;
    char ERRORMSG[1024];
    struct EZSocketCore * ClientHandler = GetClientHandler(Target,&ERRORCODE);
    if(ClientHandler==NULL)
    {
        GetClientErrorMsg(ERRORCODE,ERRORMSG,1024);
        printf("%s",ERRORMSG);
    }
    else
    {
        char buffer[40000];
        FILE *fin = fopen("test.txt","r");
        memset(buffer,0x0,40000);
        int s = fread(buffer,1,40000,fin);
        fclose(fin);
        printf("-----------------------------------------\n");
        printf("%s",buffer);
        printf("-----------------------------------------\n");
        int w = ClientHandler->WriteToServer(ClientHandler,buffer,strlen(buffer));
        memset(buffer,0x0,40000);
        int r = ClientHandler->ReadFromServer(ClientHandler,buffer,40000);
        if(r<0)
        {
            close(ClientHandler->socket_id);
            return;
        }
        printf("w=%d r=%d\n",w,r);
        printf("%s\n",buffer);
    }
    return 0;
*/


    int PORT = 9999;
    int ERRORCODE;
    char ERRORMSG[1024];
    struct EZSocketCore * ServerHandler = GetServerHandler(PORT,ServerMainLoop_EZWeb,WebLoop,&ERRORCODE);
    //struct EZSocketCore * ServerHandler = GetServerHandler(PORT,ServerMainLoop_EZUserdef,Loop,&ERRORCODE);
    if(ServerHandler==NULL)
    {
        GetServerErrorMsg(ERRORCODE,ERRORMSG,1024);
        printf("%s",ERRORMSG);
    }
    else
    {
        printf("InternetCoreV3 Server Create!\n");
        ServerHandler->StartServerForever(ServerHandler);
    }
    return 0;


}
