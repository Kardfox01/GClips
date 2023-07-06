#include <windows.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/FL_Input.H>
#include <FL/FL_Return_Button.H>
#include <FL/Fl_Input_Choice.H>


void show_message(LPCWSTR message, long type = MB_ICONINFORMATION) {
    MessageBoxW(NULL, message, L"GCLips", type);
}

template<typename T>
class App {
    T* _socket = nullptr;

    static void clipboard_notify(int, void* app_ptr) {
        static_cast<App*>(app_ptr)->send();
    }

public:
    App(const char* title) {
        SetConsoleOutputCP(65001);
        std::wcout << "COMPUTER: " << std::getenv("COMPUTERNAME") << std::endl;

        Fl_Window* window = new Fl_Window(340, 140, title);
        window->color(FL_WHITE);

        Fl_Input_Choice* host_input = new Fl_Input_Choice(120, 30, 150, 25, "IP-адрес");
        host_input->color(fl_rgb_color(230, 230, 230));
        host_input->box(FL_BORDER_FRAME);
        std::vector<const char*> hosts;
        Socket::get_hosts(hosts);
        for (const char* host: hosts)
            host_input->add(host);

        Fl_Input* port_input = new Fl_Input(120, 60, 60, 25, "Порт");
        port_input->color(fl_rgb_color(230, 230, 230));
        port_input->box(FL_BORDER_FRAME);
        port_input->static_value(Socket::default_ip);

        Fl_Return_Button* button = new Fl_Return_Button(20, 100, 300, 30, "Запустить");
        button->color(fl_rgb_color(230, 230, 230));
        button->box(FL_FLAT_BOX);
        button->callback([](Fl_Widget* widget, void* app_ptr) {
            App* app                 = static_cast<App*>(app_ptr);
            const char* host         = static_cast<Fl_Input_Choice*>(widget->window()->child(0))->value();
            const char* port         = static_cast<Fl_Input*>(widget->window()->child(1))->value();
            Fl_Return_Button* button = static_cast<Fl_Return_Button*>(widget);

            try {
                button->label("В процессе...");
                button->redraw();
                if (app->restart_socket(host, port)) {
                    if (T::type == SERVER) {
                        show_message(L"Сервер запущен");
                        Fl::add_clipboard_notify(clipboard_notify, app);
                    } else {
                        show_message(L"Подключение к серверу успешно");
                    }
                } else {
                    show_message(L"Несовместимая версия ОС/сокетов", MB_ICONERROR);
                }
            } catch (int error) {
                show_message(ERRORS[error]);
            }

           button->label("Перезапуск");
        }, this);

        window->show();
        Fl::run();
    }

    void send() {
        if (OpenClipboard(NULL)) {
            HANDLE hData = GetClipboardData(CF_UNICODETEXT);
            if (hData != NULL) {
                LPWSTR lpData = (LPWSTR)GlobalLock(hData);
                if (lpData != NULL) {
                    std::wstring text(lpData);
                    _socket->_send(text);
                    GlobalUnlock(hData);
                }
            }
            CloseClipboard();
        }
    }

    bool restart_socket(const char* host, const char* port) {
        if (_socket != nullptr)
            delete _socket;
        _socket = new T(host, std::atoi(port));
        return _socket->is_booted();
    }

    ~App() {
        delete _socket;
        std::wcout << "App exit: TRUE" << std::endl;
    }
};