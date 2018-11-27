#include <iostream>
#include "../text/Text.h"
#include "Tree.h"


int main() {

    Text text("/Users/egor/CLionProjects/Differentiator Tree/expression.txt");
    Tree tree(text.get_copy()[0]);
    std::vector<std::string> expression;
    tree.simplify();
    tree.show_tree(expression);
    for (const auto &i : expression) {
        std::cout << i;
    }

    expression.clear();
    std::cout << '\n';

    Tree diff_tree(tree.differentiate());
    diff_tree.simplify();
    diff_tree.show_tree(expression);

    for (const auto &i : expression) {
        std::cout << i;
    }

    return 0;
}