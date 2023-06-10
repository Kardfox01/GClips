#include <windows.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/FL_Input.H>
#include <FL/FL_Return_Button.H>


void show_message(LPCWSTR message, long type = MB_ICONINFORMATION) {
    MessageBoxW(NULL, message, L"GCLips", type);
}

template<typename T>
class App {
    T* _socket = NULL;

public:
    App(const char* title) {
        SetConsoleOutputCP(65001);
        std::wcout << "COMPUTER: " << std::getenv("COMPUTERNAME") << std::endl;

        Fl_Window* window = new Fl_Window(340, 140, title);
        window->color(FL_WHITE);
        Fl::set_font(FL_HELVETICA, "helvetica.ttf");

        Fl_Input* ip_input = new Fl_Input(120, 30, 150, 25, "IP-адрес");
        ip_input->color(fl_rgb_color(230, 230, 230));
        ip_input->box(FL_BORDER_BOX);
        std::string ip;
        Socket::get_host(ip);
        ip_input->static_value(ip.c_str());

        Fl_Input* port_input = new Fl_Input(120, 60, 60, 25, "Порт");
        port_input->color(fl_rgb_color(230, 230, 230));
        port_input->box(FL_BORDER_BOX);
        port_input->static_value(std::to_string(Socket::default_ip).c_str());

        Fl_Return_Button* button = new Fl_Return_Button(20, 100, 300, 30, "Запустить");
        button->color(fl_rgb_color(230, 230, 230));
        button->box(FL_THIN_UP_BOX);
        button->callback([](Fl_Widget* widget, void* app_reference) {
            App* app = (App*)app_reference;
            const char* host = dynamic_cast<Fl_Input*>(widget->window()->child(0))->value();
            const char* port = dynamic_cast<Fl_Input*>(widget->window()->child(1))->value();
            Fl_Return_Button* button = (Fl_Return_Button*)widget;

            try {
                button->label("В процессе...");
                button->redraw();
                if (app->restart_socket(host, port)) {
                    if (T::type == SERVER) {
                        show_message(L"Сервер запущен");
                        Fl::remove_clipboard_notify(clipboard<T>);
                        Fl::add_clipboard_notify([](int, void* app_reference) {
                            ((App*)app_reference)->send();
                        }, app);
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
        delete _socket;
        _socket = new T(host, std::atoi(port));
        _socket->start();
        return _socket->is_booted();
    }

    ~App() {
        delete _socket;
        std::wcout << "App exit: TRUE" << std::endl;
    }
};