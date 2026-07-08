Настройка проекта для сборки в студии (выполняем команды в консоле в директории проекта):
> mkdir build
> cd build
> cmake -S .. -G "Visual Studio 17 2022" -A Win32

Можно открыть в Visual Studio файл CMakeLists.txt. Можно собирать проект через cmake в консоле.

Сборка проекта:
> cmake --build . -C Release
> cmake --build . -C Debug


Запуск тестов:
> cmake --build . --target test_lib
> cmake --build . --target test_src
> cmake --build . --target test_all
> ctest -C Release
> ctest -C Debug
