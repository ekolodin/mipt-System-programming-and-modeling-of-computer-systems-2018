#include <iostream>
#include "../text/Text.h"
#include "Tree.h"


int main() {

    Text text("/Users/egor/CLionProjects/Differentiator Tree/expression.txt");
    for (int line = 0; line < 5; ++line) {
        Tree tree(text.get_copy()[line]);
        std::vector<std::string> expression;
        tree.simplify();
        tree.show_tree(expression);
        for (const auto &i : expression) {
            std::cout << i;
        }

        std::cout << '\n';

        expression.clear();
        Tree diff(tree.differentiate());
        diff.simplify();
        diff.show_tree(expression);

        for (const auto &i : expression) {
            std::cout << i;
        }

        std::cout << "\n\n";

    }

    return 0;
}