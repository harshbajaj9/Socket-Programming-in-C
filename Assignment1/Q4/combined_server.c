#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>
#include <string.h>

#include<signal.h>
#include<netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 
int main(){

    //tcp server----------------------------------------
     int server_socket1;
    int error_check1;    
    server_socket1 = socket(AF_INET, SOCK_STREAM, 0);//tcp server
    
    struct sockaddr_in server_address; 
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(7006); 

    //bind the socket to our specified IP & port
    error_check1=bind(server_socket1,(struct sockaddr*)&server_address,sizeof(server_address));
    if (error_check1==-1)
        perror("error in binding tcp socket");//printf("error msg: error in binding\n");
//listen for client sockets
    


    //udp server------------------------------------------
    int server_socket2;
    int error_check2;
    char server_message[256];
    //char ping[10];
    server_socket2 = socket(AF_INET, SOCK_DGRAM, 0);

    
    error_check2 = bind(server_socket2, (struct sockaddr *) &server_address, sizeof(server_address));
    if(error_check2 == -1)
    {
        printf("Error while binding udp socket\n");
        exit(1);
    }
    

    fd_set descp_set;
    
    
    
    
    
    FD_ZERO(&descp_set); 
    int maxfdp1 = max(server_socket1, server_socket2) + 1; //maximum file descriptors; can also be hardcoded
    while(1){
        FD_SET(server_socket1, &descp_set); 
        FD_SET(server_socket2, &descp_set);
        //setting fds for both cases
        // select the ready descriptor 
        error_check1= select(maxfdp1, &descp_set, 0, 0, 0); 
        
        
        if (FD_ISSET(server_socket1, &descp_set)) { 
            //len = sizeof(cliaddr); 
            error_check1=listen(server_socket1,5);
            //5 is the max no of allowed socket
            if(error_check1==-1)	
                perror("error msg:error in listening\n");
            char dateandtimebuffer[1025];
            time_t current_time; 
            
            int connection_socket1;
            connection_socket1 = accept(server_socket1, (struct sockaddr*)NULL, NULL); 
            current_time = time(NULL);
            snprintf(dateandtimebuffer, sizeof(dateandtimebuffer), "\a%s\n", ctime(&current_time));
            send(connection_socket1, dateandtimebuffer, strlen(dateandtimebuffer),0); 
            close(connection_socket1);
        }
        if (FD_ISSET(server_socket2, &descp_set)) { 
            struct sockaddr_in client_address;
            socklen_t client_len;
            client_len = sizeof(client_address);
            error_check2 = recvfrom(server_socket2,server_message,sizeof(server_message),0, (struct sockaddr *) &client_address, &client_len);
        
            if(error_check2 == -1)
            {
                printf("error receiving message from client.\n");
                exit(1);
            }

            printf("Client sent domain_name: %s\n", server_message);
            
            
            int l=strlen(server_message);
            
            
            struct hostent *hostinfo;
            char **address;

            hostinfo=gethostbyname(server_message);
            
            if(!hostinfo) {
                
                fprintf(stderr, "can not get info for the Host name %s\n",server_message);
                exit(1);
            }
            printf("Host name: %s \n",hostinfo->h_name);
            
            if(hostinfo->h_addrtype != AF_INET) {
                fprintf(stderr," Not a IP host \n");
                exit(1); 
            }

            address=hostinfo->h_addr_list;
            
            long unsigned int no[1];
            
            
            int i=0;
            
            
            while(*(address)){
                i++;
                address++;
            }
            no[0]=i;
            
            char buff[no[0]][200];
            i=0;
            address=hostinfo->h_addr_list;
            while(*(address)){
                snprintf(buff[i],"%s ",inet_ntoa(*(struct in_addr *)*address));
                
                
                i++;
                address++;
            }
            
            error_check2 = sendto(server_socket2,no,sizeof(no),0, (struct sockaddr *) &client_address, client_len);
            if(error_check2 == -1)
                {
                    printf("error sending list size to client.\n");
                    exit(1);
                }
            error_check2 = sendto(server_socket2,buff,sizeof(buff),0, (struct sockaddr *) &client_address, client_len);
            if(error_check2 == -1)
                {
                    printf("error sending list to client.\n");
                    exit(1);
                }
            memset(server_message,'\0',strlen(server_message));

        }

    }
}