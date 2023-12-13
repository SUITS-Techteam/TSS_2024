///////////////////////////////////////////////////////////////////////////////////
//                                   Headers
///////////////////////////////////////////////////////////////////////////////////

// Standard C Stuff
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Networking Headers
#include "network.h"

// Application
#include "server_data.h"

// Uncomment this for extra print statements
// #define VERBOSE_MODE 

///////////////////////////////////////////////////////////////////////////////////
//                      Helper Functions Declarations
///////////////////////////////////////////////////////////////////////////////////

bool continue_server();

///////////////////////////////////////////////////////////////////////////////////
//                               Main Functions
///////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    printf("Hello World\n\n");

    // Windows Specific Init
    #if defined(_WIN32)
    WSADATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d)){

        printf("Failed to initialize WSA\n");
        return -1;
    }
    #endif

    // ----------------- Begin Main Program Space -------------------------

    // Check for running in local host
    char hostname[16];
    char port[6] = "14141";
    if(argc > 1 && strcmp(argv[1], "--local") == 0){
        strcpy(hostname, "127.0.0.1");
    } else {
        get_ip_address(hostname);
    }
    printf("Launching Server at IP: %s:%s\n", hostname, port);

    SOCKET server = create_socket(hostname, port);

    // "Data Base" Data
    struct backend_data_t* backend = init_backend(backend);

    // Client connection Data
    struct client_info_t* clients = NULL;

    while(1){

        fd_set reads;
        reads = wait_on_clients(clients, server);

        // Server Listen Socket got a new message
        if(FD_ISSET(server, &reads)){

            // create a new client
            struct client_info_t* client = get_client(&clients, -1);

            // create client socket
            client->socket = accept(server, (struct sockaddr*) &client->address, &client->address_length);
            if(!ISVALIDSOCKET(client->socket)){
                fprintf(stderr, "accept() failed with error: %d", GETSOCKETERRNO());
            }

            #ifdef VERBOSE_MODE
            if(strcmp(get_client_address(client), hostname)){
                printf("New Connection from %s\n", get_client_address(client));

            }
            #endif

        }

        // Server-Client Socket got a new message
        struct client_info_t* client = clients;
        while(client){
            
            struct client_info_t* next_client = client->next;

            // Check if this client has pending request
            if(FD_ISSET(client->socket, &reads)){

                // Request too big
                if(MAX_REQUEST_SIZE <= client->received){
                    send_400(client);
                    drop_client(&clients, client);
                    client = next_client;
                    continue;
                }

                // read new bytes in
                int bytes_received = recv(client->socket, client->request + client->received, MAX_REQUEST_SIZE - client->received, 0);

                if(bytes_received < 1){
                    
                    #ifdef VERBOSE_MODE
                    fprintf(stderr, "Unexpected Disconnect from %s\n", get_client_address(client));
                    #endif
                    drop_client(&clients, client);
                } else {

                    client->received += bytes_received;
                    client->request[client->received] = 0;

                    // Find marker for the end of the header
                    char* q = strstr(client->request, "\r\n\r\n");
                    if(q){

                        // Received a GET Request
                        if(strncmp(client->request, "GET /", 5) == 0){
                            char* path = client->request + 4;
                            char* end_path = strstr(path, " ");

                            if(!end_path){
                                send_400(client);
                                drop_client(&clients, client);
                            } else {
                                *end_path = 0;
                                serve_resource(client, path);
                                #ifdef VERBOSE_MODE
                                // printf("serve_resource %s %s\n", get_client_address(client), path);
                                #endif
                                drop_client(&clients, client);
                            }
                        } // Received a POST Request 
                        else if(strncmp(client->request, "POST /", 6) == 0) {

                            // Get the size of the post request
                            if(client->message_size == -1){
                                char* request_content_size_ptr = strstr(client->request, "Content-Length: ");
                                if(request_content_size_ptr){
                                    request_content_size_ptr += strlen("Content-Length: ");
                                    client->message_size = atoi(request_content_size_ptr) + (q - client->request) + 4; // The size of the content plus size of the header
                                } else {
                                    // There is no content size
                                    send_400(client);
                                    drop_client(&clients, client);
                                }
                            } 
                            
                            if(client->received == client->message_size){

                                // all bytes loaded
                                char* request_content = strstr(client->request, "\r\n\r\n");
                                request_content += 4; // Jump to the beginning of the context
                            
                                #ifdef VERBOSE_MODE
                                printf("Received Post Content: \n%s\n", request_content);
                                #endif

                                if(!request_content){
                                    send_400(client);
                                    drop_client(&clients, client);
                                } else {
                                    if(update_resource(request_content, backend)){
                                        send_304(client);
                                    } else {
                                        send_400(client);
                                    }
                                    drop_client(&clients, client);
                                }
                            }

                        } // Received some other request
                        else {
                            send_400(client);
                            drop_client(&clients, client);
                        }
                    }
                }
            }

            client = next_client;
        }

        if(!continue_server()) {
            break;
        }

        // Simulate the variables
        simulate_backend(backend);

    }


    // Clean up
    printf("Clean up Database...\n");
    cleanup_backend(backend);

    printf("Closing Sockets...\n");
    CLOSESOCKET(server);
    printf("Cleaned up server listen sockets\n");
    int leftover_clients = 0;
    struct client_info_t* client = clients;
    while(client){
        drop_client(&clients, client);
        leftover_clients++;
        client = client->next;
    }
    printf("Cleaned up %d client sockets\n", leftover_clients);

    // ------------------ End Main Program Space -------------------------

    // Windows Specific Cleanup
    #if defined(_WIN32)
    WSACleanup();
    #endif

    printf("\nGoodbye World\n");
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////
//                             Helper Functions
///////////////////////////////////////////////////////////////////////////////////

bool continue_server(){

    // if the user presses the ENTER key, the program will end gracefully
    struct timeval select_wait;
    select_wait.tv_sec = 0;
    select_wait.tv_usec = 0; 
    fd_set console_fd;
    FD_ZERO(&console_fd);
    FD_SET(0, &console_fd);
    int read_files = select(1, &console_fd, 0,0, &select_wait);
    if(read_files > 0) {
        return false;
    } else {
        return true;
    }
}


