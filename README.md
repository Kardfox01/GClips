# GClips
[![Release](https://img.shields.io/github/v/release/Kardfox01/GClips.svg)](https://github.com/Kardfox01/GClips/releases/tag/v1.0.6) ![Windows](https://badgen.net/badge/windows10/builded/green?icon=github) ![Windows](https://badgen.net/badge/windows8.1/builded/green?icon=github) ![Windows](https://badgen.net/badge/windows8/builded/green?icon=github) ![Windows](https://badgen.net/badge/windows7/builded/green?icon=github) ![Windows](https://badgen.net/badge/windowsvista/builded/green?icon=github)
________________
**GClips** - открытый проект для создания общего буфера обмена между множеством компьютеров с ОС Windows 10.

Данное приложение отличается быстрой работой, минималистичным внешним видом и малейшей нагрузкой на систему. Благодаря всему этому проект подходит для старых/слабых компьютеров.
________________
## Запуск
Проект разделен на две версии приложений *server* и *client*. Исполняемые фалы приложений весят чуть более **1 Мб** и хранятся в [релизе](https://github.com/Kardfox01/GClips/releases/).
## Принцип работы
Компьютер, который выступает в роли сервера, отслеживает изменения своего буфера обмена. При его изменении он немедленно отправляет данные из него всем подключенным клиентам.
## Компиляция
Проект был создан при помощи лишь одной сторонней библиотеки - `FLTK` - для пользовательского интерфейса и скомпилирован с помощью `MINGW64`.
Компиляция:
```cmd
 g++ ______.cpp -o ______ -march=x86-64 -O2 -mwindows -lfltk -lws2_32 --static -s
```
или используя утилиту `fltk-config`:
```cmd
g++ server.cpp -o server `fltk-config --cxxflags --ldstaticflags` --static -s
```
