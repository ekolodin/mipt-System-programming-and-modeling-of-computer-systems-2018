#include "Tree.h"


#define zero_plus_node(node)      (L(node) && R(node) && node->is_add() && L(node)->is_zero())
#define node_mul_zero(node)       (L(node) && R(node) && node->is_mul() && R(node)->is_zero())
#define one_mul_node(node)        (L(node) && R(node) && node->is_mul() && L(node)->is_one())
#define node_mul_div_one(node)    (L(node) && R(node) && (node->is_mul() || node->is_div()) && R(node)->is_one())
#define node_plus_minus_one(node) (L(node) && R(node) && (node->is_add() || node->is_sub()) && R(node)->is_zero())
#define zero_mul_div_node(node)   (L(node) && R(node) && (node->is_mul() || node->is_div()) && L(node)->is_zero())


void Tree::show_tree(std::vector<std::string> &expression) {
    dfs(root_, dfs_type::show_tree_, [&expression](Node *node) {
        expression.push_back(node->to_str());
    });
}

void Tree::simplify() {
    dfs(root_, dfs_type::simplify_, [this](Node *node) {
        if (L(node) && R(node) && L(node)->is_val() && R(node)->is_val()) {
            double result = 0;
            if (node->is_add()) { result = L(node)->value_ + R(node)->value_; }

            if (node->is_sub()) { result = L(node)->value_ - R(node)->value_; }

            if (node->is_mul()) { result = L(node)->value_ * R(node)->value_; }

            if (node->is_div()) { result = L(node)->value_ / R(node)->value_; }

            node->set_val(result);

            destroy(L(node));
            destroy(R(node));
            node->left_ = nullptr;
            node->right_ = nullptr;
        }
        // 0 + node --> node  or  node * 0 --> 0  or  1 * node --> node
        if (zero_plus_node(node) || node_mul_zero(node) || one_mul_node(node)) {
            destroy(L(node));
            auto right_subtree = R(node);
            *node = *R(node);
            delete right_subtree;
        }
        // node * / 1 --> node  or  node +- 0 --> node  or  0 * / node --> 0
        if (node_mul_div_one(node) || node_plus_minus_one(node) || zero_mul_div_node(node)) {
            destroy(R(node));
            auto left_subtree = L(node);
            *node = *L(node);
            delete left_subtree;
        }
    });
}

void Tree::destroy(Node *node) {
    Tree destroy_(node);
}

Node *Tree::differentiate_(Node *node) {
    if (node->is_mul()) {
        auto dif_node = new Node('+');

        L(dif_node) = new Node('*');
        dif_node->left_->left_ = differentiate_(L(node));
        dif_node->left_->right_ = new Node(R(node));

        R(dif_node) = new Node('*');
        dif_node->right_->right_ = differentiate_(R(node));
        dif_node->right_->left_ = new Node(L(node));

        return dif_node;
    }

    if (node->is_div()) {
        //  (left / right)' = (left' * right - left * right') / (right * right)
        auto dif_node = new Node('/');

        // numerator
        L(dif_node) = new Node('-');

        dif_node->left_->left_ = new Node('*');
        dif_node->left_->left_->left_ = differentiate_(L(node));
        dif_node->left_->left_->right_ = new Node(R(node));

        dif_node->left_->right_ = new Node('*');
        dif_node->left_->right_->right_ = differentiate_(R(node));
        dif_node->left_->right_->left_ = new Node(L(node));

        // denominator
        R(dif_node) = new Node('*');
        dif_node->right_->left_ = new Node(R(node));
        dif_node->right_->right_ = new Node(R(node));

        return dif_node;
    }

    if (node->is_add() || node->is_sub()) {
        auto dif_node = new Node(node->operation_);
        L(dif_node) = differentiate_(L(node));
        R(dif_node) = differentiate_(R(node));

        return dif_node;
    }

    return new Node((node->is_val() ? 0.0 : 1.0));
}

int Tree::find_middle(int left, int right) const {
    assert(left <= right);

    int counter = 0;
    for (int i = left; i <= right; ++i) {
        if (expression_[i] == '(') {
            ++counter;
        } else if (expression_[i] == ')') {
            --counter;
        }

        if (!counter) {
            return i + 1;
        }
    }

    return -1;
}

Node *Tree::create_tree(int left, int right) {
    assert(left <= right);

    // variable
    if (left == right && expression_[left] == 'x') {
        return new Node(expression_[left]);
    }

    // constant
    bool is_constant = true;
    std::string probable_constant(expression_.begin() + left, expression_.begin() + right + 1);
    for (auto digit : probable_constant) {
        if (!isdigit(digit)) {
            is_constant = false;
            break;
        }
    }

    if (is_constant) {
        return new Node(stod(probable_constant));
    }

    // expression
    int middle = find_middle(left, right);
    assert(middle != -1);

    Node *node = new Node(expression_[middle]);
    L(node) = create_tree(left + 1, middle - 2);
    R(node) = create_tree(middle + 2, right - 1);

    return node;
}
