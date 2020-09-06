#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>
#include <string.h>



#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
int main(int argc, char *argv[])
{
    int server_socket;
    int error_check;    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);//tcp server
    
    struct sockaddr_in server_address; 
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(7004); 

    //bind the socket to our specified IP & port
    error_check=bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));
    if (error_check==-1)
        perror("error in binding");//printf("error msg: error in binding\n");
//listen for client sockets
    error_check=listen(server_socket,5);
    //5 is the max no of allowed socket
    if(error_check==-1)	
        perror("error msg:error in listening\n");
    char dateandtimebuffer[1025];
    time_t current_time; 
    
    
    int connection_socket;
    while(1)
    {
        connection_socket = accept(server_socket, (struct sockaddr*)NULL, NULL);
        //the other 2 parameters of accept, as mentioned already in Q1, are not necessary to be stored here 

        current_time = time(NULL);

        snprintf(dateandtimebuffer, sizeof(dateandtimebuffer), "\a%s\n", ctime(&current_time));
        //ctime returns string representing the local time based on the argument current_time
        send(connection_socket, dateandtimebuffer, strlen(dateandtimebuffer),0); 

        close(connection_socket);
        //since the server is iterative therefore we perform an infinite loop and after every request we close the connection with the client and not the server socket
     }
}
