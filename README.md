# GClips
[![Release](https://img.shields.io/github/v/release/Kardfox01/GClips.svg)](https://github.com/Kardfox01/GClips/releases/tag/v1.0.3) ![Windows](https://badgen.net/badge/windows10/builded/green?icon=github)
________________
**GClips** - открытый проект для создания общего буфера обмена между множеством компьютеров с ОС Windows 10.

Данное приложение отличается быстрой работой, минималистичным внешним видом и малейшей нагрузкой на систему. Благодаря всему этому проект подходит для старых/слабых компьютеров.
________________
## Запуск
Проект разделен на две версии приложений *server* и *client*. Исполняемые фалы приложений весят чуть более 3 Мб и хранятся в [релизе](https://github.com/Kardfox01/GClips/releases/).
## Принцип работы
Компьютер, который выступает в роли сервера, отслеживает изменения своего буфера обмена. При его изменении он немедленно отправляет данные из него всем подключенным клиентам.
## Компиляция
Проект был создан при помощи лишь одной сторонней библиотеки - `FLTK` - для пользовательского интерфейса и скомпилирован с помощью `MINGW64`.
Компиляция:
```
 g++ ______app.cpp -o st______app.exe -I/mingw64/include -march=x86-64 -mtune=generic -O2 -pipe -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -pipe -Wl,-subsystem,windows -mwindows /mingw64/lib/libfltk.a -lole32 -luuid -lcomctl32 -lws2_32 --static
```
