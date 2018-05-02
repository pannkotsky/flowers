//
// Created by Valerii Kovalchuk on 4/15/18.
//

#ifndef FLOWERS_HELPERS_H
#define FLOWERS_HELPERS_H

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

typedef unsigned int uint;
typedef const char* str;


void printN(str s, int n) {
    for (int i = 0; i < n; i++) {
        printw(s);
    }
}

void fprintN(FILE *file, str s, int n) {
    for (int i = 0; i < n; i++) {
        fprintf(file, "%s", s);
    }
}


void printArr(str arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        printw(arr[i]);
        printw(", ");
    }
    printw(arr[size - 1]);
}


bool contains(str haystack[], str needle, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(haystack[i], needle) == 0) return true;
    }
    return false;
}


bool str_ends_with(str full_string, str ending) {
    if (full_string == nullptr || ending == nullptr) return false;

    size_t str_len = strlen(full_string);
    size_t ending_len = strlen(ending);

    if (ending_len > str_len) return false;

    return 0 == strncmp(full_string + str_len - ending_len, ending, ending_len);
}


template<class Anytype>
int sum(Anytype *items[], uint size) {
    int res = 0;
    for (int i = 0; i < size; i++) {
        res += *(items[i]);
    }
    return res;
}


template<class Anytype>
int average(Anytype *items[], uint size) {
    if (size == 0) return 0;
    return sum<Anytype>(items, size) / size;
}


#endif //FLOWERS_HELPERS_H
