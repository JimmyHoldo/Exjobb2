// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#define PORT 8080

void startNewDatagen(FILE *p1, int secs, int msesc)
{
    FILE *p = popen("pkill -f datagen", "r");
    pclose(p);

    char command2[100];
    sprintf(command2, "./../cpp_prototype/datagen %d %d", secs, msesc);

    p1 = popen(command2, "r");
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    FILE *p1 = NULL;
    char *datagenId = NULL;
    int i = 0;
    int timeSArr[] = {10, 5,2,1,0,0,0,0,0,0,0,0,0};
    int timeMsArr[] = {0,0,0,0,750,500,350,250,200,175,150,125,100};
    while(1)
    {
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        if(p1 != NULL){
            pclose(p1);
        }
        if(i == 13){
            break;
        }
        startNewDatagen(p1, timeSArr[i], timeMsArr[i]);

        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
        i++;
    }

    FILE *p = popen("pkill -f datagen", "r");
    pclose(p);
    return 0;
}
