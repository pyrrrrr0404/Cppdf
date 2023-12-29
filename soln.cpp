#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

struct TradeData {
    std::string TimeStamp;
    std::string Symbol;
    int Quantity;
    double Price;
};

std::vector<TradeData> parseCSV(const std::string& filename) {
    std::vector<TradeData> data;
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::istringstream s(line);
        TradeData td;
        std::string field;

        std::getline(s, td.TimeStamp, ',');
        std::getline(s, td.Symbol, ',');
        std::getline(s, field, ',');
        td.Quantity = std::stoi(field);
        std::getline(s, field, ',');
        td.Price = std::stod(field);

        data.push_back(td);
    }

    return data;
}

struct AggregatedData {
    double MaxPrice;
    int TotalVolume;
    double WeightedAvgPrice;
};

std::map<std::string, AggregatedData> processData(const std::vector<TradeData>& trades) {
    std::map<std::string, AggregatedData> aggregatedData;

    for (const auto& trade : trades) {
        AggregatedData& data = aggregatedData[trade.Symbol];
        data.MaxPrice = std::max(data.MaxPrice, trade.Price);
        data.TotalVolume += trade.Quantity;
        data.WeightedAvgPrice += trade.Quantity * trade.Price;
    }

    for (auto& [symbol, data] : aggregatedData) {
        if (data.TotalVolume > 0) {
            data.WeightedAvgPrice /= data.TotalVolume;
        }
    }

    return aggregatedData;
}

int main() {
    auto trades = parseCSV("input.csv");
    auto result = processData(trades);

    std::ofstream outputFile("output.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output.csv for writing." << std::endl;
        return 1;
    }

    for (const auto& pair : result) {
        const std::string& symbol = pair.first;
        const AggregatedData& data = pair.second;

        std::cout << "Writing data for symbol: " << symbol << std::endl; // Debug
        outputFile << symbol << "," << data.MaxPrice << "," << data.TotalVolume << "," << data.WeightedAvgPrice << "\n";
    }

    std::cout << "Data processing complete. Output written to output.csv" << std::endl;

    return 0;
}
