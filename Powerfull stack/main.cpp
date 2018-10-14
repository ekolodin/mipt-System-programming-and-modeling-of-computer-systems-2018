#include <iostream>
#include <random>
#include "Stack.h"


struct Tester {
    int x;
    double y;
    char c;

    friend std::ostream &operator << (std::ostream& out, const Tester& tester) {
        out << tester.x << ' ' << tester.y << ' ' << tester.c << '\n';
        return out;
    }
};


int main() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> rand_int(-1, 1);
    std::uniform_real_distribution<double> rand_double(0, 1);

    Stack<Tester> stack(1);
    for (int i = 0; i < 100; ++i) {
        stack.push({rand_int(generator), rand_double(generator), 'a'});
    }

    std::cout << "Now stack size is: " << stack.get_size() << '\n';

    for (int i = 0; i < 100; ++i) {
        Tester tester{0, 0, 0};
        stack.top(tester);
        std::cout << tester.x << ' ' << tester.y << ' ' << tester.c << '\n';
        stack.pop();
    }

    std::cout << "Now stack size is: " << stack.get_size() << '\n';

    return 0;
}
