//
// Created by Valerii Kovalchuk on 4/29/18.
//

#ifndef FLOWERS_BALANCE_H
#define FLOWERS_BALANCE_H


class Balance:public Container<Transaction> {
    void _display(Transaction **items);
public:
    Transaction **filter_transactions(int y, int w);
    Transaction **filter_transactions(str text);
    void display();
    void display(int y, int w);
    void display(str text);
};


Transaction **Balance::filter_transactions(int y, int w) {
    // Get transactions for particular year and week

    uint len = get_length();
    auto res = new Transaction*[len + 1];  // one more item for NULL termination
    uint filtered_count = 0;
    Transaction *tr;
    Date d;
    for (int i = 0; i < len; i++) {
        tr = (*this)[i];
        d = tr->get_date();
        if (d.get_year() == y && d.get_week() == w) {
            res[filtered_count++] = tr;
        }
    }
    res[filtered_count] = nullptr;
    return res;
}


Transaction **Balance::filter_transactions(str text) {
    // Get transactions with flowers matching provided name or color

    uint len = get_length();
    auto res = new Transaction*[len + 1];  // one more item for NULL termination
    uint filtered_count = 0;
    Transaction *tr;
    SaleEntity se;

    for (int i = 0; i < len; i++) {
        tr = (*this)[i];
        se = tr->get_se();
        if (strcmp(se.get_name(), text) == 0 || strcmp(se.get_color(), text) == 0) {
            res[filtered_count++] = tr;
        }
    }
    res[filtered_count] = nullptr;
    return res;
}


void Balance::_display(Transaction **items) {
    Transaction *tr;

    // top border
    printw("\u250c");
    printN("\u2500", 9);
    printw("\u252c");
    printN("\u2500", 32);
    printw("\u252c");
    printN("\u2500", 8);
    printw("\u252c");
    printN("\u2500", 15);
    printw("\u2510");
    printw("\n");

    // header
    printw("\u2502 %-7s \u2502 %-30s \u2502 %-6s \u2502 %-13s \u2502\n", "Date", "Flower", "Amount", "Total price");

    // header border
    printw("\u251c");
    printN("\u2500", 9);
    printw("\u253c");
    printN("\u2500", 32);
    printw("\u253c");
    printN("\u2500", 8);
    printw("\u253c");
    printN("\u2500", 15);
    printw("\u2524");
    printw("\n");

    uint len = get_length();
    uint filtered_len = 0;
    for (int i = 0; i < len; i++) {
        tr = items[i];
        if (tr == nullptr) break;
        Date d = tr->get_date();
        SaleEntity se = tr->get_se();
        char *flower_repr = se.repr();
        printw("\u2502 %d-%02d \u2502 %-30s \u2502 %6d \u2502 %13.2f \u2502\n", d.get_year(), d.get_week(), flower_repr, tr->get_amount(), tr->get_price() / 100.0);
        delete[] flower_repr;
        filtered_len++;
    }

    // bottom border
    printw("\u2514");
    printN("\u2500", 9);
    printw("\u2534");
    printN("\u2500", 32);
    printw("\u2534");
    printN("\u2500", 8);
    printw("\u2534");
    printN("\u2500", 15);
    printw("\u2518");
    printw("\n");

    double total_price = sum<Transaction>(items, filtered_len) / 100.0;
    double average_price = average<Transaction>(items, filtered_len) / 100.0;
    printw("Total price: %.2f UAH\n", total_price);
    printw("Average price: %.2f UAH\n", average_price);
}


void Balance::display() {
    _display(get_items());
}


void Balance::display(int y, int w) {
    Transaction **items = filter_transactions(y, w);
    _display(items);
    delete[] items;
}


void Balance::display(str text) {
    Transaction **items = filter_transactions(text);
    _display(items);
    delete[] items;
}


#endif //FLOWERS_BALANCE_H
