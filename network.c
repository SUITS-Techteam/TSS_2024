///////////////////////////////////////////////////////////////////////////////////
//                                  Headers
///////////////////////////////////////////////////////////////////////////////////

#include "network.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////
//                                 Functions
///////////////////////////////////////////////////////////////////////////////////

void get_ip_address(char* hostname_out){
    
    // initialize default hostname
    memset(hostname_out, 0, 16);
    strcpy(hostname_out, "127.0.0.1");

    // Get all addresses
    struct ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1) {
        printf("getifaddrs call failed\n");
        return;
    }

    // Loop through addresses and find a non local host IP4 address
    struct ifaddrs *address = addresses;
    while(address) {
        // skip if the address is NULL
        if (address->ifa_addr == NULL) { 
            address = address->ifa_next;
            continue;
        }

        // Check that it is IP4
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET) {

            char ap[100];
            const int family_size = sizeof(struct sockaddr_in);
            getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);

            // Check that it isn't local host
            if(strcmp(ap, "127.0.0.1") != 0){
                memset(hostname_out, 0, 16);
                strcpy(hostname_out, ap);
            }

        }
        address = address->ifa_next;
    }
    freeifaddrs(addresses);
}

const char* get_content_type(const char* path){
    const char* last_dot = strstr(path, ".");
    if(last_dot){
        if(strcmp(last_dot, ".css") == 0)  return "text/css"; 
        if(strcmp(last_dot, ".csv") == 0)  return "text/csv"; 
        if(strcmp(last_dot, ".gif") == 0)  return "image/gif"; 
        if(strcmp(last_dot, ".htm") == 0)  return "text/html"; 
        if(strcmp(last_dot, ".html") == 0) return "text/html"; 
        if(strcmp(last_dot, ".ico") == 0)  return "image/x-icon"; 
        if(strcmp(last_dot, ".jpeg") == 0) return "image/jpeg"; 
        if(strcmp(last_dot, ".jpg") == 0)  return "image/jpeg"; 
        if(strcmp(last_dot, ".js") == 0)   return "application/javascript"; 
        if(strcmp(last_dot, ".json") == 0) return "application/json"; 
        if(strcmp(last_dot, ".png") == 0)  return "image/png"; 
        if(strcmp(last_dot, ".pdf") == 0)  return "application/pdf"; 
        if(strcmp(last_dot, ".svg") == 0)  return "image/svg+xml"; 
        if(strcmp(last_dot, ".txt") == 0)  return "text/plain"; 
    }

    return "application/octet-stream";
}

SOCKET create_socket(char* hostname, char* port){
   
    printf("Configuring Local Address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;
    struct addrinfo *bind_address;
    if(getaddrinfo(hostname, port, &hints, &bind_address)){
        fprintf(stderr, "getaddrinfo() failed with error: %d", GETSOCKETERRNO());
        exit(1);
    }

    printf("Creating Socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if(!ISVALIDSOCKET(socket_listen)){
        fprintf(stderr, "socket() failed with error: %d", GETSOCKETERRNO());
        exit(1);
    }

    char c = 1;
    setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR, &c, sizeof(int));
    
    printf("Binding Socket...\n");
    if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)){
        fprintf(stderr, "bind() failed with error: %d", GETSOCKETERRNO());
        CLOSESOCKET(socket_listen);
        exit(1);
    }

    printf("Listening...\n");
    if(listen(socket_listen, 10) > 0){
        fprintf(stderr, "listen() failed with error: %d", GETSOCKETERRNO());
        exit(1);
    }
    freeaddrinfo(bind_address);

    return socket_listen;

}

struct client_info_t* get_client(struct client_info_t** clients, SOCKET socket){
    
    // look for the client in the linked list
    struct client_info_t* client = *clients;
    while(client){

        if(client->socket == socket){
            return client;
        }
        client = client->next;
    }

