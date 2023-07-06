#include "../socket.cpp"
#include "../app.cpp"


class Client final: public Socket {
    bool thread_flag = true;

    void _recv() {
        std::wstring data_length(DATA_LENGTH, ' ');
        while (recv(_socket, (char*)data_length.data(), DATA_LENGTH*sizeof(WCHAR), 0) > 0) {
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

            data_length.clear();
        }
        std::wcout << "_recv thread exit: TRUE" << std::endl;
    }

public:
    static const Type type = CLIENT;

    Client(
        const char* host,
        unsigned short port
    ): Socket(host, port) {
        if (connect(_socket, (sockaddr*)&server_info, sizeof(server_info)) != 0) {
            booted = false; throw 4;
        }
        std::wcout << "Connected: TRUE" << std::endl;

        std::thread(_recv, this).detach();
    }
};


int main() {
    App<Client> app("GClips - Client");
    return 0;
}