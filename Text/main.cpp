#include <iostream>
#include "Text.h"

typedef unsigned int ui;


int reverse_compare(const char *first, const char *second) {
    size_t len_first = strlen(first) - 1;
    size_t len_second = strlen(second) - 1;

    while (len_first > 0 && !isalnum(*(first + len_first))) {
        --len_first;
    }
    while (len_second > 0 && !isalnum(*(second + len_second))) {
        --len_second;
    }

    while (len_first > 0 && len_second > 0 &&
           (*(first + len_first) == *(second + len_second))) {

        --len_first;
        --len_second;
        while (len_first > 0 && !isalnum(*(first + len_first))) {
            --len_first;
        }
        while (len_second > 0 && !isalnum(*(second + len_second))) {
            --len_second;
        }
    }
    return *(first + len_first) - *(second + len_second);
}


inline void write(const char *path_to_file, char **pointers, ui size) {
    FILE *file = fopen(path_to_file, "w");
    if (!file) {
        perror("Cannot be opened for write\n");
        return;
    }

    for (int i = 0; i < size; ++i) {
        fwrite(pointers[i], sizeof(char), sizeof(char) * strlen(pointers[i]), file);
        fputc('\n', file);
    }
    fclose(file);
}


int main() {
    // How to use class Text

    Text text("/Users/egor/CLionProjects/text/input.txt");

    std::cout << "The original: \n\n";
    text.show();
    std::cout << "\n\n";

    auto copy = text.get_copy();


    std::sort(copy, copy + text.get_lines(),
              [](const char *a, const char *b) { return strcmp(a, b) < 0; });
    write("/Users/egor/CLionProjects/text/sorted.txt", copy, text.get_lines());


    std::sort(copy, copy + text.get_lines(),
              [](const char *a, const char *b) { return reverse_compare(a, b) < 0; });
    write("/Users/egor/CLionProjects/text/rev_sorted.txt", copy, text.get_lines());


    return 0;
}
