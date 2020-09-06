#include<stdio.h>//Defines core input and output functions
#include<stdlib.h>//Defines numeric conversion functions, pseudo-random numbers generation functions, memory allocation, process control functions

#include<sys/types.h>//The <sys/types.h> header contains a number of basic derived types that should be used whenever appropriate, including select() functionality
#include<sys/socket.h>//The <sys/socket.h> header shall define the sockaddr structure

#include<netinet/in.h>//The <netinet/in.h> header shall define the sockaddr_in structure

#include<unistd.h>//for read write and close operations, also select() implementation
#include<string.h>//Defines string-handling functions

#include <arpa/inet.h>
int main(){
    int error_check;//this error variable is just an integer and a flag for us to check whether we have achieved our objective in each step or not
    


    //creating server socket
    int server_socket;
    server_socket=socket(AF_INET,SOCK_STREAM,0);//SOCK_STREAM is for tcp
    if(server_socket==-1)
        perror("error in socket creation");

    //defining server's address
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;          //ipv4
    server_address.sin_port=htons(7003);        //port to connect to
    server_address.sin_addr.s_addr=INADDR_ANY;  //this effectively gives 0


    //binding the socket to our specified IP & port
    error_check=bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));
    if (error_check==-1)
        perror("error in binding");

    
    //listening for client sockets
    error_check=listen(server_socket,5);
    //5 is the max no of allowed sockets
    if(error_check==-1)	
        perror("error in listening\n");

    
    /*AFTER LISTENING SERVER HAS FOUND THE CLIENT BUT NEEDS TO ACCEPT THE CONNECTION BY STORING THE ADDRESS OF THE CLIENT*/
    
    //fetching client's address to establish connection
    struct sockaddr_in client_address;
    socklen_t client =sizeof(client_address);    
    int connection_socket=accept(server_socket,(struct sockaddr*)&client_address,&client);//you can even do accept(server_socket,0,0); because there is no need of the other 2 parameters here
    /*  first argument is the file descriptor of the server_socket, 
        second argument is the pointer to the address of the client,
        third argument is the file descriptor to the client*/
    if(connection_socket==-1)
        perror("error in establishing connection with the client\n");

    char connection_msg[256];
    fd_set descp_set;
    while(1){
        FD_ZERO(&descp_set);//to remove all file descriptors from the set
		FD_SET(0,&descp_set);//setting the bit of the standard input descriptor
		FD_SET(connection_socket,&descp_set);//setting the bit for the connection socket from the client
        error_check=select(5,&descp_set,0,0,0);//monitoring multiple descriptors and waiting until anyone of the descriptors in the set is ready to be read for input in this case
        if(error_check==-1){
            printf("error in select operation");
            close(server_socket);
            close(connection_socket);
            return 0;
        }
        //now finding who is ready for input ; is it the stdin or the connection(client) socket
        
        //select() operation will clear fds of all descriptors other than the ones who are ready to be read
        
        //checking if standard input could be read
        else if(FD_ISSET(0,&descp_set)){
            fgets(connection_msg,sizeof(connection_msg),stdin);//get the message to give to the client from the terminal
            error_check=send(connection_socket,&connection_msg,sizeof(connection_msg),0);
            /*similar syntax to recv first argument is the connection socket(client) ;second argument is the variable in which the message received from client is to be stored
            third argument is the size of the received message;0 is a flag*/
            if(error_check==-1){
                perror("error msg: error in sending message to client\n\n");
                return 0;
            }
            if(strncmp(connection_msg,"bye",3)==0){//strncmp is used to compare only n characters
                close(server_socket);
                close(connection_socket);
                return 0;
            }
        }
        //checking if connection socket could be read
        else if(FD_ISSET(connection_socket,&descp_set)){
            error_check=recv(connection_socket,&connection_msg,sizeof(connection_msg),0);
            if(error_check==-1){
                perror("error msg:msg wasnt recieved from client\n");
                return 0;
            }
            if (strncmp(connection_msg,"bye",3)==0)
			{
				close(server_socket);
				close(connection_socket);
				return 0;
			}
            printf("cli-%s\n",connection_msg);
        }
        }
    close(server_socket);
    close(connection_socket);
    return 0;
    }
