//
// Created by Valerii Kovalchuk on 4/3/18.
//

#ifndef FLOWERS_DATE_H
#define FLOWERS_DATE_H

#define MIN_YEAR 2000
#define MAX_YEAR 3000


class Date {
    uint year;
    uint week;
    // validators
    void check_year(uint y);
    void check_week(uint w);
public:
    Date() {}
    Date(uint y, uint w);
    static Date *create_safe(uint y, uint w);
    uint get_year() {return year;}
    uint get_week() {return week;}
    void display();
};


void Date::check_year(uint y) {
    if (y < MIN_YEAR || y > MAX_YEAR) {
        throw std::invalid_argument("invalid year");
    }
}


void Date::check_week(uint w) {
    if (w < 1 || w > 53) {
        throw std::invalid_argument("invalid week");
    }
}


Date::Date(uint y, uint w) {
    check_year(y);
    check_week(w);
    year = y;
    week = w;
}


Date *Date::create_safe(uint y, uint w) {
    try {
        return new Date(y, w);
    } catch (const std::invalid_argument &e) {
        printw("Couldn't create Date: %s", e.what());
        return nullptr;
    }
}


void Date::display() {
    printw("Year %d, Week %02d", year, week);
}


#endif //FLOWERS_DATE_H
