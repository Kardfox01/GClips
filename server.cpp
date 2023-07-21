#include "socket.cpp"
#include "app.cpp"


class Server final: public Socket {
    std::vector<SOCKET> clients;

    void _accept() {
        sockaddr_in client_info;
        int client_info_size = sizeof(client_info);
        SOCKET client;

        while (
            (client = accept(_socket, (sockaddr*)&client_info, &client_info_size)) &&
             client != INVALID_SOCKET
        ) {
            clients.push_back(client);
            std::cout << "Accepted client" << std::endl;
            ZeroMemory(&client_info, sizeof(client_info));
        }
        std::cout << std::endl << "_accept thread exit: TRUE" << std::endl;
    }

public:
    static const Type type = SERVER;

    explicit Server(
        const char* host,
        unsigned short port
    ): Socket(host, port) {
        if (
            bind(_socket, (sockaddr*)&server_info, sizeof(server_info)) != 0
        ) {
            booted = false; throw 2;
        }
        std::cout << "Bind: TRUE" << std::endl;

        if (listen(_socket, 1) != 0) {
            booted = false; throw 3;
        }
        std::cout << "Listen: TRUE" << std::endl;

        std::thread(_accept, this).detach();
    }

    void _send(std::wstring& data) noexcept {
        size_t clients_size = clients.size();
        std::cout << std::endl << "Send data to " << clients_size << " clients" << std::endl;
        for (size_t i = 0; i < clients_size; i++) {
            std::wstring size     = std::to_wstring(data.size()),
                         new_data = size + std::wstring(DATA_LENGTH - size.size(), '*') + data;

            if (
                send(clients[i], (char*)new_data.data(), new_data.size()*sizeof(WCHAR), 0) == SOCKET_ERROR
            )
                clients.erase(clients.begin() + i);
        }
    }
};


int main() {
    App<Server> app("GClips - Server");
    return 0;
}