#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
static int i;

int main()
{
        int fd,op;
        int32_t value;
        char s[255],res[255];
        printf("*********************************\n");
        printf("\nOpening Driver\n");
        fd = open("/dev/Chr_Dev_device", O_RDWR);
        if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
}

while(1){
printf("enter '1'->enqueue '2'->dequeue '3'->count_queue_elements '4'->exit \n");
scanf("%d",&op);
       switch(op){
          case 1 :
               if(i<20){
                printf("enter the  string to write into the queue\n");
                scanf("%s",s);
                write(fd,s,strlen(s));
                i++;
                break;}
                else {printf("queue is full\n"); break;}
         case 2 :
                 read(fd,res,255);
                 i--;
                 if(strcmp(res," ")==0){
                        printf("queue is empty\n");  memset(res,0,strlen(res));}
                 else {
                        printf("the read string is ........%s\n",res); memset(res,0,strlen(res));}
                break;
         case 3 :
                ioctl(fd,RD_VALUE,(int32_t*)&value);
                printf("no. of elements cnt Value is,,,,, %d\n", value);
                break;
         case 4 :
                goto end;

}}
end :    {
                 printf("Closing Driver\n");
                 close(fd);}
}

