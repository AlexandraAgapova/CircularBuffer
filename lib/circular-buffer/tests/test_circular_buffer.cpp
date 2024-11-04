#include <gtest/gtest.h>
#include "circular-buffer.h"

// Тестирование конструктора по умолчанию
TEST(CircularBufferTest, DefaultConstructor) {
    CircularBuffer cb;
    EXPECT_EQ(cb.size(), 0);
    EXPECT_EQ(cb.capacity(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_TRUE(cb.full());
}

// Тестирование конструктора с заданной ёмкостью
TEST(CircularBufferTest, CapacityConstructor) {
    CircularBuffer cb(5);
    EXPECT_EQ(cb.size(), 0);
    EXPECT_EQ(cb.capacity(), 5);
    EXPECT_TRUE(cb.empty());
    EXPECT_FALSE(cb.full());
}

// Тестирование конструктора с ёмкостью и заполнением
TEST(CircularBufferTest, FillConstructor) {
    CircularBuffer cb(3, 'x');
    EXPECT_EQ(cb.size(), 3);
    EXPECT_EQ(cb.capacity(), 3);
    EXPECT_FALSE(cb.empty());
    EXPECT_TRUE(cb.full());
    EXPECT_EQ(cb[0], 'x');
    EXPECT_EQ(cb[1], 'x');
    EXPECT_EQ(cb[2], 'x');
}

// Тестирование копирующего конструктора
TEST(CircularBufferTest, CopyConstructor) {
    CircularBuffer cb1(3, 'a');
    CircularBuffer cb2(cb1);
    EXPECT_EQ(cb2.size(), cb1.size());
    EXPECT_EQ(cb2.capacity(), cb1.capacity());
    EXPECT_TRUE(cb2.full());
    for (int i = 0; i < cb2.size(); ++i) {
        EXPECT_EQ(cb2[i], 'a');
    }
}

// Тестирование метода push_back
TEST(CircularBufferTest, PushBack) {
    CircularBuffer cb(3);
    cb.push_back('a');
    cb.push_back('b');
    cb.push_back('c');
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(cb.full());
    EXPECT_EQ(cb[0], 'a');
    EXPECT_EQ(cb[1], 'b');
    EXPECT_EQ(cb[2], 'c');

    // Переполнение буфера
    cb.push_back('d');
    EXPECT_EQ(cb.size(), 3);
    EXPECT_EQ(cb[0], 'b');
    EXPECT_EQ(cb[1], 'c');
    EXPECT_EQ(cb[2], 'd');
}

// Тестирование метода push_front
TEST(CircularBufferTest, PushFront) {
    CircularBuffer cb(3);
    cb.push_front('a');
    cb.push_front('b');
    cb.push_front('c');
    EXPECT_EQ(cb.size(), 3);
    EXPECT_TRUE(cb.full());
    EXPECT_EQ(cb[0], 'c');
    EXPECT_EQ(cb[1], 'b');
    EXPECT_EQ(cb[2], 'a');

    // Переполнение буфера
    cb.push_front('d');
    EXPECT_EQ(cb.size(), 3);
    EXPECT_EQ(cb[0], 'd');
    EXPECT_EQ(cb[1], 'c');
    EXPECT_EQ(cb[2], 'b');
}

// Тестирование методов pop_back и pop_front
TEST(CircularBufferTest, PopMethods) {
    CircularBuffer cb(3);
    cb.push_back('a');
    cb.push_back('b');
    cb.push_back('c');

    cb.pop_back();
    EXPECT_EQ(cb.size(), 2);
    EXPECT_EQ(cb[0], 'a');
    EXPECT_EQ(cb[1], 'b');

    cb.pop_front();
    EXPECT_EQ(cb.size(), 1);
    EXPECT_EQ(cb[0], 'b');

    cb.pop_back();
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());

    // Попытка удалить из пустого буфера
    EXPECT_THROW(cb.pop_front(), std::runtime_error);
    EXPECT_THROW(cb.pop_back(), std::runtime_error);
}

// Тестирование методов at и operator[]
TEST(CircularBufferTest, AccessMethods) {
    CircularBuffer cb(3);
    cb.push_back('x');
    cb.push_back('y');
    cb.push_back('z');

    EXPECT_EQ(cb.at(0), 'x');
    EXPECT_EQ(cb.at(1), 'y');
    EXPECT_EQ(cb.at(2), 'z');

    cb[1] = 'a';
    EXPECT_EQ(cb[1], 'a');

    // Проверка выхода за пределы
    EXPECT_THROW(cb.at(-1), std::out_of_range);
    EXPECT_THROW(cb.at(3), std::out_of_range);
}

// Тестирование метода front и back
TEST(CircularBufferTest, FrontBackMethods) {
    CircularBuffer cb(3);
    cb.push_back('a');
    cb.push_back('b');

    EXPECT_EQ(cb.front(), 'a');
    EXPECT_EQ(cb.back(), 'b');

    cb.push_back('c');
    EXPECT_EQ(cb.back(), 'c');

    // Проверка исключения для пустого буфера
    cb.clear();
    EXPECT_THROW(cb.front(), std::runtime_error);
    EXPECT_THROW(cb.back(), std::runtime_error);
}

// Тестирование метода linearize и is_linearized
TEST(CircularBufferTest, Linearize) {
    CircularBuffer cb(5);
    cb.push_back('a');
    cb.push_back('b');
    cb.push_back('c');
    cb.push_back('d');
    cb.pop_front(); // Теперь буфер не линейный
    cb.push_back('e');

    EXPECT_FALSE(cb.is_linearized());

    value_type* data = cb.linearize();
    EXPECT_TRUE(cb.is_linearized());
    EXPECT_EQ(data[0], 'b');
    EXPECT_EQ(data[1], 'c');
    EXPECT_EQ(data[2], 'd');
    EXPECT_EQ(data[3], 'e');
}

// Тестирование метода rotate
TEST(CircularBufferTest, Rotate) {
    CircularBuffer cb(5);
    cb.push_back('a');
    cb.push_back('b');
    cb.push_back('c');
    cb.push_back('d');
    cb.push_back('e');

    cb.rotate(2); // Теперь 'c' должен быть первым элементом
    EXPECT_EQ(cb[0], 'c');
    EXPECT_EQ(cb[1], 'd');
    EXPECT_EQ(cb[2], 'e');
    EXPECT_EQ(cb[3], 'a');
    EXPECT_EQ(cb[4], 'b');

    // Проверка исключения при неверном индексе
    EXPECT_THROW(cb.rotate(5), std::out_of_range);
}

// Тестирование метода swap
TEST(CircularBufferTest, Swap) {
    CircularBuffer cb1(3, 'a');
    CircularBuffer cb2(2, 'b');

    cb1.swap(cb2);

    EXPECT_EQ(cb1.capacity(), 2);
    EXPECT_EQ(cb1.size(), 2);
    EXPECT_EQ(cb1[0], 'b');
    EXPECT_EQ(cb1[1], 'b');

    EXPECT_EQ(cb2.capacity(), 3);
    EXPECT_EQ(cb2.size(), 3);
    EXPECT_EQ(cb2[0], 'a');
    EXPECT_EQ(cb2[1], 'a');
    EXPECT_EQ(cb2[2], 'a');
}

// Тестирование метода resize
TEST(CircularBufferTest, Resize) {
    CircularBuffer cb(5);
    cb.push_back('a');
    cb.push_back('b');
    cb.push_back('c');

    cb.resize(5, 'x');
    EXPECT_EQ(cb.size(), 5);
    EXPECT_EQ(cb[0], 'a');
    EXPECT_EQ(cb[1], 'b');
    EXPECT_EQ(cb[2], 'c');
    EXPECT_EQ(cb[3], 'x');
    EXPECT_EQ(cb[4], 'x');

    cb.resize(2);
    EXPECT_EQ(cb.size(), 2);
    EXPECT_EQ(cb[0], 'a');
    EXPECT_EQ(cb[1], 'b');

    // Проверка исключения при неверном размере
    EXPECT_THROW(cb.resize(6), std::invalid_argument);
}

// Тестирование метода set_capacity
TEST(CircularBufferTest, SetCapacity) {
    CircularBuffer cb(3, 'a');
    cb.set_capacity(5);
    EXPECT_EQ(cb.capacity(), 5);
    EXPECT_EQ(cb.size(), 3);
    cb.push_back('b');
    cb.push_back('c');
    EXPECT_EQ(cb.size(), 5);

    cb.set_capacity(2);
    EXPECT_EQ(cb.capacity(), 2);
    EXPECT_EQ(cb.size(), 2);
    EXPECT_EQ(cb[0], 'a');
    EXPECT_EQ(cb[1], 'a');
}

// Тестирование метода insert
TEST(CircularBufferTest, Insert) {
    CircularBuffer cb(5);
    cb.push_back('a');
    cb.push_back('b');
    cb.push_back('c');

    cb.insert(1, 'x');
    EXPECT_EQ(cb.size(), 4);
    EXPECT_EQ(cb[0], 'a');
    EXPECT_EQ(cb[1], 'x');
    EXPECT_EQ(cb[2], 'b');
    EXPECT_EQ(cb[3], 'c');

    // Проверка исключения при переполнении
    cb.insert(4, 'y');
    EXPECT_EQ(cb.size(), 5);
    EXPECT_THROW(cb.insert(5, 'z'), std::runtime_error);

    // Проверка исключения при неверной позиции
    EXPECT_THROW(cb.insert(-1, 'w'), std::out_of_range);
    EXPECT_THROW(cb.insert(6, 'w'), std::out_of_range);
}

// Тестирование метода erase
TEST(CircularBufferTest, Erase) {
    CircularBuffer cb(5, 'a'); // Буфер заполнен 'a'

    cb.erase(1, 3);
    EXPECT_EQ(cb.size(), 3);
    EXPECT_EQ(cb[0], 'a');
    EXPECT_EQ(cb[1], 'a');
    EXPECT_EQ(cb[2], 'a');

    // Проверка исключения при неверном диапазоне
    EXPECT_THROW(cb.erase(2, 1), std::out_of_range);
    EXPECT_THROW(cb.erase(-1, 2), std::out_of_range);
    EXPECT_THROW(cb.erase(0, 4), std::out_of_range);
}

// Тестирование метода clear
TEST(CircularBufferTest, Clear) {
    CircularBuffer cb(3, 'a');
    cb.clear();
    EXPECT_EQ(cb.size(), 0);
    EXPECT_TRUE(cb.empty());
    EXPECT_FALSE(cb.full());
}

// Тестирование оператора присваивания
TEST(CircularBufferTest, AssignmentOperator) {
    CircularBuffer cb1(3, 'a');
    CircularBuffer cb2(2, 'b');
    cb2 = cb1;

    EXPECT_EQ(cb2.capacity(), 3);
    EXPECT_EQ(cb2.size(), 3);
    EXPECT_EQ(cb2[0], 'a');
    EXPECT_EQ(cb2[1], 'a');
    EXPECT_EQ(cb2[2], 'a');
}

// Тестирование операторов сравнения
TEST(CircularBufferTest, EqualityOperators) {
    CircularBuffer cb1(3, 'a');
    CircularBuffer cb2(3, 'a');
    CircularBuffer cb3(3, 'b');

    EXPECT_TRUE(cb1 == cb2);
    EXPECT_TRUE(cb1 != cb3);

    cb2.push_back('b'); // Переписывает первый элемент
    EXPECT_TRUE(cb1 != cb2);
}

// Тестирование исключений в конструкторах
TEST(CircularBufferTest, ConstructorExceptions) {
    EXPECT_THROW(CircularBuffer cb(-1), std::invalid_argument);
    EXPECT_THROW(CircularBuffer cb(-5, 'x'), std::invalid_argument);
}

// Тестирование методов с некорректными аргументами
TEST(CircularBufferTest, MethodExceptions) {
    CircularBuffer cb(3);

    // Доступ по неверному индексу
    EXPECT_THROW(cb.at(0), std::out_of_range);
    cb.push_back('a');
    EXPECT_NO_THROW(cb.at(0));
    EXPECT_THROW(cb.at(1), std::out_of_range);

    // Установка ёмкости в неверное значение
    EXPECT_THROW(cb.set_capacity(-1), std::invalid_argument);

    // Ресайз до неверного размера
    EXPECT_THROW(cb.resize(-1), std::invalid_argument);
    EXPECT_THROW(cb.resize(5), std::invalid_argument);

    // Поворот на неверный индекс
    EXPECT_THROW(cb.rotate(-1), std::out_of_range);
    EXPECT_THROW(cb.rotate(2), std::out_of_range);
}

// Тестирование переполнения при push_back и push_front
TEST(CircularBufferTest, OverwriteBehavior) {
    CircularBuffer cb(3);
    cb.push_back('a');
    cb.push_back('b');
    cb.push_back('c');

    // Переполнение при push_back
    cb.push_back('d'); // 'a' будет переписан
    EXPECT_EQ(cb.size(), 3);
    EXPECT_EQ(cb[0], 'b');
    EXPECT_EQ(cb[1], 'c');
    EXPECT_EQ(cb[2], 'd');

    // Переполнение при push_front
    cb.push_front('e'); // 'd' будет переписан
    EXPECT_EQ(cb.size(), 3);
    EXPECT_EQ(cb[0], 'e');
    EXPECT_EQ(cb[1], 'b');
    EXPECT_EQ(cb[2], 'c');
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
