#pragma once
#include <iostream>

struct PricePoint {
    double price;
    double quantity;

    PricePoint(double price, double quantity) : price(price), quantity(quantity) {}
};

class OrderBook {
public:
    OrderBook() {
        bids.reserve(1024);
        asks.reserve(1024);
    }
    void process_snapshot(bool is_bid, const std::vector<std::pair<double, double>>& snapshot);
    void process_update(bool is_bid, const std::vector<std::pair<double, double>>& updates);
    double get_best_bid();
    double get_best_ask();
    friend void printBook(std::unordered_map<std::string, OrderBook>& order_books);
private:
    void insert_price_point(bool is_bid, double price, double quantity, std::vector<PricePoint>::iterator side_end);
    void remove_price_point(bool is_bid, double price);
    void update_best_prices();

    std::vector<PricePoint> bids; // descending
    std::vector<PricePoint> asks; // ascending
    double best_ask, best_bid;
};