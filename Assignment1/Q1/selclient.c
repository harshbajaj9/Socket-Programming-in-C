#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>

#include<unistd.h>
#include<string.h>

#include <arpa/inet.h>

int main(){
    int error_check;
    


    //creating connection socket for client
    int connection_socket;
    connection_socket=socket(AF_INET,SOCK_STREAM,0);


        

    // specify an address for the socket to connect to
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7003);
    server_address.sin_addr.s_addr =INADDR_ANY;


    int connection_status=connect(connection_socket,(struct sockaddr*)&server_address,sizeof(server_address));
    if (connection_status == -1)
		printf("error in establishing connection with the server\n");
    
    
    char connection_msg[256];
    fd_set descp_set;
    while(1){
        FD_ZERO(&descp_set);//to remove all file descriptors from the set
		FD_SET(0,&descp_set);//setting the bit of the standard input descriptor
		FD_SET(connection_socket,&descp_set);//setting the bit for the connection socket from the client
        error_check=select(5,&descp_set,0,0,0);//monitoring multiple descriptors and waiting until anyone of the descriptors in the set is ready to be read for input in this case
        if(error_check==-1){
            printf("error in select operation");
            close(connection_socket);
            return 0;
        }
        else if(FD_ISSET(0,&descp_set)){//checking if standard input could be read
            
            fgets(connection_msg,sizeof(connection_msg),stdin);//get the message to give to the client from the terminal
            error_check=send(connection_socket,&connection_msg,sizeof(connection_msg),0);//similar syntax to recv
            
            if(error_check==-1){
                perror("error msg: error in sending message to server\n\n");
                return 0;
            }
            if(strncmp(connection_msg,"bye",3)==0){
            
                close(connection_socket);
                return 0;
            }
        }
        else if(FD_ISSET(connection_socket,&descp_set)){
            error_check=recv(connection_socket,&connection_msg,sizeof(connection_msg),0);
            if(error_check==-1){
                perror("error msg:msg wasnt recieved from server\n");
                return 0;
            }
            if(strncmp(connection_msg,"bye",3)==0){
            
                close(connection_socket);
                return 0;
            }
            printf("ser: %s\n", connection_msg);

        }
        
	
    }

    close(connection_socket);
    return 0;
    }
