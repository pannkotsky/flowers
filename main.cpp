#include <iostream>
#include <string>
#include <fstream>
#include <ncurses.h>
#include <menu.h>
#include <dirent.h>
#include "helpers.h"
#include "models/date.h"
#include "models/flower.h"
#include "models/sale_entity.h"
#include "models/transaction.h"
#include "models/container.h"
#include "models/assortment.h"
#include "models/balance.h"

const char *choices[] = {
        "Add flower to assortment",
        "View assortment",
        "Save assortment to binary file",
        "Load assortment from binary file",
        "Save assortment to text file",
        "Read assortment from text file (read-only)",
        "Add transaction",
        "View all transactions",
        "Save transactions to file",
        "Load transactions from file",
        "View transactions for a week",
        "View transactions by flower name or color",
        "Exit",
        (char *)nullptr,
};


void add_sale_entity(Assortment *assortment);
template<class Item>void view(Item *item);
template<class Item>void save(Item *item);
template<class Item>void read(Item *item);
void save_assortment_text(Assortment *assortment);
void read_assortment_text();
void add_transaction(Assortment *assortment, Balance *balance);
void filter_transactions_by_date(Balance *balance);
void filter_transactions_by_text(Balance *balance);
char *get_filename_with_menu(str suffix, bool with_new_file = false);


int main() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    ITEM **my_items;
    MENU *my_menu;
    int n_choices, i;
    bool exit = false;
    Assortment assortment;
    Balance balance;

    n_choices = ARRAY_SIZE(choices);
    my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));

    for(i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i], choices[i]);

    my_menu = new_menu((ITEM **)my_items);

    /* Set menu option not to show the description */
    menu_opts_off(my_menu, O_SHOWDESC);
    post_menu(my_menu);
    refresh();

    while(!exit) {
        switch(getch()) {
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case 10:
                int selected = item_index(current_item(my_menu));
                pos_menu_cursor(my_menu);

                switch(selected) {
                    case 0:
                        add_sale_entity(&assortment);
                        break;
                    case 1:
                        view<Assortment>(&assortment);
                        break;
                    case 2:
                        save<Assortment>(&assortment);
                        break;
                    case 3:
                        read<Assortment>(&assortment);
                        break;
                    case 4:
                        save_assortment_text(&assortment);
                        break;
                    case 5:
                        read_assortment_text();
                        break;
                    case 6:
                        add_transaction(&assortment, &balance);
                        break;
                    case 7:
                        view<Balance>(&balance);
                        break;
                    case 8:
                        save<Balance>(&balance);
                        break;
                    case 9:
                        read<Balance>(&balance);
                        break;
                    case 10:
                        filter_transactions_by_date(&balance);
                        break;
                    case 11:
                        filter_transactions_by_text(&balance);
                        break;
                    case 12:
                        exit = true;
                        break;
                }

                break;
        }
    }

    for (i = 0; i < n_choices; ++i) {
        free_item(my_items[i]);
    }

    unpost_menu(my_menu);
    free_menu(my_menu);

    endwin();
    return EXIT_SUCCESS;
}


void add_sale_entity(Assortment *assortment) {
    char nm[MAX_INPUT], col_nm[MAX_INPUT], price_str[MAX_INPUT];
    SaleEntity *se;
    move(15, 0);
    clrtobot();
    do {
        move(17, 0);
        clrtobot();
        printw("Enter the flower name (%d to %d characters long): ", MIN_FLOWER_NAME, MAX_FLOWER_NAME);
        echo();
        flushinp();
        getstr(nm);
        printw("Enter the color (options: ");
        printArr(ACCEPTED_COLORS, ARRAY_SIZE(ACCEPTED_COLORS));
        printw("): ");
        flushinp();
        getstr(col_nm);
        printw("Enter the price (in kopecks, max %d): ", MAX_PRICE);
        flushinp();
        getstr(price_str);
        noecho();
        int price = atoi(price_str);

        move(15, 0);
        attron(COLOR_PAIR(2));
        se = SaleEntity::create_safe(price, nm, col_nm);
        attroff(COLOR_PAIR(2));
    } while (se == nullptr);

    move(15, 0);
    attron(COLOR_PAIR(1));
    printw("Flower created successfully: ");
    se->display();
    attroff(COLOR_PAIR(1));
    clrtobot();
    assortment->append(se);
}


template <class Item>
void view(Item *item) {
    move(15, 0);
    clrtobot();
    move(17, 0);
    item->display();
}


