#include <iostream>
#include <stdexcept>
#include <cstring>

typedef char value_type;

class CircularBuffer {
private:
    value_type* buffer;    // Pointer to the buffer array
    int cap;               // Capacity of the buffer
    int start;             // Index of the first element
    int end;               // Index of the last element
    int count;             // Number of elements in the buffer

    // Helper function to calculate the actual index in the buffer array
    int index(int i) const {
        return (start + i) % cap;
    }

public:
    // Default constructor
    CircularBuffer();

    // Destructor
    ~CircularBuffer();

    // Copy constructor
    CircularBuffer(const CircularBuffer& cb);

    // Constructs a buffer with a given capacity
    explicit CircularBuffer(int capacity);

    // Constructs a buffer with a given capacity and fills it with elem
    CircularBuffer(int capacity, const value_type& elem);

    // Access by index without bounds checking
    value_type& operator[](int i);
    const value_type& operator[](int i) const;

    // Access by index with bounds checking
    value_type& at(int i);
    const value_type& at(int i) const;

    // Reference to the first element
    value_type& front();
    const value_type& front() const;

    // Reference to the last element
    value_type& back();
    const value_type& back() const;

    // Linearizes the buffer so that the first element is at the beginning of allocated memory
    value_type* linearize();

    // Checks if the buffer is linearized
    bool is_linearized() const;

    // Rotates the buffer so that the element at new_begin becomes the first element
    void rotate(int new_begin);

    // Returns the number of elements stored in the buffer
    int size() const;

    // Checks if the buffer is empty
    bool empty() const;

    // Checks if the buffer is full (size == capacity)
    bool full() const;

    // Returns the number of free slots in the buffer
    int reserve() const;

    // Returns the capacity of the buffer
    int capacity() const;

    // Sets a new capacity for the buffer
    void set_capacity(int new_capacity);

    // Resizes the buffer
    // If the buffer is expanded, new elements are filled with item
    void resize(int new_size, const value_type& item = value_type());

    // Assignment operator
    CircularBuffer& operator=(const CircularBuffer& cb);

    // Swaps the contents of the buffer with another buffer
    void swap(CircularBuffer& cb);

    // Adds an element to the end of the buffer
    // If the buffer is full, the first element is overwritten
    void push_back(const value_type& item = value_type());

    // Adds a new element before the first element of the buffer
    // If the buffer is full, the last element is overwritten
    void push_front(const value_type& item = value_type());

    // Removes the last element of the buffer
    void pop_back();

    // Removes the first element of the buffer
    void pop_front();

    // Inserts an element at the specified position
    // The capacity of the buffer remains unchanged
    void insert(int pos, const value_type& item = value_type());

    // Erases elements in the range [first, last)
    void erase(int first, int last);

    // Clears the buffer
    void clear();
};

// Equality operators
bool operator==(const CircularBuffer& a, const CircularBuffer& b);
bool operator!=(const CircularBuffer& a, const CircularBuffer& b);

