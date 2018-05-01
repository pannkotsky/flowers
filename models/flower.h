//
// Created by Valerii Kovalchuk on 4/7/18.
//

#ifndef FLOWERS_FLOWER_H
#define FLOWERS_FLOWER_H

#define MIN_FLOWER_NAME 3
#define MAX_FLOWER_NAME 20
#define MAX_COLOR_NAME 6
const char *ACCEPTED_COLORS[] = {"Red", "Blue", "Yellow", "White", "Pink", "Purple"};


class Flower {
    char name[MAX_FLOWER_NAME];
    char color[MAX_COLOR_NAME];
    // validators
    void check_name(str nm);
    void check_color(str col);
public:
    Flower() {}
    Flower(str nm, str col);
    static Flower *create_safe(str nm, str col);
    char *get_name() { return name; }
    char *get_color() { return color; }
    void display();
    char *repr();
};


void Flower::check_name(str nm) {
    if (strlen(nm) < MIN_FLOWER_NAME) {
        throw std::invalid_argument("name is too short");
    }
    if (strlen(nm) > MAX_FLOWER_NAME) {
        throw std::invalid_argument("name is too long");
    }
}


void Flower::check_color(str col) {
    char msg[strlen("invalid color: ") + strlen(col)];
    sprintf(msg, "invalid color: %s", col);
    if (!contains(ACCEPTED_COLORS, col, ARRAY_SIZE(ACCEPTED_COLORS))) {
        throw std::invalid_argument(msg);
    }
}


Flower::Flower(str nm, str col) {
    check_name(nm);
    check_color(col);
    sprintf(name, "%s", nm);
    sprintf(color, "%s", col);
}


Flower *Flower::create_safe(str nm, str col) {
    try {
        return new Flower(nm, col);
    } catch (const std::invalid_argument &e) {
        printw("Couldn't create Flower: %s", e.what());
        return nullptr;
    }
}


void Flower::display() {
    printw("%s %s", color, name);
}


char *Flower::repr() {
    auto res = new char[strlen(color) + 1 + strlen(name) + 1];
    sprintf(res, "%s %s", color, name);
    return res;
}


#endif //FLOWERS_FLOWER_H
