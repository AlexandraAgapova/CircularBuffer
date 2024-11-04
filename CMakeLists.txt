cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)

# Добавляем поддиректорию с библиотекой и тестами
add_subdirectory(lib/circular-buffer)

# Указываем директорию заголовочных файлов для основного приложения
include_directories(lib/circular-buffer/include)

# Добавляем исполняемый файл основного приложения
add_executable(my_executable src/main.cpp)

# Линкуем основное приложение с библиотекой circular_buffer
target_link_libraries(my_executable circular_buffer)
