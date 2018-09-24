#include <iostream>


typedef unsigned int ui;


inline long get_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}


inline ui count_lines(const char *text, long size) {
    ui lines = 0;

    for (int i = 0; i < size; ++i) {
        if (text[i] == '\n') {
            ++lines;
        }
    }

    return lines;
}


class Text {
public:
    explicit Text(const char *path_to_file) : file_(nullptr),
                                              text_(nullptr),
                                              original_(nullptr),
                                              copy_(nullptr) {

        file_ = fopen(path_to_file, "r");
        if (!file_) {
            printf("File not found\n");
            return;
        }

        size_of_file_ = get_size(file_);
        text_ = new char[size_of_file_ + 1];
        if (!fread(text_, sizeof(char), size_of_file_, file_)) {
            printf("Could not read file\n");
        }

        lines_ = count_lines(text_, size_of_file_);

        original_ = new char *[lines_ + 1];
        copy_ = new char *[lines_ + 1];
        original_[lines_] = copy_[lines_] = nullptr;

        set_pointers_();

    }

    ~Text();

    void show(std::ostream &stream = std::cout) const;

    ui get_lines() const;

    char **get_copy();

private:

    void set_pointers_();

private:
    FILE *file_;
    long size_of_file_{};
    unsigned int lines_ = 0;

    char *text_;
    char **copy_;
    char **original_;
};