//
// Created by Valerii Kovalchuk on 4/29/18.
//

#ifndef FLOWERS_CONTAINER_H
#define FLOWERS_CONTAINER_H

#define CAPACITY 10


template<class Item>
class Container {
    Item **items;
    uint length, capacity;
public:
    Container();
    ~Container();
    bool append(Item *se);
    uint get_length() { return length; }
    Item **get_items() { return items; }
    Item *operator[](int index);
    void write_to_file(const char *filename);
    void read_from_file(const char *filename);
};


template<class Item>
Container<Item>::Container() {
    capacity = CAPACITY;
    length = 0;
    items = new Item*[capacity];
}


template<class Item>
Container<Item>::~Container() {
    if (items) {
        for (int i = 0; i < length; i++) {
            delete[] items[i];
        }
        delete[] items;
    }
}


template<class Item>
bool Container<Item>::append(Item *se) {
    if (length >= capacity) {
        Item **temp = new Item*[capacity + CAPACITY];
        for (int i = 0; i < capacity; i++) {
            temp[i] = items[i];
        }
        capacity += CAPACITY;
        delete[] items;
        items = temp;
    }
    items[length] = se;
    length++;
    return true;
}


template<class Item>
Item *Container<Item>::operator[](int index) {
    if (index < 0 || index >= length) return nullptr;
    return items[index];
}


template<class Item>
void Container<Item>::write_to_file(const char *filename) {
    std::ofstream out_file;
    out_file.open(filename, std::ios::app | std::ios::binary);
    if (out_file.fail()) {
        throw std::invalid_argument("Could not open file");
    }
    Item *item;
    for (int i = 0; i < length; i++) {
        item = items[i];
        out_file.write((char*)item, sizeof(*item));
    }
    out_file.close();
}


template<class Item>
void Container<Item>::read_from_file(const char *filename) {
    std::ifstream in_file;
    in_file.open(filename, std::ios::in | std::ios::binary);
    if (in_file.fail()) {
        throw std::invalid_argument("Could not open file");
    }
    in_file.seekg(0, std::ios::end);
    long long int endpos = in_file.tellg();
    int leftover = endpos % sizeof(Item);
    if (leftover) {
        throw std::invalid_argument("File data is wrong");
    }
    int n = endpos / sizeof(Item);
    for (int i = 0; i < n; i++) {
        auto item = new Item;
        int pos = i * sizeof(Item);
        in_file.seekg(pos, std::ios::beg);
        in_file.read((char*)item, sizeof(Item));
        append(item);
    }

    in_file.close();
}

#endif //FLOWERS_CONTAINER_H
