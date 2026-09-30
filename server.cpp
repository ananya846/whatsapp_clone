
/*
//we need the socket code used to connect process on one computer to the process on another computer on the network 
//bascially the number of the recieveing device is assigned to another number by the OS 
f_d= socket(int domain, int type, int protocol);

//the socket is then bound to an available port 
//since the computer is running multiple apps at the same time , its  important that the current app is running on an empty port 
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// our server should be constantly waiting to receieve the phoner number of the reciever 
listen(int sockfd, int backlog);
//once wee get the phopne number of the reiever we have to accept it 
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// we then recieve the message sent my one reciever 
//we also have to send the recieved message from one user to the other 


//so socket to make connection between processes, port so that this new process of connection can run on it , constant listening to wait for all the users who wanna do some action 
//accepting and then performing the action 
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
*/
#include <iostream>
#include <cstdio>          // perror
#include <sys/socket.h>    // socket, bind, listen, accept, send, recv
#include <netinet/in.h>    // sockaddr_in, htons
#include <unistd.h>        // close

int main() {
    // Get a phone. AF_INET = IPv4, SOCK_STREAM = TCP
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    // Lets you restart the server right away without "Address already in use"
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Fill in "my number": IPv4, any of my network cards, port 8080
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    std::cout << "ready\n";

    // YOUR PART (steps 3 and 4) goes here
    if(listen(server_fd,4 )<0)
    {
        perror("phone isnt listening because it aint on"); return 1;
    }

    int client_fd= accept(server_fd, nullptr, nullptr);
    //pick up the phone and give it a new socket number on your system
    if(client_fd!=-1)
    {
        std::cout << "someone connected!\n";
    }
    else
    {
                perror("client_phone "); return 1;

    }

    char buf[1024];
    while(true)
    {
        size_t n= recv(client_fd, buf, sizeof(buf), 0);
        if(n<0)
        {
            perror("issue recieving data from client");
        }
        if(n==0)
        {
            std::cout<<"client left";
            break;
        }
        send(client_fd, buf, n, 0);
        


    }
    close(client_fd);
    close(server_fd);
}