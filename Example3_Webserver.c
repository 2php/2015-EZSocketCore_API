/*
*    For WINDOWS :
*    Add link library wsock32 to your IDE or add -lwsock32 to gcc option.
*/

#include "EZSocketCore.h"

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
        RsponseController->SetResponseResource(RsponseController,Resource);
    }
};

int main(void)
{
    int Port = 9999;
    int Errorcode;
    char ErrorMsg[1024];
    struct EZSocketCore * ServerHandler = GetServerHandler(Port,ServerMainLoop_EZWeb,WebLoop,&Errorcode);
    if(ServerHandler==NULL)
    {
        GetServerErrorMsg(Errorcode,ErrorMsg,1024);
        printf("%s",ErrorMsg);
    }
    else
    {
        printf("Web Server Create!\n");
        ServerHandler->StartServerForever(ServerHandler);
    }
    return 0;
}
