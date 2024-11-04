# MyProject/lib/circular-buffer/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(circular_buffer_tests)

set(CMAKE_CXX_STANDARD 11)

# Подключаем директорию заголовочных файлов
include_directories(${PROJECT_SOURCE_DIR}/../include)

# Находим библиотеку GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Добавляем тестовый исполняемый файл
add_executable(runCircularBufferTests test_circular_buffer.cpp)

# Линкуем тесты с библиотекой circular_buffer и GTest
target_link_libraries(runCircularBufferTests circular_buffer ${GTEST_LIBRARIES} pthread)

# Добавляем тест
add_test(NAME CircularBufferTests COMMAND runCircularBufferTests)