template<class Item>
void save(Item *item) {
    char *filename_full = get_filename_with_menu(".dat", true);
    if (filename_full == nullptr) return;

    try {
        item->write_to_file(filename_full);
        move(15, 0);
        clrtobot();
        attron(COLOR_PAIR(1));
        printw("Saved successfully!");
        attroff(COLOR_PAIR(1));
    } catch (const std::invalid_argument &e) {
        move(15, 0);
        clrtobot();
        attron(COLOR_PAIR(2));
        printw(e.what());
        attroff(COLOR_PAIR(2));
    }
    delete[] filename_full;
}


template<class Item>
void read(Item *item) {
    char *filename_full = get_filename_with_menu(".dat");
    if (filename_full == nullptr) return;

    try {
        item->read_from_file(filename_full);
        move(15, 0);
        clrtobot();
        attron(COLOR_PAIR(1));
        printw("Read successfully!");
        attroff(COLOR_PAIR(1));
    } catch (const std::invalid_argument &e) {
        move(15, 0);
        clrtobot();
        attron(COLOR_PAIR(2));
        printw(e.what());
        attroff(COLOR_PAIR(2));
    }
    delete[] filename_full;
}


void save_assortment_text(Assortment *assortment) {
    char *filename_full = get_filename_with_menu(".txt", true);
    if (filename_full == nullptr) return;

    try {
        assortment->write_to_file_text(filename_full);
        move(15, 0);
        clrtobot();
        attron(COLOR_PAIR(1));
        printw("Assortment saved successfully!");
        attroff(COLOR_PAIR(1));
    } catch (const std::invalid_argument &e) {
        move(15, 0);
        clrtobot();
        attron(COLOR_PAIR(2));
        printw(e.what());
        attroff(COLOR_PAIR(2));
    }
    delete [] filename_full;
}


void read_assortment_text() {
    char *filename_full = get_filename_with_menu(".txt");
    if (filename_full == nullptr) return;

    FILE *in_file = fopen(filename_full, "r");
    if (in_file == nullptr) {
        attron(COLOR_PAIR(2));
        printw("Could not open file");
        attroff(COLOR_PAIR(2));
    } else {
        move(15, 0);
        clrtobot();
        char buf[1024];
        while (fread(buf, 1, 1024, in_file) > 0) {
            printw(buf);
        }
        fclose(in_file);
    }
    delete[] filename_full;
}


void add_transaction(Assortment *assortment, Balance *balance) {
    char year_str[MAX_INPUT], week_str[MAX_INPUT], amount_str[MAX_INPUT];

    Transaction *transaction = nullptr;
    move(15, 0);
    clrtobot();
    while (transaction == nullptr) {
        move(17, 0);
        clrtobot();

        int assortment_length = assortment->get_length();
        if (assortment_length == 0) {
            move(15, 0);
            clrtobot();
            printw("Assortment has no items, please add some");
            break;
        }

        ITEM **se_items;
        MENU *se_menu;
        WINDOW *se_menu_win;

        se_items = (ITEM **)calloc(assortment_length + 1, sizeof(ITEM *));

        auto names = new char*[assortment_length];

        for (int i = 0; i < assortment_length; ++i) {
            SaleEntity *se = (*assortment)[i];
            char *name = se->repr();
            names[i] = name;
            se_items[i] = new_item(name, name);
        }
        se_items[assortment_length] = new_item((char *)nullptr, (char *)nullptr);

        se_menu = new_menu((ITEM **)se_items);
        se_menu_win = newwin(assortment_length, COLS, 19, 0);
        keypad(se_menu_win, TRUE);
        set_menu_win(se_menu, se_menu_win);

        /* Set menu option not to show the description */
        menu_opts_off(se_menu, O_SHOWDESC);

        printw("Choose flower");

        post_menu(se_menu);
        refresh();

        int se_index = -1;

        while(se_index == -1) {
            switch(wgetch(se_menu_win)) {
                case KEY_DOWN:
                    menu_driver(se_menu, REQ_DOWN_ITEM);
                    break;
                case KEY_UP:
                    menu_driver(se_menu, REQ_UP_ITEM);
                    break;
                case 10:
                    se_index = item_index(current_item(se_menu));
                    break;
            }
        }

        move(17, 0);
        clrtobot();
        for (int i = 0; i < assortment_length; ++i) {
            delete[] names[i];
            free_item(se_items[i]);
        }
        delete[] names;

        unpost_menu(se_menu);
        free_menu(se_menu);

        echo();
        printw("Enter the year (%d - %d): ", MIN_YEAR, MAX_YEAR);
        flushinp();
        getstr(year_str);

        printw("Enter the week of the year (1 - 53): ");
        flushinp();
        getstr(week_str);

        printw("Enter the amount (max %d): ", MAX_AMOUNT);
        flushinp();
        getstr(amount_str);

        noecho();

        SaleEntity *se = (*assortment)[se_index];

        int year = atoi(year_str);
        int week = atoi(week_str);
        int amount = atoi(amount_str);

        move(15, 0);
        attron(COLOR_PAIR(2));
        transaction = Transaction::create_safe(se, amount, year, week);
        attroff(COLOR_PAIR(2));
    }

    if (transaction != nullptr) {
        move(15, 0);
        attron(COLOR_PAIR(1));
        printw("Transaction created successfully: ");
        transaction->display();
        attroff(COLOR_PAIR(1));
        clrtobot();
        balance->append(transaction);
    }
}


