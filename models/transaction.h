//
// Created by Valerii Kovalchuk on 4/9/18.
//

#ifndef FLOWERS_TRANSACTION_H
#define FLOWERS_TRANSACTION_H

#define MAX_AMOUNT 9999


class Transaction {
    SaleEntity sale_entity;
    uint amount;
    Date date;
    //validators
    void check_amount(uint am);
    void check_se(SaleEntity *se);
public:
    Transaction() {}
    Transaction(SaleEntity *se, uint am, uint y, uint w);
    static Transaction *create_safe(SaleEntity *se, uint am, uint y, uint w);
    SaleEntity get_se() { return sale_entity; }
    Date get_date() { return date; }
    uint get_amount() { return amount; }
    uint get_price() { return sale_entity.get_price() * amount; }
    int operator+(Transaction *tr) {return this->get_price() + tr->get_price(); }
    int operator+(int x) { return this->get_price() + x; }
    friend int operator+(int x, Transaction &tr) { return x + tr.get_price(); }
    friend int operator+=(int &x, Transaction &tr) { return x += tr.get_price(); }
    void display();
};


void Transaction::check_se(SaleEntity *se) {
    if (se == nullptr) {
        throw std::invalid_argument("Sale entity is NULL");
    }
}


void Transaction::check_amount(uint am) {
    if (am == 0) {
        throw std::invalid_argument("amount is 0");
    }
    if (am > MAX_AMOUNT) {
        throw std::invalid_argument("amount is too big");
    }
}


Transaction::Transaction(SaleEntity *se, uint am, uint y, uint w) {
    check_se(se);
    check_amount(am);
    date = Date(y, w);
    sale_entity = SaleEntity(*se);
    amount = am;
}


Transaction *Transaction::create_safe(SaleEntity *se, uint am, uint y, uint w) {
    try {
        return new Transaction(se, am, y, w);
    } catch (std::invalid_argument &e) {
        printw("Couldn't create Transaction: %s", e.what());
        return nullptr;
    }
}


void Transaction::display() {
    date.display();
    printw(": %d %s %s%s", amount, sale_entity.get_color(), sale_entity.get_name(), amount > 1 ? "s" : "");
}


#endif //FLOWERS_TRANSACTION_H
