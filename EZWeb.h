#ifndef _EZWEB_H_
#define _EZWEB_H_
#include "EZSocketCore.h"
#define MaxResourcePath 256
#define MaxRequireFile 1024
#define MaxResources 1024
#define MaxResourcesSize 1024*1024*10   //10MB

enum EZWeb_Type{HTML,PNG,JPEG,GIF,MSWORD,PDF,TXT,UNKNOW,SOURCECODE};
typedef enum EZWeb_Type EZWeb_Type;


struct EZWeb_ResourceInfo
{
    char url[FILEPATH_MAX_LENGTH];
    EZWeb_Type type;
};

struct EZWeb_Resource
{
    EZWeb_Type type;
    char *data;
    int datalength;
};

struct EZWeb_ResponseController
{
    struct EZWeb_Resource * ResponseResource;
    void (*SetResponseResource)( struct EZWeb_ResponseController *pThis,struct EZWeb_Resource *Resource );
};

struct EZWeb_ResourceController
{
    struct EZWeb_Resource * Resources[MaxResources];
    int Count;
    void (*ClearResource)( struct EZWeb_ResponseController *pThis);
    struct EZWeb_Resource * (*GetResourceByInfo)(struct EZWeb_ResourceController *pThis,struct EZWeb_ResourceInfo info,int *errorcode);
    int (*EZWeb_HTMLCode_PushBack)(struct EZWeb_ResourceController *pThis,struct EZWeb_Resource *HTMLFile,char *HtmlCode,int HtmlCodeLength);
};


#endif // _EZWEB_H_
