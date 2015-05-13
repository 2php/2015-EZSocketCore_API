#include "EZUserdef.h"
int ServiceThread_EZUserdef_Loop(struct SERVICE *Client)
{

    char RcvBuffer[MAX_RECV_BUFFER];
    char SndBuffer[MAX_SEND_BUFFER];
    while(true)
    {
        memset(RcvBuffer,0x0,MAX_RECV_BUFFER);
        int r = recv(Client->fd, RcvBuffer, MAX_RECV_BUFFER, 0);
        int w = 0;
        if(r<0)
        {
            printf("recv already close/timeout ! disconnect to %s:%d\n",Client->From.ip,Client->From.port);
            return 0;
        }
        memset(SndBuffer,0x0,MAX_SEND_BUFFER);
        void (*EZUserdefLoop)(struct Address_and_Port,char*,int,char*,int*) =  Client->ServerMainLoop;   //收到資料後該如何處理

        EZUserdefLoop(Client->From,RcvBuffer,r,SndBuffer,&w);


        if(w>0)
        {
            if(w>MAX_SEND_BUFFER)
            {
                printf("data have not sent to %s:%d , w too large!\n",Client->From.ip,Client->From.port);
            }
            else
            {
                r = send(Client->fd,SndBuffer,w,0);
                if(r<0)
                {
                    printf("send already close/timeout ! disconnect to %s:%d\n",Client->From.ip,Client->From.port);
                    return 0;
                }
                else
                {
                    if(r!=w)
                    {
                        printf("only port of data have sent to %s:%d , %d/%d\n",Client->From.ip,Client->From.port,r,w);
                        return 0;
                    }
                }
            }
        }
        else
        {
            printf("no data to send...\n");
            return 0;
        }
        
    }
}

