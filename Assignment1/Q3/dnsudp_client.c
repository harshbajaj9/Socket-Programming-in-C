#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>

#include<unistd.h>
#include<string.h>

#include <arpa/inet.h>

int main()
{
    int client_socket;
    int error_check;
    struct sockaddr_in client_address;
    socklen_t server_len;
    char client_message[256];

    server_len = sizeof(client_address);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_socket == -1)
    {
        printf("Error creating socket\n");
        exit(1);
    }

    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(7005);
    client_address.sin_addr.s_addr = INADDR_ANY;

    printf("input the domain\n");
    fgets(client_message,sizeof(client_message),stdin);
    int leng=strlen(client_message);
    int i;
    char conn_msg[leng];
    memset(conn_msg,'\0',strlen(conn_msg));
    for(i=0;i<leng-1;i++){
        conn_msg[i]=client_message[i];
    }
    conn_msg[leng-1]='\0';

    
    error_check = sendto(client_socket, conn_msg, sizeof(conn_msg),0, (struct sockaddr *) &client_address, server_len);
    if(error_check == -1)
    {
        printf("Error sending message\n");
        exit(1);
    }   

    long unsigned int no[1];
    error_check = recvfrom(client_socket, no, sizeof(no),0, (struct sockaddr *) &client_address, &server_len);
    if(error_check == -1)
    {
        printf("Error in receiving no of ip addresses\n");
        exit(1);
    }
    printf("%lu associated ip address(es) found\n",no[0]);
    char list_of_ips[no[0]][200];
    error_check = recvfrom(client_socket, list_of_ips, sizeof(list_of_ips),0, (struct sockaddr *) &client_address, &server_len);
    if(error_check == -1)
    {
        printf("Error in receiving ip addresses\n");
        exit(1);
    }

    
    
    i=0;
    while(i<no[0]){
        printf("%s\n",list_of_ips[i++]);
    }
    
    close(client_socket);
    return 0;

}
