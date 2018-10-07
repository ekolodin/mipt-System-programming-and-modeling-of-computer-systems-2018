#include <iostream>
#include <random>


typedef unsigned long long ull;


const int DEADBEEF = 0xDEADBEEF;


template<typename T = int>
class Stack {

public:
    explicit Stack(ull capacity_ = 1024) : capacity_(capacity_) {
        capacity_ = (!capacity_ ? 1 : capacity_);
        buffer_ = (char *) std::malloc(capacity_ * sizeof(T) + 2 * sizeof(int));

        begin_canary_ = (int *) buffer_;
        *begin_canary_ = DEADBEEF;

        end_canary_ = (int *) (buffer_ + sizeof(int) + capacity_ * sizeof(T));
        *end_canary_ = DEADBEEF;

        buffer_ += sizeof(int);

    }

    ~Stack();

    int begin_canary() const {
        return *begin_canary_;
    }

    int end_canary() const {
        return *end_canary_;
    }

    ull size() const;

    ull capacity() const;

    void top(T &element) const;

    void pop();

    void push(T element);


private:
    void resize_();


private:
    int *begin_canary_ = nullptr;
    char *buffer_ = nullptr;
    int *end_canary_ = nullptr;
    ull capacity_;
    ull curr_size_ = 0;
};


template<typename T>
bool verify(const Stack<T> *stack) {
    return stack->size() <= stack->capacity() &&
           stack->begin_canary() == DEADBEEF &&
           stack->end_canary() == DEADBEEF;
}


template<typename T>
void dump(const Stack<T> *stack) {
    if (stack->size() > stack->capacity()) {

        std::cerr << "Stack size is larger than its capacity\n";
    }

    if (stack->begin_canary() != DEADBEEF) {

        std::cerr << "Canary protection was broken, got " <<
                  stack->begin_canary() << " instead of " << DEADBEEF << '\n';
    }

    if (stack->end_canary() != DEADBEEF) {

        std::cerr << "Canary protection was broken, got " <<
                  stack->end_canary() << " instead of " << DEADBEEF << '\n';
    }
}


template<typename T>
Stack<T>::~Stack() {
    buffer_ -= sizeof(int);
    delete[] buffer_;
}


template<typename T>
ull Stack<T>::size() const {
    return curr_size_;
}


template<typename T>
ull Stack<T>::capacity() const {
    return capacity_;
}


template<typename T>
void Stack<T>::top(T &element) const {
    if (!verify(this)) {
        dump(this);
        return;
    }

    if (size() > 0) {
        auto ptr = (T *) (buffer_);
        element = *(ptr + curr_size_ - 1);
        return;
    }

    std::cerr << "Stack is empty\n";
}


template<typename T>
void Stack<T>::pop() {
    if (!verify(this)) {
        dump(this);
        return;
    }

    if (size() > 0) {
        --curr_size_;
        return;
    }

    std::cerr << "Popping from an empty stack\n";
}


template<typename T>
void Stack<T>::push(T element) {
    if (!verify(this)) {
        dump(this);
        return;
    }

    if (capacity() == size()) {
        resize_();
    }
    auto ptr = (T *) (buffer_);
    ptr += curr_size_++;
    *ptr = element;
}


template<typename T>
void Stack<T>::resize_() {
    capacity_ *= 2;

    auto *new_buffer = (char *) std::malloc(capacity_ * sizeof(T) + 2 * sizeof(int));

    begin_canary_ = (int *) new_buffer;
    *begin_canary_ = DEADBEEF;
    new_buffer += sizeof(int);

    memcpy(new_buffer, buffer_, (capacity_ / 2) * sizeof(T));

    end_canary_ = (int *) (new_buffer + sizeof(int) + capacity_ * sizeof(T));
    *end_canary_ = DEADBEEF;

    buffer_ -= sizeof(int);
    delete[] buffer_;
    buffer_ = new_buffer;
}


struct Tester {
    int x;
    double y;
    char c;
};


int main() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> rand_int(-1, 1);
    std::uniform_real_distribution<double> rand_double(0, 1);

    Stack<Tester> stack(1);
    for (int i = 0; i < 100; ++i) {
        stack.push({rand_int(generator), rand_double(generator), 'a'});
    }

    std::cout << "Now stack size is: " << stack.size() << '\n';

    for (int i = 0; i < 100; ++i) {
        Tester tester{0, 0, 0};
        stack.top(tester);
        std::cout << tester.x << ' ' << tester.y << ' ' << tester.c << '\n';
        stack.pop();
    }

    std::cout << "Now stack size is: " << stack.size() << '\n';

    return 0;
}