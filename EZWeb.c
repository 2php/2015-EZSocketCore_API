#include "EZWeb.h"
//------------------------------- push some html code to the back of an EZWeb_Resource which type element equal to EZWeb_Type-HTML -------------------------------
int Do_EZWebHTMLCodePushBack(struct EZWeb_ResourceController *pThis,struct EZWeb_Resource *HTMLFile,char *HtmlCode,int HtmlCodeLength)
{


}
//------------------------------- clear all Resources handle by EZWeb_ResourceInfo -------------------------------
/* NOTICE!! when this function execute , the Resource return from GetResourceByInfo function also disappear */
void Do_ClearResource(struct EZWeb_ResourceController *pThis)
{
    pThis->Count=0;
    int i;
    for(i=0 ; i<MaxResources ; i++)
    {
        if(pThis->Resources[i]!=NULL)
        {
            if(pThis->Resources[i]->data!=NULL)
            {
                free(pThis->Resources[i]->data);
                pThis->Resources[i]->data = NULL;
            }
            free(pThis->Resources[i]);
            pThis->Resources[i]=NULL;
        }
    }
}
//------------------------------- get a resource and turn it to EZWeb_Resource type from web root folder recursive  -------------------------------
struct EZWeb_Resource * Do_GetResourceByInfo(struct EZWeb_ResourceController *pThis,struct EZWeb_ResourceInfo info, int *Errorcode)
{
    printf(" Do_GetResourceByInfo : path=%s\n",info.url);
    *Errorcode = 0;
    FILE *fp = fopen(info.url,"rb");
    if(fp!=NULL)
    {
        fseek(fp,0,SEEK_END);
        int datalength=ftell(fp);
        fseek(fp,0,SEEK_SET);
        if(datalength>MaxResourcesSize)
        {
            *Errorcode = 13;
            return NULL;
        }
        struct EZWeb_Resource *Resource = (struct EZWeb_Resource *)malloc(sizeof(struct EZWeb_Resource));
        char *data = (char*)malloc(datalength*sizeof(char));
        int r = fread(data,1,datalength,fp);
        if(r!=datalength)
        {
            *Errorcode = 12;
            free(data);
            free(Resource);
            return NULL;
        }
        else
        {
            /* NOTICE!! if the user free this return Resource , the Resource of EZWeb_ResourceController handel also disappear */
            pThis->Count++;
            if(pThis->Count>MaxResources)
            {
                *Errorcode = 14;
                free(data);
                free(Resource);
                pThis->Count--;
                return NULL;
            }
            else
            {
                Resource->datalength = datalength;
                Resource->data=data;
                Resource->type = info.type;
                pThis->Resources[pThis->Count]=Resource;
                return Resource;
            }
        }
    }
    else
    {
        *Errorcode = 10;
        return NULL;
    }
}
//------------------------------- make C code in Object-oriented style   -------------------------------
struct EZWeb_ResourceController * EZWebResourceController_INIT()
{
    struct EZWeb_ResponseController * pThis;
    pThis = (struct EZWeb_ResponseController *)malloc(sizeof(struct EZWeb_ResponseController));
    if(!pThis)
        return NULL;
    memset(pThis, 0x0, sizeof(struct EZWeb_ResponseController));
};
//------------------------------- EZWeb_ResourceController Error Handling -------------------------------
void GetEZWebResourceControllerErrorMsg(int ERRORCODE,char *MSG,int maxlength)
{
    memset(MSG,0x0,maxlength);
    int e = ERRORCODE;
    switch(e)
    {
        /*    GetEZWebResourceControllerHandler    */
        case -2 :
        strcpy(MSG,"EZWebResourceController_INIT");
        break;

        /*    Do_GetResourceByInfo   */
        case 10 :
        strcpy(MSG,"GetResourceByInfo : File not exist");
        break;

        case 11 :
        strcpy(MSG,"GetResourceByInfo : Unknow file type");
        break;

        case 12 :
        strcpy(MSG,"GetResourceByInfo : Reading error");
        break;

        case 13 :
        strcpy(MSG,"GetResourceByInfo : File too large,please edit MaxResourcesSize in EZWeb.h");
        break;

        case 14 :
        strcpy(MSG,"GetResourceByInfo : Too many file in Resources buffer,please edit MaxResources in EZWeb.h");
        break;

        default :
        strcpy(MSG,"UNKNOW failed");
    }
}
//------------------------------- Get an EZWeb_ResourceController Handler ----------
struct EZWeb_ResourceController * GetEZWebResourceControllerHandler(int * ERRORCODE)
{
    struct EZWeb_ResourceController * pThis;
    pThis = (struct EZWeb_ResourceController *)malloc(sizeof(struct EZWeb_ResourceController));
    if(!pThis)
        return NULL;
    memset(pThis, 0x0, sizeof(struct EZWeb_ResourceController));
    pThis->GetResourceByInfo = Do_GetResourceByInfo;
    pThis->EZWeb_HTMLCode_PushBack = Do_EZWebHTMLCodePushBack;
    return pThis;
};





