#define L(node) ((node)->left_)
#define R(node) ((node)->right_)


enum Type {
    value,
    operation,
    variable,
};


struct Node {
    Node *left_ = nullptr;
    Node *right_ = nullptr;

    Type type_;

    double value_ = 0;
    char operation_ = 0;
    char variable_ = 0;

    explicit Node(double value) : value_(value), type_(Type::value) {};

    explicit Node(char c) {

        if (c == 'x') {
            variable_ = c;
            type_ = Type::variable;
        } else {
            operation_ = c;
            type_ = Type::operation;
        }
    };

    explicit Node(Node *node) : type_(node->type_),
                                value_(node->value_),
                                variable_(node->variable_),
                                operation_(node->operation_) {

        if (L(node)) {
            left_ = new Node(L(node));
        }

        if (R(node)) {
            right_ = new Node(R(node));
        }
    }

    void set_val(double val) {
        type_ = Type::value;
        value_ = val;
    }

    std::string to_str() const {
        if (type_ == Type::value) {
            return std::to_string(value_);
        }

        return std::string(1, (type_ == Type::variable ? variable_ : operation_));
    }

    Node &operator=(const Node &node) {
        left_ = node.left_;
        right_ = node.right_;

        type_ = node.type_;
        value_ = node.value_;
        variable_ = node.variable_;
        operation_ = node.operation_;

        return *this;
    }

    bool is_val() { return type_ == Type::value; }

    bool is_zero() { return type_ == Type::value && value_ == 0; }

    bool is_one() { return type_ == Type::value && value_ == 1; }

    bool is_add() { return type_ == Type::operation && operation_ == '+'; }

    bool is_sub() { return type_ == Type::operation && operation_ == '-'; }

    bool is_mul() { return type_ == Type::operation && operation_ == '*'; }

    bool is_div() { return type_ == Type::operation && operation_ == '/'; }
};
