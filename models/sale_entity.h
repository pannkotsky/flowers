//
// Created by Valerii Kovalchuk on 4/7/18.
//

#ifndef FLOWERS_SALE_ENTITY_H
#define FLOWERS_SALE_ENTITY_H

#define MAX_PRICE 99999


class SaleEntity:public Flower {
    uint price;
    // validators
    void check_price(uint pr);
public:
    SaleEntity() {}
    SaleEntity(uint pr, str nm, str col_nm);
    SaleEntity(const SaleEntity &obj) {*this = obj;}
    static SaleEntity *create_safe(uint pr, str nm, str col_nm);
    uint get_price() { return price; }
    void display();
};


void SaleEntity::check_price(uint pr) {
    if (pr > MAX_PRICE) {
        throw std::invalid_argument("price is too high");
    }
}


SaleEntity::SaleEntity(uint pr, str nm, str col_nm): Flower(nm, col_nm) {
    check_price(pr);
    price = pr;
}


SaleEntity *SaleEntity::create_safe(uint pr, str nm, str col_nm) {
    try {
        return new SaleEntity(pr, nm, col_nm);
    } catch (std::invalid_argument &e) {
        printw("Couldn't create SaleEntity: %s", e.what());
        return nullptr;
    }
}


void SaleEntity::display() {
    Flower::display();
    printw(", %.2f UAH", price / 100.0);
}


#endif //FLOWERS_SALE_ENTITY_H
