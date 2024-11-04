#include "circular-buffer.h"

// Default constructor
CircularBuffer::CircularBuffer() : buffer(nullptr), cap(0), start(0), end(0), count(0) {}

// Destructor
CircularBuffer::~CircularBuffer() {
    delete[] buffer;
}

// Copy constructor
CircularBuffer::CircularBuffer(const CircularBuffer &cb)
    : cap(cb.cap), start(cb.start), end(cb.end), count(cb.count) {
    buffer = new value_type[cap];
    for (int i = 0; i < count; ++i) {
        buffer[index(i)] = cb.buffer[cb.index(i)];
    }
}

// Constructs a buffer with a given capacity
CircularBuffer::CircularBuffer(int capacity)
    : cap(capacity), start(0), end(0), count(0) {
    if (capacity < 0) {
        throw std::invalid_argument("Capacity must be non-negative");
    }
    buffer = new value_type[cap];
}

// Constructs a buffer with a given capacity and fills it with elem
CircularBuffer::CircularBuffer(int capacity, const value_type &elem)
    : cap(capacity), start(0), end(0), count(capacity) {
    if (capacity < 0) {
        throw std::invalid_argument("Capacity must be non-negative");
    }
    buffer = new value_type[cap];
    for (int i = 0; i < cap; ++i) {
        buffer[i] = elem;
    }
}

// Access by index without bounds checking
value_type &CircularBuffer::operator[](int i) {
    return buffer[index(i)];
}

const value_type &CircularBuffer::operator[](int i) const {
    return buffer[index(i)];
}

// Access by index with bounds checking
value_type &CircularBuffer::at(int i) {
    if (i < 0 || i >= count) {
        throw std::out_of_range("Index out of range");
    }
    return buffer[index(i)];
}

const value_type &CircularBuffer::at(int i) const {
    if (i < 0 || i >= count) {
        throw std::out_of_range("Index out of range");
    }
    return buffer[index(i)];
}

// Reference to the first element
value_type &CircularBuffer::front() {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer[start];
}

const value_type &CircularBuffer::front() const {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer[start];
}

// Reference to the last element
value_type &CircularBuffer::back() {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer[(end - 1 + cap) % cap];
}

const value_type &CircularBuffer::back() const {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    return buffer[(end - 1 + cap) % cap];
}

// Linearizes the buffer so that the first element is at the beginning of allocated memory
value_type *CircularBuffer::linearize() {
    if (is_linearized() || empty()) {
        return buffer;
    }

    auto *new_buffer = new value_type[cap];
    for (int i = 0; i < count; ++i) {
        new_buffer[i] = buffer[index(i)];
    }
    delete[] buffer;
    buffer = new_buffer;
    start = 0;
    end = count % cap;

    return buffer;
}

// Checks if the buffer is linearized
bool CircularBuffer::is_linearized() const {
    return start == 0 || empty();
}

// Rotates the buffer so that the element at new_begin becomes the first element
void CircularBuffer::rotate(int new_begin) {
    if (new_begin < 0 || new_begin >= count) {
        throw std::out_of_range("new_begin out of range");
    }
    int real_new_begin = index(new_begin);
    int offset = (real_new_begin - start + cap) % cap;
    start = real_new_begin;
    end = (start + count) % cap;
}

// Returns the number of elements stored in the buffer
int CircularBuffer::size() const {
    return count;
}

// Checks if the buffer is empty
bool CircularBuffer::empty() const {
    return count == 0;
}

// Checks if the buffer is full (size == capacity)
bool CircularBuffer::full() const {
    return count == cap;
}

// Returns the number of free slots in the buffer
int CircularBuffer::reserve() const {
    return cap - count;
}

// Returns the capacity of the buffer
int CircularBuffer::capacity() const {
    return cap;
}

// Sets a new capacity for the buffer
void CircularBuffer::set_capacity(int new_capacity) {
    if (new_capacity < 0) {
        throw std::invalid_argument("new_capacity must be non-negative");
    }
    if (new_capacity == cap) {
        return;
    }
    auto *new_buffer = new value_type[new_capacity];
    int new_count = std::min(count, new_capacity);
    for (int i = 0; i < new_count; ++i) {
        new_buffer[i] = buffer[index(i)];
    }
    delete[] buffer;
    buffer = new_buffer;
    cap = new_capacity;
    start = 0;
    count = new_count;
    end = count % cap;
}

