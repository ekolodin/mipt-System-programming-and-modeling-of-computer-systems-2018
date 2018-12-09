#include <iostream>
#include <fstream>
#include "../text/Text.h"
#include "Tree.h"


int main() {

    Text text("/Users/egor/CLionProjects/Differentiator Tree/expression.txt");
    for (int line = 0; line < 5; ++line) {
        Tree tree(text.get_copy()[line]);
        std::vector<std::string> expression;

        std::cout << "Simplifying the expression:\n";
        tree.simplify();
        std::cout << "Simplifying ends\nNow it is:\n";
        tree.show_tree(expression);
        for (const auto &i : expression) {
            std::cout << i;
        }

        std::cout << '\n';

        expression.clear();


        Tree diff(tree.differentiate());

        std::cout << "Simplifying the derivative:\n";
        tree.simplify();
        std::cout << "Simplifying ends\nNow it is:\n";

        diff.show_tree(expression);

        for (const auto &i : expression) {
            std::cout << i;
        }

        std::cout << "\n\n";
    }

    return 0;
}
