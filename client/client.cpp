#include "../socket.cpp"


class Client final: public Socket {
    bool thread_flag = true;

    void _recv() {
        while (thread_flag) {
            std::wstring data_length(DATA_LENGTH, ' ');
            if (
                recv(_socket, (char*)data_length.data(), DATA_LENGTH*sizeof(WCHAR), 0) > 0
            ) {
                int size = stoi(data_length);
                std::wcout << DATA_LENGTH << std::endl;
                std::wstring data(size, ' ');
                recv(_socket, (char*)data.data(), size*sizeof(WCHAR), 0);
                std::wcout << data << std::endl;

                if (!OpenClipboard(NULL)) return;
                EmptyClipboard();
                HGLOBAL clipboard = GlobalAlloc(GMEM_DDESHARE, (data.length() + 1)*sizeof(WCHAR));
                LPWSTR clipboard_data = static_cast<LPWSTR>(GlobalLock(clipboard));
                wcscpy_s(clipboard_data, data.length() + 1, data.c_str());
                GlobalUnlock(clipboard);
                SetClipboardData(CF_UNICODETEXT, clipboard);
                CloseClipboard();
            } else break;
        }
        std::wcout << "_recv thread exit: TRUE" << std::endl;
    }

public:
    Client(
        const char* host,
        unsigned short port
    ): Socket(host, port) {}

    static const Type type = CLIENT;
    std::thread recv_thread;

    void start() {
        sockaddr_in server_info;
        Socket::start_socket(server_info);

        if (connect(_socket, (sockaddr*)&server_info, sizeof(server_info)) != 0) {
            booted = false; throw 4;
        }
        std::wcout << "Connected: TRUE" << std::endl;

        std::thread(_recv, this).detach();
    }

    ~Client() {
        std::wcout << "Client exit: TRUE" << std::endl;
    }
};