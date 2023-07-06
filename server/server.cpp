#include "../socket.cpp"
#include "../app.cpp"


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
            std::wcout << "Accepted client" << std::endl;
            ZeroMemory(&client_info, sizeof(client_info));
        }
        std::wcout << std::endl << "_accept thread exit: TRUE" << std::endl;
    }

public:
    static const Type type = SERVER;

    explicit Server(
        const char* host,
        unsigned short port
    ): Socket(host, port) {
        if (bind(_socket, (sockaddr*)&server_info, sizeof(server_info)) != 0) {
            booted = false; throw 2;
        }
        std::wcout << "Bind: TRUE" << std::endl;

        if (listen(_socket, 1) != 0) {
            booted = false; throw 3;
        }
        std::wcout << "Listen: TRUE" << std::endl;

        std::thread(_accept, this).detach();
    }

    void _send(std::wstring& data) {
        std::wcout << std::endl << "Send data to " << clients.size() << " clients" << std::endl;
        for (unsigned short i = 0; i < clients.size(); i++) {
            std::wstring size     = std::to_wstring(data.size()),
                         new_data = size + std::wstring(DATA_LENGTH - size.size(), '*') + data;
            std::wcout << "Client " << i << ": " << new_data;
            std::wcout.clear();
            std::wcout << std::endl;

            if(
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