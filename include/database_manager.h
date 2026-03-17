#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>
#include <string>
#include <memory>
#include <vector>

class StockData;

class DatabaseManager
{
public:
    DatabaseManager(const QSqlDatabase& db);
    ~DatabaseManager() = default;
    
    // Stock operations
    bool addStock(const std::string& symbol, const std::string& name, double price);
    bool updateStockPrice(const std::string& symbol, double price);
    std::unique_ptr<StockData> getStock(const std::string& symbol);
    std::vector<std::unique_ptr<StockData>> getAllStocks();
    
    // Portfolio operations
    bool addStockToPortfolio(int stockId, int quantity, double purchasePrice, const QDateTime& purchaseDate);
    std::vector<std::tuple<int, std::string, int, double, QDateTime>> getPortfolioStocks();
    
    // Watchlist operations
    bool addToWatchlist(const std::string& symbol);
    bool removeFromWatchlist(const std::string& symbol);
    std::vector<std::string> getWatchlistSymbols();
    
    // Helper methods
    int getStockId(const std::string& symbol);
    bool stockExists(const std::string& symbol);
    
private:
    QSqlDatabase m_db;
};

#endif // DATABASE_MANAGER_H