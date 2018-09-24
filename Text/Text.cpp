#include "Text.h"


Text::~Text() {
    fclose(file_);
    delete[] text_;

    delete[] copy_;
    delete[] original_;

    file_ = nullptr;
}

void Text::show(std::ostream &stream) const {
    for (int i = 0; i < lines_ - 1; ++i) {
        stream << original_[i] << '\n';
    }
}

ui Text::get_lines() const {
    return lines_;
}

char **Text::get_copy() {
    return copy_;
}

void Text::set_pointers_() {
    int counter = 0;
    copy_[counter] = text_;
    original_[counter] = text_;
    counter++;

    for (int i = 0; i < size_of_file_; ++i) {
        if (text_[i] == '\n') {
            if (i != size_of_file_ - 1) {
                copy_[counter] = &text_[i + 1];
                original_[counter] = &text_[i + 1];
                counter++;
            }
            text_[i] = '\0';
        }
    }
}
