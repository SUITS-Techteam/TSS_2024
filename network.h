
///////////////////////////////////////////////////////////////////////////////////
//                              Platform Dependent Headers
///////////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)
    // Windows Networking
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <WS2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")


    #define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
    #define CLOSESOCKET(s) closesocket(s)
    #define GETSOCKETERRNO() (WSAGetLastError())
#else
    // Unix Networking
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <ifaddrs.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>

    #define SOCKET int
    #define ISVALIDSOCKET(s) ((s) >= 0)
    #define CLOSESOCKET(s) close(s)
    #define GETSOCKETERRNO() (errno)
#endif

///////////////////////////////////////////////////////////////////////////////////
//                                  Structs
///////////////////////////////////////////////////////////////////////////////////

#define MAX_REQUEST_SIZE 2047

struct client_info_t {
    socklen_t address_length;
    struct sockaddr_storage address;
    SOCKET socket;
    char request[MAX_REQUEST_SIZE+1];
    int received;
    int message_size;
    struct client_info_t* next;
};

///////////////////////////////////////////////////////////////////////////////////
//                                 Functions
///////////////////////////////////////////////////////////////////////////////////

void get_ip_address(char* hostname_out);

const char* get_content_type(const char* path);

SOCKET create_socket(char* hostname, char* port);

struct client_info_t* get_client(struct client_info_t** clients, SOCKET socket);

void drop_client(struct client_info_t** clients, struct client_info_t* client);

const char* get_client_address(struct client_info_t* client);

fd_set wait_on_clients(struct client_info_t* clients, SOCKET server);

void send_400(struct client_info_t* client);

void send_404(struct client_info_t* client);

void send_201(struct client_info_t* client);

void send_304(struct client_info_t* client);

void reset_client_request_buffer(struct client_info_t* client);

void serve_resource(struct client_info_t* client, const char* path);


