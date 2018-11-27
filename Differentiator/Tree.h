#include <vector>
#include <string>
#include "Node.h"


class Tree {
public:

    explicit Tree(const std::string &expression) {
        expression_ = expression;
        root_ = create_tree(0, expression_.size() - 1);
    }

    Tree(const Tree &tree) {
        expression_ = tree.get_expression();
        root_ = create_tree(0, expression_.size() - 1);
    }

    ~Tree() {
        dfs(root_, dfs_type::destructor_, [](Node *node) { delete node; });
    }

    Tree differentiate() {
        return Tree(differentiate_(root_));
    }

    void show_tree(std::vector<std::string> &expression);

    void simplify();

public:

    enum dfs_type {
        destructor_, show_tree_, simplify_
    };

private:

    explicit Tree(Node *root) : root_(root) {
        std::vector<std::string> expression;
        show_tree(expression);

        for (const auto &i : expression) {
            expression_ += i;
        }
    }

    void destroy(Node *node);

    std::string get_expression() const {
        return expression_;
    }

    Node *differentiate_(Node *node);

    int find_middle(int left, int right) const;

    Node *create_tree(int left, int right);

    template<typename Lambda>
    void dfs(Node *node, dfs_type flag, Lambda &&lambda) {
        if (node) {
            Node open_bracket('(');
            Node close_bracket(')');

            if (flag == dfs_type::show_tree_) { lambda(&open_bracket); }

            if (flag == dfs_type::simplify_) { lambda(node); }

            dfs(L(node), flag, lambda);

            if (flag != dfs_type::destructor_) { lambda(node); }

            dfs(R(node), flag, lambda);

            if (flag == dfs_type::simplify_) { lambda(node); }

            if (flag == dfs_type::show_tree_) { lambda(&close_bracket); }

            if (flag == dfs_type::destructor_) { lambda(node); }
        }
    }

private:

    std::string expression_{};
    Node *root_ = nullptr;
};
