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
    /* --------------------------------------------------------------------------------------------------
    *  Name:
    *  GetServerHandler : Get a server handler to control EZSocketcore's server function
    *
    *  Synopsis:
    *  #include "EZSocketCore.h"
    *  struct EZSocketCore * GetServerHandler(int port,int mode,(void*)function,int *errorcode);
    *
    *  Description:
    *  First parameter is the port which you want to bind.
    *  EZSocketCore support two mode on the Server program.
    *  You should pass ServerMainLoop_EZUserdef or ServerMainLoop_EZWeb (define in EZSocketCore.h) to
    *  second parameter and pass a function pointer to third parameter.
    *  You should pass a pointer which point to an integer to lastest parameter , when error occur,
    *  GetServerHandler set errorcode end return NULL.
    *  Watch example code to learn more about it.
    *
    *  Return Value
    *  On success, GetServerHandler return a Handler (a pointer to an EZSocketCore struct) to control
    *  functions of EZSocketcore server , Otherwise, NULL is returned and errorcode is set .
    *
    *  --------------------------------------------------------------------------------------------------
    *  Name:
    *  GetServerErrorMsg : Get the error message when GetServerHandler return NULL
    *
    *  Synopsis:
    *  #include "EZSocketCore.h"
    *  GetServerErrorMsg(int errorcode,char *message,int maxlength)
    *
    *  Description:
    *  When GetServerHandler return NULL , you can call GetServerErrorMsg to get error message .
    *  First parameter is the error code set by GetServerHandler , second parameter is a string pointer.
    *  Third parameter is the max length of the sring.
    *
    */
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