// Resizes the buffer
// If the buffer is expanded, new elements are filled with item
void CircularBuffer::resize(int new_size, const value_type &item) {
    if (new_size < 0 || new_size > cap) {
        throw std::invalid_argument("new_size must be between 0 and capacity");
    }
    if (new_size < count) {
        // Shrinking the buffer
        count = new_size;
        end = (start + count) % cap;
    } else if (new_size > count) {
        // Expanding the buffer
        int additional = new_size - count;
        for (int i = 0; i < additional; ++i) {
            buffer[(end + i) % cap] = item;
        }
        count = new_size;
        end = (start + count) % cap;
    }
    // If sizes are equal, do nothing
}

// Assignment operator
CircularBuffer &CircularBuffer::operator=(const CircularBuffer &cb) {
    if (this != &cb) {
        auto *new_buffer = new value_type[cb.cap];
        for (int i = 0; i < cb.count; ++i) {
            new_buffer[cb.index(i)] = cb.buffer[cb.index(i)];
        }
        delete[] buffer;
        buffer = new_buffer;
        cap = cb.cap;
        start = cb.start;
        end = cb.end;
        count = cb.count;
    }
    return *this;
}

// Swaps the contents of the buffer with another buffer
void CircularBuffer::swap(CircularBuffer &cb) {
    std::swap(buffer, cb.buffer);
    std::swap(cap, cb.cap);
    std::swap(start, cb.start);
    std::swap(end, cb.end);
    std::swap(count, cb.count);
}

// Adds an element to the end of the buffer
// If the buffer is full, the first element is overwritten
void CircularBuffer::push_back(const value_type &item) {
    if (cap == 0) {
        throw std::runtime_error("Buffer capacity is zero");
    }
    buffer[end] = item;
    end = (end + 1) % cap;
    if (full()) {
        start = (start + 1) % cap;
    } else {
        ++count;
    }
}

// Adds a new element before the first element of the buffer
// If the buffer is full, the last element is overwritten
void CircularBuffer::push_front(const value_type &item) {
    if (cap == 0) {
        throw std::runtime_error("Buffer capacity is zero");
    }
    start = (start - 1 + cap) % cap;
    buffer[start] = item;
    if (full()) {
        end = (end - 1 + cap) % cap;
    } else {
        ++count;
    }
}

// Removes the last element of the buffer
void CircularBuffer::pop_back() {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    end = (end - 1 + cap) % cap;
    --count;
}

// Removes the first element of the buffer
void CircularBuffer::pop_front() {
    if (empty()) {
        throw std::runtime_error("Buffer is empty");
    }
    start = (start + 1) % cap;
    --count;
}

// Inserts an element at the specified position
// The capacity of the buffer remains unchanged
void CircularBuffer::insert(int pos, const value_type &item) {
    if (pos < 0 || pos > count) {
        throw std::out_of_range("Position out of range");
    }
    if (cap == 0) {
        throw std::runtime_error("Buffer capacity is zero");
    }
    if (full()) {
        throw std::runtime_error("Buffer is full");
    }
    // Shift elements to make room
    for (int i = count; i > pos; --i) {
        buffer[index(i)] = buffer[index(i - 1)];
    }
    buffer[index(pos)] = item;
    ++count;
    end = (start + count) % cap;
}

// Erases elements in the range [first, last)
void CircularBuffer::erase(int first, int last) {
    if (first < 0 || last > count || first >= last) {
        throw std::out_of_range("Invalid range");
    }
    int num_erased = last - first;
    // Shift elements to close the gap
    for (int i = first; i < count - num_erased; ++i) {
        buffer[index(i)] = buffer[index(i + num_erased)];
    }
    count -= num_erased;
    end = (start + count) % cap;
}

// Clears the buffer
void CircularBuffer::clear() {
    start = 0;
    end = 0;
    count = 0;
}

// Equality operators
bool operator==(const CircularBuffer &a, const CircularBuffer &b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const CircularBuffer &a, const CircularBuffer &b) {
    return !(a == b);
}