void filter_transactions_by_date(Balance *balance) {
    char year_str[MAX_INPUT], week_str[MAX_INPUT];

    move(15, 0);
    clrtobot();
    echo();
    printw("Enter the year (%d - %d): ", MIN_YEAR, MAX_YEAR);
    flushinp();
    getstr(year_str);

    printw("Enter the week of the year (1 - 53): ");
    flushinp();
    getstr(week_str);
    noecho();

    int year = atoi(year_str);
    int week = atoi(week_str);

    balance->display(year, week);
}


void filter_transactions_by_text(Balance *balance) {
    char text[MAX_INPUT];

    move(15, 0);
    clrtobot();
    echo();
    printw("Enter flower name or color: ");
    flushinp();
    getstr(text);
    noecho();

    balance->display(text);
}


char *get_filename_with_menu(str suffix, bool with_new_file) {
    move(15, 0);
    clrtobot();
    move(17, 0);

    DIR *dir;
    struct dirent *ent;
    str names[100];
    int names_count = 0;
    if ((dir = opendir("files")) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            str name = ent->d_name;
            if (str_ends_with(name, suffix)) {
                names[names_count++] = name;
                if (names_count >= 100) {
                    attron(COLOR_PAIR(2));
                    printw("Too many files");
                    attroff(COLOR_PAIR(2));
                    break;
                }
            }
        }
        closedir(dir);
    } else {
        attron(COLOR_PAIR(2));
        printw("Could not open files directory");
        attroff(COLOR_PAIR(2));
        return nullptr;
    }

    if (with_new_file) {
        names[names_count++] = "New file";
    }

    if (names_count == 0) {
        attron(COLOR_PAIR(2));
        printw("No data files found");
        attroff(COLOR_PAIR(2));
        return nullptr;
    }

    ITEM **file_items;
    MENU *file_menu;
    WINDOW *file_menu_win;

    file_items = (ITEM **)calloc(names_count + 1, sizeof(ITEM *));
    auto nms = new char*[names_count];

    for (int i = 0; i < names_count; ++i) {
        str name = names[i];
        auto nm = new char[strlen(name) + 1];
        strcpy(nm, name);
        nms[i] = nm;
        file_items[i] = new_item(nm, nm);
    }

    file_items[names_count] = new_item((char *)nullptr, (char *)nullptr);

    file_menu = new_menu((ITEM **)file_items);
    file_menu_win = newwin(names_count, COLS, 19, 0);
    keypad(file_menu_win, TRUE);
    set_menu_win(file_menu, file_menu_win);

    /* Set menu option not to show the description */
    menu_opts_off(file_menu, O_SHOWDESC);

    printw("Choose filename");
    refresh();

    post_menu(file_menu);
    wrefresh(file_menu_win);

    str filename;
    bool selected = false;

    while(!selected) {
        switch(wgetch(file_menu_win)) {
            case KEY_DOWN:
                menu_driver(file_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(file_menu, REQ_UP_ITEM);
                break;
            case 10:
                filename = item_name(current_item(file_menu));
                selected = true;
                break;
        }
    }

    move(17, 0);
    clrtobot();

    for (int i = 0; i <= names_count; ++i) {
        free_item(file_items[i]);
    }

    unpost_menu(file_menu);
    free_menu(file_menu);

    if (with_new_file && strcmp(filename, "New file") == 0) {
        char new_filename[MAX_INPUT];
        move(15, 0);
        clrtobot();
        move(17, 0);
        printw("Enter filename for new file: ");
        flushinp();
        echo();
        getstr(new_filename);
        noecho();
        strcat(new_filename, suffix);
        filename = new_filename;
    }

    auto filename_full = new char[strlen("files/") + strlen(filename) + 1];
    sprintf(filename_full, "files/%s", filename);

    for (int i = 0; i < names_count; ++i) {
        delete[] nms[i];
    }
    delete[] nms;

    return filename_full;
}
