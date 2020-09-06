#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>

#include<unistd.h>
#include<string.h>

#include <arpa/inet.h>// for inet_ntoa()
#include<netdb.h> // for hostent structure, gethostbyname()

int main(){
    int server_socket;
    int error_check;
    char server_message[256];
    //char ping[10];
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7005);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    error_check = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if(error_check == -1)
    {
        printf("Error while binding\n");
        exit(1);
    }
    
    struct sockaddr_in client_address;
    socklen_t client_len;
    client_len = sizeof(client_address);

    while(1){
        error_check = recvfrom(server_socket,server_message,sizeof(server_message),0, (struct sockaddr *) &client_address, &client_len);
        
        if(error_check == -1)
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
        
        error_check = sendto(server_socket,no,sizeof(no),0, (struct sockaddr *) &client_address, client_len);
        if(error_check == -1)
            {
                printf("error sending list size to client.\n");
                exit(1);
            }
        error_check = sendto(server_socket,buff,sizeof(buff),0, (struct sockaddr *) &client_address, client_len);
        if(error_check == -1)
            {
                printf("error sending list to client.\n");
                exit(1);
            }
        memset(server_message,'\0',strlen(server_message));

    }
    printf("Server shutdown.\n");
    close(server_socket);
    return 0;
    
    
}
