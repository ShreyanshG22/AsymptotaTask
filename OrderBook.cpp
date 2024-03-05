#include "OrderBook.h"
#include <chrono>

void OrderBook::process_snapshot(bool is_bid, const std::vector<std::pair<double, double>>& snapshot) {
    auto start = std::chrono::high_resolution_clock::now();
    
    auto& side = is_bid ? bids : asks;
 
    for (const auto& snap : snapshot) 
    {
        side.emplace_back(snap.first, snap.second);
    }

    std::sort(side.begin(), side.end(), [is_bid](const PricePoint& a, const PricePoint& b) {
                if(is_bid)
                    return a.price > b.price;
                else
                    return a.price < b.price;
            });

    update_best_prices();
    auto end = std::chrono::high_resolution_clock::now();
 
    // Calculating total time taken by the program.
    double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout<<"Time taken per Total per snapshot process = " << time_taken << " nanoseconds\n";
}

void OrderBook::process_update(bool is_bid, const std::vector<std::pair<double, double>>& updates) {
    auto start = std::chrono::high_resolution_clock::now();
    
    auto& side = is_bid ? bids : asks;
    auto side_end = side.end();
    std::vector<double/*price*/> delete_price;
    for (const auto& update : updates) {
        if (update.second /*quantity*/ > 0) {
            insert_price_point(is_bid, update.first /*price*/, update.second /*quantity*/, side_end);
        } else {
            delete_price.emplace_back(update.first);
        }
    }

    //auto start = std::chrono::high_resolution_clock::now();
    
    for (const auto& del_price: delete_price) {
        remove_price_point(is_bid, del_price /*price*/);
    }

    std::sort(side.begin(), side.end(), [is_bid](const PricePoint& a, const PricePoint& b) {
                if(is_bid)
                    return a.price > b.price;
                else
                    return a.price < b.price;
            });

    //Remove logic: remove a batch at last of processing.. much faster
    
    double price = is_bid ? -1 : INT_MAX;
    auto it = std::lower_bound(side.begin(), side.end(), price, [is_bid](const PricePoint& p, double price) {
                return is_bid ? p.price > price : p.price < price;
            });
    if(it != side.end() && it->price == price)
        side.erase(it, side.end());

    update_best_prices();

    auto end = std::chrono::high_resolution_clock::now();
 
    // Calculating total time taken by the program.
    double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout<<"Time taken per Total Update = " << time_taken << " nanoseconds\n";
}

void OrderBook::insert_price_point(bool is_bid, double price, double quantity, std::vector<PricePoint>::iterator side_end) {
    //auto start = chrono::high_resolution_clock::now();
    auto& side = is_bid ? bids : asks;
    auto it = std::lower_bound(side.begin(), side_end, price, [is_bid](const PricePoint& p, double price) {
                if(is_bid)
                    return p.price > price;
                else
                    return p.price < price;
            });
    if (it != side.end() && it->price == price) {
        it->quantity = quantity;
    }
    else {
        side.emplace(it, price, quantity);
    }
    //auto end = chrono::high_resolution_clock::now();
 
    // Calculating total time taken by the program.
    //double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    //std::cout<<"Time taken per Update = " << time_taken << " ns\n";
}

void OrderBook::remove_price_point(bool is_bid, double price) {
    //auto start = chrono::high_resolution_clock::now();
    auto& side = is_bid ? bids : asks;
    auto it = std::lower_bound(side.begin(), side.end(), price, [is_bid](const PricePoint& p, double price) {
                        if(is_bid)
                        return p.price > price;
                    else
                        return p.price < price;
            });
    if (it != side.end() && it->price == price) {
        //side.erase(it);
        it->price = is_bid ? -1 : INT_MAX;
    }
    //auto end = chrono::high_resolution_clock::now();
 
    // Calculating total time taken by the program.
    //double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    //std::cout<<"Time taken per Removal = " << time_taken << " ns\n";
}

void OrderBook::update_best_prices() {
    if (!bids.empty()) {
        best_bid = bids.front().price;
    } else {
        best_bid = 0;
    }

    if (!asks.empty()) {
        best_ask = asks.front().price;
    } else {
        best_ask = 0;
    }
}

double OrderBook::get_best_bid() {
    return best_bid;
}

double OrderBook::get_best_ask() {
    return best_ask;
}