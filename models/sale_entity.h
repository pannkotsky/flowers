//
// Created by Valerii Kovalchuk on 4/7/18.
//

#ifndef FLOWERS_SALE_ENTITY_H
#define FLOWERS_SALE_ENTITY_H

#define MAX_PRICE 99999


class SaleEntity:public Flower {
    uint price;
    // validators
    void check_price(int pr);
public:
    SaleEntity() {}
    SaleEntity(int pr, str nm, str col_nm);
    SaleEntity(const SaleEntity &obj) {*this = obj;}
    static SaleEntity *create_safe(uint pr, str nm, str col_nm);
    uint get_price() { return price; }
    void set_price(int pr);
    void display();
};


void SaleEntity::check_price(int pr) {
    if (pr <= 0) {
        throw std::invalid_argument("price must be positive");
    }
    if (pr > MAX_PRICE) {
        throw std::invalid_argument("price is too high");
    }
}


SaleEntity::SaleEntity(int pr, str nm, str col_nm): Flower(nm, col_nm) {
    set_price(pr);
}


SaleEntity *SaleEntity::create_safe(uint pr, str nm, str col_nm) {
    try {
        return new SaleEntity(pr, nm, col_nm);
    } catch (std::invalid_argument &e) {
        printw("Couldn't create SaleEntity: %s", e.what());
        return nullptr;
    }
}


void SaleEntity::set_price(int pr) {
    check_price(pr);
    price = pr;
}


void SaleEntity::display() {
    Flower::display();
    printw(", %.2f UAH", price / 100.0);
}


#endif //FLOWERS_SALE_ENTITY_H
