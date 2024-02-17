#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define MAX_NUM_OF_CONNECTIONS 5

int main(){
     char buffer[BUFFER_SIZE];

     // Step 1: Create the socket
     int server_socket = socket(AF_INET, SOCK_STREAM, 0);
     if (server_socket == -1){
          perror("socket creation returned -1");
          return -1;
     }
     printf("socket created succesfully!...\r\n");

     // Step 2: Bind socket to an address and port nr
     struct sockaddr_in server_address = {
         AF_INET,
         htons(SERVER_PORT),
         htonl(INADDR_LOOPBACK)
     };
     socklen_t server_addr_size = sizeof(server_address);
     int bind_retval = bind(server_socket, (struct sockaddr *)&server_address, server_addr_size);
     if (bind_retval < 0) {
          perror("socket binding unsuccesful");
          return -1;
     }
     printf("socket binding succesful!\n\r");

     // Step 3: Listening the bound socket for connections -- setting as passive socket
     int listen_retval = listen(server_socket, MAX_NUM_OF_CONNECTIONS);
     if (listen_retval < 0) {
          perror("listen unsuccesful");
          return -1;
     }
     printf("socket listen succesful!\n\r");

     // Step 4: accepting incoming connections --> Connection alive
     char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
     
     while(1) {
          int client_socket = accept(server_socket, (struct sockaddr *)&server_address, &server_addr_size);
          if (client_socket < 0){
               perror("accept unsuccesful");
               return -1;
          }
          printf("Client accepted!...\n\n\r");

          // Step 5: Reading client request and sending response
          int read_retval = read(client_socket, buffer, BUFFER_SIZE);
          if (read_retval < 0){
               perror("read unsuccesful");
               return -1;
          }
          printf(buffer);
          printf("\n\n\r");

          int write_retval = write(client_socket, response, sizeof(response));
          if (write_retval < 0){
               perror("write unsuccesful");
               return -1;
          }

          close(client_socket);
     }

     return 0;
}