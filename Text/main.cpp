#include <iostream>

Библиотечку сделай, хотя б в инклюдах. Полезный класс будет

class Text {
public:
    explicit Text(const char *path_to_file) :
        секция инициализации?..    
    {
        file_ = fopen(path_to_file, "r");
        if (!file_) {
            printf("File not found\n");
            return;
            
#define else не нужен

        } else {
            set_size_();
            
            Вызываешь ф-ю, скрыто меняющую стейт класса. Лучше внешнюю ф-ю getSize(), возвр. размер.
            И здесь присваивать. Будет явно. Плюс получишь +1 удобную независ. ф-ю.

            text_ = new char[size_of_file_];            размер +1 очень советую
            sorted_text_ = new char[size_of_file_];     ойойой дублирование контента
            rev_sorted_text_ = new char[size_of_file_]; ойойой зачем?
                                                        достаточно массив указателей скопировать

            fread(text_, sizeof(char), size_of_file_, file_);  я б ретерн его проверил, а то вдруг 0, например
            strcpy(sorted_text_, text_);                       мастер -
            strcpy(rev_sorted_text_, text_);                          - копипастер))

            for (int i = 0; i < size_of_file_; ++i) {          на \0 менять не будешь?
                if (text_[i] == '\n') {
                    ++count_lines_;
                }
            }

            pointers_to_strings_ = new char *[count_lines_];  +1 советую, и последний пойнтер в nullptr (зачем?)
        }
    }

    ~Text() {
        fclose(file_);                                         советую в nullptr сбросить после отпускания ресурсов
        delete[] text_;
        delete[] sorted_text_;
        delete[] rev_sorted_text_;
        delete[] pointers_to_strings_;
    }

#define __away__

    void sort_straight(const char *path_to_file) {           
        FILE *file_sort_straight = fopen(path_to_file, "w");
        if (!file_sort_straight) {
            printf("File not found\n");                      cannot be opened for write. Советую perror()
            return __away__;
            
        } else {
            set_pointers_(sorted_text_);

            std::sort(pointers_to_strings_, pointers_to_strings_ + count_lines_,
                      [](const char *a, const char *b) { return strcmp(a, b) < 0; });
                      
            пустую строку поставить здесь советую тебе
            
            for (int i = 0; i < count_lines_; ++i) {
                fwrite(pointers_to_strings_[i], sizeof(char), sizeof(char) * strlen(pointers_to_strings_[i]),
                       file_sort_straight);
                fputc('\n', file_sort_straight);
            }
            fclose(file_sort_straight);
        }
    }

вообще у тебя класс Текст оказался сильно связан с текущей задачей. Это делает его менее универсальным.
Имхо сортировки и записи ответов выкинь из класса и засунь в мейн

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
        
        return??? Егор, такие ошибки неделями ищут!.. Опасайся
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
        std::reverse(a, a + strlen(a));      хехехе. хитрый. но это память жрет сильно, и время.
        std::reverse(b, b + strlen(b));

        bool answer = strcmp_punct_(a, b) > 0;

        std::reverse(a, a + strlen(a));      ну а как же, а то заметят :)
        std::reverse(b, b + strlen(b));

        return answer;
    }

    static int strcmp_punct_(const char *a, const char *b) {
        while (*a && ispunct(*a)) { лучше !isalmun. А то бывают еще и пробелы всякие в конце строки
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

    // How my friends imagine this :)
    
    Text() .input() .output (sort1) .output (sort2) .output();
    
    // How my dean imagine this :)
    
    TESTS PASSED, GRADE = A+
        
    // How it should be
        
    Text text ("input.txt");
    
    Text textSorted = text.shallowCopy();
    std::sort (textSorted.data(), textSorted.data() + textSorted.length());
    WriteText (textSorted, "output1.txt");

    auto revCmp = [] (const char*, const char*) {
        ...
    };
    
    Text textSorted = text.shallowCopy();
    std::sort (textSorted.data(), textSorted.data() + textSorted.length(), revCmp);
    WriteText (textSorted, "output2.txt");

    WriteText (text,       "output3.txt");

    return 0;
}
