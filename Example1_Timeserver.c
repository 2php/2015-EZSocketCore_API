/*
*    For WINDOWS :
*    Add link library wsock32 to your IDE or add -lwsock32 to gcc option.
*/

#include "EZSocketCore.h"
#include <time.h>
void TimeLoop(struct Address_and_Port From,char *ReceiveData , int ReceiveLength , char *ResponseData , int * ResponseLength)
{
    printf("Connect From %s:%d\n",From.ip,From.port);
	time_t now;
	time(&now);
	char timestr[100];
	memset(timestr,0x0,100);
	sprintf(timestr,"%s",ctime(&now));
	strcpy(ResponseData,timestr);
	*ResponseLength=strlen(ResponseData);
};

int main(void)
{
    int Port = 9999;
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
