#include "Util.h"


template<typename T = int>
class Stack {

public:
    explicit Stack(ull_t capacity_ = capacity) : capacity_(capacity_) {
        capacity_ = (!capacity_ ? 1 : capacity_);
        buffer_ = new char[capacity_ * sizeof(T) + 2 * sizeof(int)];

        begin_canary_buffer_ = (int *) buffer_;
        *begin_canary_buffer_ = DEADBEEF;

        end_canary_buffer_ = (int *) (buffer_ + sizeof(int) + capacity_ * sizeof(T));
        *end_canary_buffer_ = DEADBEEF;

        buffer_ += sizeof(int);
        sum_ = count_checksum_();
    }

    ~Stack() {
        buffer_ -= sizeof(int);
        delete[] buffer_;
    }

    int begin_canary_buffer() const {
        return begin_canary_buffer_ ? *begin_canary_buffer_ : 0;
    }

    int end_canary_buffer() const {
        return end_canary_buffer_ ? *end_canary_buffer_ : 0;
    }

    ull_t get_size() const {
        return curr_size_;
    }

    ull_t get_capacity() const {
        return capacity_;
    }

    ull_t get_checksum() const {
        return sum_;
    }

    ull_t count_checksum_() const;

    int get_first_canary() const {
        return first_canary_;
    }

    int get_last_canary() const {
        return last_canary_;
    }

    void top(T &element);

    void pop();

    void push(T element);


private:
    void resize_();


private:
    int first_canary_ = DEADBEEF;

    int *begin_canary_buffer_ = nullptr;
    char *buffer_ = nullptr;
    int *end_canary_buffer_ = nullptr;
    ull_t capacity_;
    ull_t curr_size_ = 0;

    mutable ull_t sum_ = 0; // because we need to set 0, before counting it

    int last_canary_ = DEADBEEF;
};


template<typename T>
bool verify(Stack<T> *stack) {
    return stack &&
           stack->get_first_canary() == DEADBEEF &&
           stack->get_last_canary() == DEADBEEF &&
           stack->get_checksum() == stack->count_checksum_() &&
           stack->get_size() <= stack->get_capacity() &&
           stack->begin_canary_buffer() == DEADBEEF &&
           stack->end_canary_buffer() == DEADBEEF;
}


template<typename T>
void dump(Stack<T> *stack) {
    if (!stack) {
        std::cerr << "Pointer to stack is null\n";
        return;
    }

    if (stack->get_size() > stack->get_capacity()) {

        std::cerr << "Stack size is larger than its capacity\n";
    }

    if (stack->begin_canary_buffer() != DEADBEEF) {

        std::cerr << "Canary buffer protection was broken, got " <<
                  stack->begin_canary_buffer() << " instead of " << DEADBEEF << '\n';
    }

    if (stack->end_canary_buffer() != DEADBEEF) {

        std::cerr << "Canary buffer protection was broken, got " <<
                  stack->end_canary_buffer() << " instead of " << DEADBEEF << '\n';
    }

    if (stack->count_checksum_() != stack->get_checksum()) {

        std::cerr << "Stack checksum was crashed\n";
    }

    if (stack->get_size()) {

        std::cerr << "Elements which were in stack:\n";
        while (stack->get_size()) {
            T element;
            stack->top(element);
            std::cerr << element << ' ';
            stack->pop();
        }
    } else {

        std::cerr << "Stack was empty\n";
    }
}


template<typename T>
ull_t Stack<T>::count_checksum_() const {
    ull_t old_value = sum_;
    sum_ = 0;

    ull_t sum = 0;
    for (int i = 0; i < sizeof(*this); i++) {
        sum += ((char *) this)[i];
    }

    sum_ = old_value;
    return sum;
}


template<typename T>
void Stack<T>::top(T &element) {
    VERIFY;

    if (get_size() > 0) {
        auto ptr = (T *) (buffer_);
        element = *(ptr + curr_size_ - 1);
        VERIFY;
        return;
    }

    std::cerr << "Stack is empty\n";
}


template<typename T>
void Stack<T>::pop() {
    VERIFY;

    if (get_size() > 0) {
        --curr_size_;
        sum_ = count_checksum_();
        VERIFY;
        return;
    }

    std::cerr << "Popping from an empty stack\n";
}


template<typename T>
void Stack<T>::push(T element) {
    VERIFY;

    if (get_capacity() == get_size()) {
        resize_();
    }
    auto ptr = (T *) (buffer_);
    ptr += curr_size_++;
    *ptr = element;
    sum_ = count_checksum_();

    VERIFY;
}


template<typename T>
void Stack<T>::resize_() {
    capacity_ *= 2;

    auto *new_buffer = new char[capacity_ * sizeof(T) + 2 * sizeof(int)];

    begin_canary_buffer_ = (int *) new_buffer;
    *begin_canary_buffer_ = DEADBEEF;
    new_buffer += sizeof(int);

    memcpy(new_buffer, buffer_, (capacity_ / 2) * sizeof(T));

    end_canary_buffer_ = (int *) (new_buffer + sizeof(int) + capacity_ * sizeof(T));
    *end_canary_buffer_ = DEADBEEF;

    buffer_ -= sizeof(int);
    delete[] buffer_;
    buffer_ = new_buffer;
}
