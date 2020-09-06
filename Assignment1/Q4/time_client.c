#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>
#include <string.h>



#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int connection_socket, n = 0;
    char databuffer[1024];
    struct sockaddr_in server_address; 

    if((connection_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7006); 
    server_address.sin_addr.s_addr =INADDR_ANY;

    if( connect(connection_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    n = recv(connection_socket, databuffer, sizeof(databuffer)-1,0);
    if(n==-1)
    {
        printf("\n error in receiving \n");
    } 
    printf("%s",databuffer);
    
    return 0;
}
