/*
*    For WINDOWS :
*    Add link library wsock32 to your IDE or add -lwsock32 to gcc option.
*/

#include "EZSocketCore.h"
#include <time.h>
void TimeLoop(struct Address_and_Port From,char *ReceiveData , int ReceiveLength , char * ResponseData , int * ResponseLength)
{
    if(ReceiveLength>0)
    {
        if(strstr(ReceiveData,"time?")!=NULL)
        {
            printf("Connect From %s:%d\n",From.ip,From.port);
            time_t now;
            time(&now);
            char timestr[100];
            memset(timestr,0x0,100);
            sprintf(timestr,"%s",ctime(&now));
            printf("Time=%s\n",timestr);
            strcpy(ResponseData,timestr);
            *ResponseLength = strlen(ResponseData);
        }
    }
};

int main(int argc,char **argv)
{
    int Port;
    if(argc<2)
    {
        Port = 9999;
        printf("Port=%d\n",Port);
    }
    else
    {
        Port = atoi(argv[1]);
        printf("Port=%d\n",Port);
    }
    int Errorcode;
    char ErrorMsg[1024];
    struct EZSocketCore * ServerHandler = GetServerHandler(Port,ServerMainLoop_EZUserdef,TimeLoop,&Errorcode);
    if(ServerHandler==NULL)
    {
        GetServerErrorMsg(Errorcode,ErrorMsg,1024);
        printf("%s",ErrorMsg);
    }
    else
    {
        printf("Time Server Create!\n");
        ServerHandler->StartServerForever(ServerHandler);
    }
    return 0;
}
