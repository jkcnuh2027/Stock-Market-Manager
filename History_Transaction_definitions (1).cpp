#include <cassert>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

#pragma warning(disable:4996)
////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned int year_date, std::string buy_sell_trans, unsigned int number_shares, double trans_amount)
{
    symbol = ticker_symbol;
    day = day_date;
    month = month_date;
    year = year_date;
    trans_type = buy_sell_trans;
    shares = number_shares;
    amount = trans_amount;
}

// Destructor
// TASK 1
//
Transaction::~Transaction() {
    free(this);
}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<(Transaction const& other) {
    if (this->year < other.get_year()) {
        if (this->month < other.get_month()) {
            if (this->day < other.get_day()) return true;
            else return false;
        }
        else return false;
    }
    return false;
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History() {
    p_head = NULL;
}

// Destructor
// TASK 3
//
History::~History() {
    free(this);
}


// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history() {
    std::ifstream readFile;
    readFile.open("transaction_history.txt");

    if (!readFile.is_open()) {
        printf("No File Exist.");
        return;
    }
    
    while (!readFile.eof()) {
        std::string line;
        std::getline(readFile, line);

        std::string delim = " ";
        std::vector<std::string> inputs{};
        
        std::stringstream ss(line);
        std::string tmp;

        while (ss >> tmp)
            inputs.push_back(tmp);

        Transaction* t = new Transaction(inputs[0], std::stoi(inputs[1]), std::stoi(inputs[2]), std::stoi(inputs[3]), inputs[4], std::stoi(inputs[5]), std::stod(inputs[6]));
        insert(t);
    }
}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction* p_new_trans) {
    if (p_head == nullptr) {
        this->p_head = p_new_trans;
    }
    else {
        Transaction* p{ p_head };

        while (p->get_next() != nullptr) {
            p = p->get_next();
        }
        p->set_next(p_new_trans);
    }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date() {
    // sort when insert
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl() {
    Transaction* p{ p_head };

    double acb = 0, cgl = 0;
    double acb_per_share = 0;
    int sb = 0;

    while (p->get_next() != nullptr) {
        if (p->get_trans_type()) acb += p->get_amount();
        else acb -= p->get_amount();
        p->set_acb(acb);

        if (p->get_trans_type()) sb += p->get_shares();
        else sb -= p->get_shares();
        p->set_share_balance(sb);

        double dsb = p->get_share_balance();
        acb_per_share = acb / dsb;
        p->set_acb_per_share(acb_per_share);

        cgl = p->get_amount() - p->get_shares() * p->get_acb_per_share();
        if (!p->get_trans_type()) p->set_cgl(cgl);
        else p->set_cgl(0);

        p = p->get_next();
    }
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year) {
    Transaction* p{ p_head };
    double cgl_of_year = 0;
    while (p->get_next() != nullptr) {
        if (p->get_year() == year) cgl_of_year += p->get_cgl();
        p = p->get_next();
    }

    return cgl_of_year;
}

// print() Print the transaction history.
//TASK 9
//
void History::print() {
    Transaction* p{ p_head };

    int index = 0;
    printf("========== BEGIN TRANSACTION HISTORY ============\n");
    while (p->get_next() != nullptr) {
        std::cout << index++<<"\t";
        std::cout << p->get_symbol() << "\t";
        std::cout << p->get_day() << "\t";
        std::cout << p->get_month() << "\t";
        std::cout << p->get_year() << "\t";
        if(p->get_trans_type() == 1) std::cout << "Buy" << "\t";
        else std::cout << "Sell" << "\t";
        std::cout << p->get_shares() << "\t";
        std::cout << p->get_amount() << "\t";
        std::cout << p->get_acb() << "\t";
        std::cout << p->get_share_balance() << "\t";
        std::cout << p->get_acb_per_share() << "\t";
        std::cout << p->get_cgl() << "\t" <<std::endl;
        p = p->get_next();
    }
    printf("========== END TRANSACTION HISTORY ============\n");
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