//------------------------------- add an EZWeb Resource to the ResponseController Response array -------------------------------
void Do_SetResponseResource( struct EZWeb_ResponseController *pThis , struct EZWeb_Resource *Resource)
{
    pThis->ResponseResource=Resource;
}
//------------------------------- make C code in Object-oriented style   -------------------------------
struct EZWeb_ResponseController * EZWebResponseController_INIT()
{
    struct EZWeb_ResponseController * pThis;
    pThis = (struct EZWeb_ResponseController *)malloc(sizeof(struct EZWeb_ResponseController));
    if(!pThis)
        return NULL;
    memset(pThis, 0x0, sizeof(struct EZWeb_ResponseController));
    pThis->SetResponseResource = Do_SetResponseResource;
    return pThis;
};
//------------------------------- EZWeb_ResponseController Error Handling -------------------------------
void GetEZWebResponseControllerErrorMsg(int ERRORCODE,char *MSG,int maxlength)
{
    memset(MSG,0x0,maxlength);
    int e = ERRORCODE;
    switch(e)
    {
        case -2 :
        strcpy(MSG,"EZWebResponseController_INIT");
        break;

        default :
        strcpy(MSG,"UNKNOW failed");
    }
}
//------------------------------- Get an EZWeb_ResponseController Handler -------------------------------
struct EZWeb_ResponseController * GetEZWebResponseControllerHandler(int * ERRORCODE)
{
    *ERRORCODE=0;
    struct EZWeb_ResponseController *Controller;
    Controller = EZWebResponseController_INIT();
    if(!Controller)
    {
        *ERRORCODE = -2;
        return NULL;
    }
    Controller->ResponseResource = NULL;
    return Controller;
    *ERRORCODE=0;
};
//------------------------------- EZWeb component MainLoop -------------------------------
int ServiceThread_EZWeb_Service(struct SERVICE *Client)
{
    printf("ServiceThread_EZWeb_Loop(%d)\n",Client->id);
    char RcvBuffer[MAX_RECV_BUFFER];
    char SndBuffer[MAX_SEND_BUFFER];
    int errorcode;
    char errormsg[1024];
    struct EZWeb_ResponseController *ResponseController = GetEZWebResponseControllerHandler(&errorcode);
    if(ResponseController==NULL)
    {
        memset(errormsg,0x0,1024);
        GetEZWebResponseControllerErrorMsg(errorcode,errormsg,1024);
        printf("%s",errormsg);
        return -1;
    }
    struct EZWeb_ResourceController *ResourceController = GetEZWebResourceControllerHandler(&errorcode);
    if(ResourceController==NULL)
    {
        memset(errormsg,0x0,1024);
        GetEZWebResourceControllerErrorMsg(errorcode,errormsg,1024);
        printf("%s",errormsg);
        return -1;
    }
    /* NOTICE!! This version is very week on buffer-overflow attack . if I have enough time , I will repair this in the next version */
    /* NOTICE!! This version is very week on directory traversal attack . if I have enough time , I will repair this in the next version */
    /* NOTICE!! I just recv once from the client fd . if browser request lager than MAX_RECV_BUFFER  , this program ignore part of request */
    memset(RcvBuffer,0x0,MAX_RECV_BUFFER);
    printf("Recv\n");
    int r = recv(Client->fd, RcvBuffer, MAX_RECV_BUFFER, 0);
    if(r<=0)
    {
        printf("recv already close/timeout ! disconnect to %s:%d\n",Client->From.ip,Client->From.port);
        return 0;
    }
    printf("Show\n");
    printf("-----------------------------------------\n");
    printf("%s",RcvBuffer);
    printf("-----------------------------------------\n");
    printf("length=%d\n",r);
    char webpath[MaxResourcePath];
    char realpath[MaxResourcePath];
    memset(webpath,0x0,MaxResourcePath);
    sscanf(RcvBuffer,"GET %s HTTP",webpath);
    printf("webpath=%s\n",webpath);
    if(strcmp(webpath,"/")==0)
    {
        memset(webpath,0x0,MaxResourcePath);
        strcpy(webpath,"./index.html");
    }
    memset(realpath,0x0,MaxResourcePath);
    sprintf(realpath,"./%s",webpath);
    printf("realpath=%s\n",realpath);
    char temp[MaxResourcePath];
    memcpy(temp,realpath,MaxResourcePath);
    char ignore[MaxResourcePath];
    int s;
    //remove all the "../" "./"  "xxx/" , after this block , out temp will become the fromat like "<filename>.<type>"
    do
    {
        s = sscanf(temp,"%[^/]/%s",ignore,temp);
    }while(s>1);
    char type[100];
    memset(type,0x0,100);
    struct EZWeb_ResourceInfo RequestFile;
    memset(&RequestFile,0x0,sizeof(struct EZWeb_ResourceInfo));
    strcpy(RequestFile.url,realpath);
    //now we get the <type>
    sscanf(temp,"%[^.].%s",ignore,type);
    if(strcmp(type,"html")==0||strcmp(type,"htm")==0 || strcmp(type,"HTML")==0||strcmp(type,"HTM")==0)
    {
        RequestFile.type=HTML;
    }
    else if(strcmp(type,"jpg")==0||strcmp(type,"jpeg")==0||strcmp(type,"JPG")==0||strcmp(type,"JPEG")==0)
    {
        RequestFile.type=JPEG;
    }
    else if(strcmp(type,"png")==0||strcmp(type,"PNG")==0)
    {
        RequestFile.type=PNG;
    }
    else if(strcmp(type,"gif")==0||strcmp(type,"GIF")==0)
    {
        RequestFile.type=GIF;
    }
    else if(strcmp(type,"doc")==0||strcmp(type,"docx")==0||strcmp(type,"DOC")==0||strcmp(type,"DOCX")==0)
    {
        RequestFile.type=MSWORD;
    }
    else if(strcmp(type,"txt")==0||strcmp(type,"TXT")==0)
    {
        RequestFile.type=TXT;
    }
    else if(strcmp(type,"pdf")==0||strcmp(type,"PDF")==0)
    {
        RequestFile.type=PDF;
    }
    else if(strcmp(type,"c")==0||strcmp(type,"cpp")==0)
    {
        RequestFile.type=SOURCECODE;
    }
    else
    {
        RequestFile.type=UNKNOW;
    }
    void (*EZWebService)(struct Address_and_Port,struct EZWeb_ResourceInfo ReqFile , \
                         struct EZWeb_ResourceController *ResourceCTL,struct EZWeb_ResponseController *RsponseCTL) \
                          =  Client->ServerMainLoop;
    printf("EZWebLoop\n");
    EZWebService(Client->From,RequestFile,ResourceController,ResponseController);
    printf("OK\n");
    char HttpContentType[100];
    memset(HttpContentType,0x0,100);
    if(ResponseController->ResponseResource != NULL)
    {
        switch(ResponseController->ResponseResource->type)
        {
            case GIF:
            strcpy(HttpContentType,"Content-Type: image/gif");
            break;

            case PNG:
            strcpy(HttpContentType,"Content-Type: image/png");
            break;

            case JPEG:
            strcpy(HttpContentType,"Content-Type: image/jpeg");
            break;

            case HTML:
            strcpy(HttpContentType,"Content-Type: text/html");
            break;

            case MSWORD:
            strcpy(HttpContentType,"Content-Type: application/msword");
            break;

            case PDF:
            strcpy(HttpContentType,"Content-Type: application/pdf");
            break;

            case TXT:
            case SOURCECODE:
            strcpy(HttpContentType,"Content-Type: text/plain");
            break;

            default:
            strcpy(HttpContentType,"");
            break;
        }

        int w;
        memset(SndBuffer,0x0,MAX_SEND_BUFFER);
        printf("Response 200 OK\n");
        sprintf(SndBuffer,"HTTP/1.1 200 OK\r\n%s\r\n\r\n",HttpContentType);
        //send http protocol header
        printf("%s\n",SndBuffer);
        w = send(Client->fd,SndBuffer,strlen(SndBuffer),0);
        if(w<0)
        {
            printf("send already close/timeout ! disconnect to %s:%d\n",Client->From.ip,Client->From.port);
            return 0;
        }
        printf("strlen=%d;w=%d\n",strlen(SndBuffer),w);
            //send content data

        printf("Response data\n");
        w = send(Client->fd,ResponseController->ResponseResource->data,ResponseController->ResponseResource->datalength,0);
        if(w<0)
        {
            printf("send already close/timeout ! disconnect to %s:%d\n",Client->From.ip,Client->From.port);
            return 0;
        }
        printf("datalen=%d;w=%d\n",ResponseController->ResponseResource->datalength,w);
        return 0;
    }
    else
    {
        printf("NOTICE : ResponseController->ResponseResource is NULL\n");
        printf("Response 404 NOT Found\n");
        memset(SndBuffer,0x0,MAX_SEND_BUFFER);
        sprintf(SndBuffer,"HTTP/1.0 404 not found\r\%s\r\n\r\n",HttpContentType);
        //send http protocol header
        int w = send(Client->fd,SndBuffer,strlen(SndBuffer),0);
        if(w<0)
        {
            printf("send already close/timeout ! disconnect to %s:%d\n",Client->From.ip,Client->From.port);
            return 0;
        }
    }
    return 0;
}