    // client not found, make one
    struct client_info_t* new_client = (struct client_info_t*) malloc(sizeof(struct client_info_t));
    if(new_client == NULL){
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    // Set struct to zero, this is mainly for the char request buffer (since 0 is the NULL Terminator)
    memset(new_client, 0, sizeof(struct client_info_t));

    // Initialize local variables
    new_client->address_length = sizeof(new_client->address);
    new_client->received = 0;
    new_client->message_size = -1;

    // Push to clients list
    new_client->next = *clients;
    *clients = new_client;

    return new_client;


}

void drop_client(struct client_info_t** clients, struct client_info_t* client){
    
    CLOSESOCKET(client->socket);

    struct client_info_t** p = clients;

    while(*p){
        if(*p == client){
            *p = client->next;
            free(client);
            return;
        }
        p = & ((*p)->next);
    }

    fprintf(stderr, "drop_client: client not found\n");
    exit(1);

}

const char* get_client_address(struct client_info_t* client){
    static char address_buffer[100];
    getnameinfo((struct sockaddr*) &client->address, client->address_length, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    return address_buffer;
}

fd_set wait_on_clients(struct client_info_t* clients, SOCKET server){

    struct timeval select_wait;
    select_wait.tv_sec = 0;
    select_wait.tv_usec = 100000; 

    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(server, &reads);
    SOCKET max_socket = server;

    // Push all the clients to reads
    struct client_info_t* client = clients;
    while(client){
        FD_SET(client->socket, &reads);
        if(client->socket > max_socket){
            max_socket = client->socket;
        }
        client = client->next;
    }

    // Wait on sockets
    if(select(max_socket+1, &reads, 0, 0, &select_wait) < 0){
        fprintf(stderr, "select() failed with error: %d", GETSOCKETERRNO());
        exit(1);
    }

    return reads;

}

void send_400(struct client_info_t* client){  
    
    const char* c400 = "HTTP/1.1 400 Bad Request\r\n"
        "Connection: close\r\n"
        "Content-Length: 10\r\n\r\nBadRequest";

    send(client->socket, c400, strlen(c400), 0);

}

void send_404(struct client_info_t* client){  
    
    const char* c404 = "HTTP/1.1 404 Not Found\r\n"
        "Connection: close\r\n"
        "Content-Length: 9\r\n\r\nNot Found";

    send(client->socket, c404, strlen(c404), 0);
    
}

void send_201(struct client_info_t* client){  
    
    const char* c201 = "HTTP/1.1 201 Created\r\n"
        "Connection: close\r\n"
        "Content-Length: 7\r\n\r\nCreated";

    send(client->socket, c201, strlen(c201), 0);
    
}

void send_304(struct client_info_t* client){  
    
    const char* c304 = "HTTP/1.1 304 Not Modified\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 12\r\n\r\nNot Modified";

    int bytes_sent = send(client->socket, c304, strlen(c304), 0);
}

void reset_client_request_buffer(struct client_info_t* client){
    // This allows a client to send a new request over the same socket
    client->received = 0;
    memset(client->request, 0, MAX_REQUEST_SIZE);
}

void serve_resource(struct client_info_t* client, const char* path){

    if(strcmp(path, "/") == 0){
        path = "/index.html";
    }

    if(strlen(path) > 100){
        send_400(client);
        return;
    }

    if(strstr(path, "..")){
        send_404(client);
        return;
    }

    char full_path[128];
    sprintf(full_path, "public%s", path);

    // Swap '/' to '\' on Windows
    #if defined(_WIN32)
    char *p = full_path;
    while(*p) {
        if(*p == '/'){
            *p = '\\';
        }
        p++;
    }
    #endif

    // Get the requested file
    FILE *fp = fopen(full_path, "rb");

    // if not found
    if(!fp){
        send_404(client);
        return;
    }

    // if found 
    fseek(fp, 0L, SEEK_END);
    size_t content_length = ftell(fp);
    rewind(fp);

    const char* content_type = get_content_type(full_path);

    #define BSIZE 1024

    char content_buffer[BSIZE];

    // Send Header
    sprintf(content_buffer, "HTTP/1.1 200 OK\r\n");
    send(client->socket, content_buffer, strlen(content_buffer), 0);

    sprintf(content_buffer, "Connection: close\r\n");
    send(client->socket, content_buffer, strlen(content_buffer), 0);

    sprintf(content_buffer, "Content-Length: %lu\r\n", content_length);
    send(client->socket, content_buffer, strlen(content_buffer), 0);

    sprintf(content_buffer, "Content-Type: %s\r\n", content_type);
    send(client->socket, content_buffer, strlen(content_buffer), 0);

    sprintf(content_buffer, "\r\n");
    send(client->socket, content_buffer, strlen(content_buffer), 0);

    // Send Content
    int bytes_read = fread(content_buffer, 1, BSIZE, fp);
    while(bytes_read){
        send(client->socket, content_buffer, bytes_read, 0);
        bytes_read = fread(content_buffer, 1, BSIZE, fp);
    }

    // Clean up
    fclose(fp);

}



