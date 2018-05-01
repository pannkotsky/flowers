//
// Created by Valerii Kovalchuk on 4/29/18.
//

#ifndef FLOWERS_BALANCE_H
#define FLOWERS_BALANCE_H


class Balance:public Container<Transaction> {
public:
    void display();
};


void Balance::display() {
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

     int total_price = 0;
    int len = get_length();
    for (int i = 0; i < len; i++) {
        tr = (*this)[i];
        total_price += *tr;
        Date d = tr->get_date();
        SaleEntity se = tr->get_se();
        char *flower_repr = se.repr();
        printw("\u2502 %d-%02d \u2502 %-30s \u2502 %6d \u2502 %13.2f \u2502\n", d.get_year(), d.get_week(), flower_repr, tr->get_amount(), tr->get_price() / 100.0);
        delete[] flower_repr;
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
    printw("Total price: %.2f UAH\n", total_price / 100.0);
}


#endif //FLOWERS_BALANCE_H
