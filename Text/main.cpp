#include <iostream>

class Text {
public:
    explicit Text(const char *path_to_file) {
        file_ = fopen(path_to_file, "r");
        if (!file_) {
            printf("File not found\n");
        } else {
            set_size_();

            text_ = new char[size_of_file_];
            sorted_text_ = new char[size_of_file_];
            rev_sorted_text_ = new char[size_of_file_];

            fread(text_, sizeof(char), size_of_file_, file_);
            strcpy(sorted_text_, text_);
            strcpy(rev_sorted_text_, text_);

            for (int i = 0; i < size_of_file_; ++i) {
                if (text_[i] == '\n') {
                    ++count_lines_;
                }
            }

            pointers_to_strings_ = new char *[count_lines_];
        }
    }

    ~Text() {
        fclose(file_);
        delete[] text_;
        delete[] sorted_text_;
        delete[] rev_sorted_text_;
        delete[] pointers_to_strings_;
    }

    void sort_straight(const char *path_to_file) {
        FILE *file_sort_straight = fopen(path_to_file, "w");
        if (!file_sort_straight) {
            printf("File not found\n");
        } else {
            set_pointers_(sorted_text_);

            std::sort(pointers_to_strings_, pointers_to_strings_ + count_lines_,
                      [](const char *a, const char *b) { return strcmp(a, b) < 0; });
            for (int i = 0; i < count_lines_; ++i) {
                fwrite(pointers_to_strings_[i], sizeof(char), sizeof(char) * strlen(pointers_to_strings_[i]),
                       file_sort_straight);
                fputc('\n', file_sort_straight);
            }
            fclose(file_sort_straight);
        }
    }

    void sort_reverse(const char *path_to_file) {
        FILE *file_rev_sort = fopen(path_to_file, "w");
        if (!file_rev_sort) {
            printf("File not found\n");
        } else {
            set_pointers_(rev_sorted_text_);

            std::sort(pointers_to_strings_, pointers_to_strings_ + count_lines_, rev_cmp);
            for (int i = 0; i < count_lines_; ++i) {
                fwrite(pointers_to_strings_[i], sizeof(char), sizeof(char) * strlen(pointers_to_strings_[i]),
                       file_rev_sort);
                fputc('\n', file_rev_sort);
            }
            fclose(file_rev_sort);
        }
    }

private:
    long set_size_() {
        fseek(file_, 0, SEEK_END);
        size_of_file_ = ftell(file_);
        fseek(file_, 0, SEEK_SET);
    }

    void set_pointers_(char *text) {
        int counter = 0;
        pointers_to_strings_[counter++] = text;

        for (int i = 0; i < size_of_file_; ++i) {
            if (text[i] == '\n') {
                if (i != size_of_file_ - 1) {
                    pointers_to_strings_[counter++] = &text[i + 1];
                }
                text[i] = '\0';
            }
        }
    }

    static bool rev_cmp(char *a, char *b) {
        std::reverse(a, a + strlen(a));
        std::reverse(b, b + strlen(b));

        bool answer = strcmp_punct_(a, b) > 0;

        std::reverse(a, a + strlen(a));
        std::reverse(b, b + strlen(b));

        return answer;
    }

    static int strcmp_punct_(const char *a, const char *b) {
        while (*a && ispunct(*a)) {
            a++;
        }
        while (*b && ispunct(*b)) {
            b++;
        }

        while (*a && *b && (*a == *b)) {
            ++a;
            ++b;
            while (*a && ispunct(*a)) {
                a++;
            }
            while (*b && ispunct(*b)) {
                b++;
            }
        }
        return *a - *b;
    }

private:
    FILE *file_;
    long size_of_file_{};
    unsigned int count_lines_ = 0;
    char *text_;
    char *sorted_text_;
    char *rev_sorted_text_;
    char **pointers_to_strings_;
};

int main() {
    // How to use class Text
    
    Text text("/Users/egor/CLionProjects/text/input.txt");
    text.sort_straight("/Users/egor/CLionProjects/text/sorted.txt");
    text.sort_reverse("/Users/egor/CLionProjects/text/rev_sorted.txt");

    return 0;
}