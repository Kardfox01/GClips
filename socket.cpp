#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>

#pragma comment (lib, "Ws2_32.lib")

LPCWSTR ERRORS[] = {
    // SOCKET_ERRORS
    L"Ошибка при создании сокета",           // 0
    L"Неправильный IP-адрес",                // 1
    L"Ошибка попытки связки адреса",         // 2
    L"Ошибка во время прослушивания",        // 3
    L"Ошибка во время подключения",          // 4
    L"Ошибка при отправлении данных",        // 5
    L"Ошибка в получении сообщения",         // 6
};

enum Type {
    SERVER,
    CLIENT
};

class Socket {
protected:
    const short DATA_LENGTH = 10;

    SOCKET _socket;
    bool booted = false,
         thread_flag = true;

    const char* host;
    unsigned short port;

    Socket(const char* host, unsigned short port): host(host), port(port) {
        booted = boot();
        std::wcout << std::endl << "Host: " << host << std::endl;
        std::wcout << "Port: " << port << std::endl;
    }

    void start_socket(sockaddr_in& server_info) {
        if (!booted) throw 0;
        std::wcout << "Booted: TRUE" << std::endl;

        in_addr addr;
        if (inet_pton(AF_INET, host, &addr) <= 0) {
            booted = false; throw 1;
        }
        std::wcout << "Correct IP: TRUE" << std::endl;

        ZeroMemory(&server_info, sizeof(server_info));
        server_info.sin_family = AF_INET;
        server_info.sin_addr = addr;
        server_info.sin_port = htons(port);
    }

    bool boot() {
        WSADATA ws_data;
        if (WSAStartup(MAKEWORD(2, 2), &ws_data) != 0)
            return false;

        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == INVALID_SOCKET)
            return false;

        return true;
    }

public:
    static const unsigned short default_ip = 5001;

    static void get_host(std::string& ip) {
        WSADATA ws_data;
        struct hostent *host;
        char *host_name;
        struct in_addr addr;

        if (WSAStartup(MAKEWORD(2, 2), &ws_data) != 0) ip.append("ERROR");
        host = gethostbyname(std::getenv("COMPUTERNAME"));
        if (host != NULL)
            if (host->h_addrtype == AF_INET) {
                USHORT i = 0;
                while (host->h_addr_list[i++] != 0) {}
                if (host->h_addr_list[i] != 0) {
                    addr.s_addr = *(u_long*)host->h_addr_list[0];
                    ip.append(inet_ntoa(addr));
                }
            }
    }

    void _send(std::wstring& data) {}

    bool is_booted() {
        return booted;
    }

    ~Socket() {
        thread_flag = false;
        shutdown(_socket, SD_BOTH);
        closesocket(_socket);
        WSACleanup();
        std::wcout << "Socket exit: TRUE" << std::endl;
    }
};