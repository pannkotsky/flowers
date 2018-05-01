//
// Created by Valerii Kovalchuk on 4/9/18.
//

#ifndef FLOWERS_ASSORTMENT_H
#define FLOWERS_ASSORTMENT_H


class Assortment:public Container<SaleEntity> {
public:
    void display();
    void write_to_file_text(const char *filename);
};


void Assortment::display() {
    SaleEntity *se;

    // top border
    printw("\u250c");
    printN("\u2500", 22);
    printw("\u252c");
    printN("\u2500", 12);
    printw("\u252c");
    printN("\u2500", 8);
    printw("\u2510");
    printw("\n");

    // header
    printw("\u2502 %-20s \u2502 %-10s \u2502 %-6s \u2502\n", "Name", "Color", "Price");

    // header border
    printw("\u251c");
    printN("\u2500", 22);
    printw("\u253c");
    printN("\u2500", 12);
    printw("\u253c");
    printN("\u2500", 8);
    printw("\u2524");
    printw("\n");

    int len = get_length();
    for (int i = 0; i < len; i++) {
        se = (*this)[i];
        printw("\u2502 %-20s \u2502 %-10s \u2502 %6.2f \u2502\n", se->get_name(), se->get_color(), se->get_price() / 100.0);
    }

    // bottom border
    printw("\u2514");
    printN("\u2500", 22);
    printw("\u2534");
    printN("\u2500", 12);
    printw("\u2534");
    printN("\u2500", 8);
    printw("\u2518");
    printw("\n");
}


void Assortment::write_to_file_text(const char *filename) {
    FILE *out_file = fopen(filename, "w");
    if (out_file == nullptr) {
        throw std::invalid_argument("Could not open file");
    }
    SaleEntity *se;

    // top border
    fprintf(out_file, "\u250c");
    fprintN(out_file, "\u2500", 22);
    fprintf(out_file, "\u252c");
    fprintN(out_file, "\u2500", 12);
    fprintf(out_file, "\u252c");
    fprintN(out_file, "\u2500", 8);
    fprintf(out_file, "\u2510");
    fprintf(out_file, "\n");

    // header
    fprintf(out_file, "\u2502 %-20s \u2502 %-10s \u2502 %-6s \u2502\n", "Name", "Color", "Price");

    // header border
    fprintf(out_file, "\u251c");
    fprintN(out_file, "\u2500", 22);
    fprintf(out_file, "\u253c");
    fprintN(out_file, "\u2500", 12);
    fprintf(out_file, "\u253c");
    fprintN(out_file, "\u2500", 8);
    fprintf(out_file, "\u2524");
    fprintf(out_file, "\n");

    int len = get_length();
    for (int i = 0; i < len; i++) {
        se = (*this)[i];
        fprintf(out_file, "\u2502 %-20s \u2502 %-10s \u2502 %6.2f \u2502\n", se->get_name(), se->get_color(), se->get_price() / 100.0);
    }

    // bottom border
    fprintf(out_file, "\u2514");
    fprintN(out_file, "\u2500", 22);
    fprintf(out_file, "\u2534");
    fprintN(out_file, "\u2500", 12);
    fprintf(out_file, "\u2534");
    fprintN(out_file, "\u2500", 8);
    fprintf(out_file, "\u2518");
    fprintf(out_file, "\n");

    fclose(out_file);
}


#endif //FLOWERS_ASSORTMENT_H
