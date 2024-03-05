#include "OrderBook.h"
#include<fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

void printBook(std::unordered_map<std::string, OrderBook>& order_books) {
    int count = 0;
    for (auto& sym_ob : order_books) {
        std::cout << "Symbol: " << sym_ob.first << "\n";
        std::cout << "Bids:\n";
        count = 0;
        for (const auto& bid : sym_ob.second.bids) {
            count++;
        if(count == 10) break;
            std::cout << "Price : " << bid.price << ", Quantity : " << bid.quantity << "\n";
        }
        std::cout << "Best Bid: " << sym_ob.second.get_best_bid() << "\n";
        std::cout << "Asks:\n";
        count = 0;
        for (const auto& ask : sym_ob.second.asks) {
            count++;
        if(count == 10) break;
            std::cout << "Price : " << ask.price << ", Quantity : " << ask.quantity << "\n";
        }
        std::cout << "Best Ask: " << sym_ob.second.get_best_ask() << "\n";
    }
    std::cout << std::endl;
}

int main() {
    std::unordered_map<std::string, OrderBook> order_books;

    // Example usage
    
    {
        std::ifstream f("snapshot.json");
        json data = json::parse(f);

        vector<std::pair<double, double>> bid, ask;
        for(const auto& item : data["bids"]) {
            bid.push_back(make_pair(std::stod(string(item[0])), std::stod(string(item[1]))));
        }
        for(const auto& item : data["asks"]) {
            ask.push_back(make_pair(std::stod(string(item[0])), std::stod(string(item[1]))));
        }

        order_books["BTC"].process_snapshot(true, bid);
        order_books["BTC"].process_snapshot(false, ask);

        printBook(order_books);
    }
    std::cout<<"\n";
    {
        std::ifstream file("updates.json");
        std::string line;

        while (std::getline(file, line)) {
            json j = json::parse(line);
            vector<std::pair<double, double>> bid, ask;
            for(const auto& item : j["b"]) {
                bid.push_back(make_pair(std::stod(string(item[0])), std::stod(string(item[1]))));
            }
            for(const auto& item : j["a"]) {
                ask.push_back(make_pair(std::stod(string(item[0])), std::stod(string(item[1]))));
            }

            order_books["BTC"].process_update(true, bid);
            order_books["BTC"].process_update(false, ask);
            std::cout<<"\n";
            printBook(order_books);
        }
    }
    /*
    order_books["BNBBTC"].process_snapshot(true, {{0.0024,14.70000000}, {0.0022,6.40000000}, {0.0020, 9.70000000}});
    order_books["BNBBTC"].process_snapshot(false, {{0.0024,14.90000000},{0.0026, 3.60000000},{0.0028, 1.00000000}});

    printBook(order_books);

    order_books["BNBBTC"].process_update(true, {{0.0024, 10}});
    order_books["BNBBTC"].process_update(false,{{0.0026, 100}});
    
    printBook(order_books);
    
    order_books["BNBBTC"].process_update(true, {{0.0024, 8}});
    order_books["BNBBTC"].process_update(false,{{0.0028, 0}});
    
    printBook(order_books);
    
    order_books["BNBBTC"].process_update(true, {{0.0024, 0}});
    order_books["BNBBTC"].process_update(false,{{0.0026, 15}, {0.0027, 5}});
    
    printBook(order_books);
    
    order_books["BNBBTC"].process_update(true, {{0.0025, 100}});
    order_books["BNBBTC"].process_update(false,{{0.0026, 0}, {0.0027, 5}});
    
    printBook(order_books);
    
    order_books["BNBBTC"].process_update(true, {{0.0025, 0}});
    order_books["BNBBTC"].process_update(false,{{0.0026, 15}, {0.0024, 0}});

    printBook(order_books);*/
    
    //order_books["ETHUSDT"].update({{200.0, 10.00, 0}, {200.0, 50.0, 1}, {200.1, 10.0, 0}, {200.1, 50.0, 1}});

    return 0;
}