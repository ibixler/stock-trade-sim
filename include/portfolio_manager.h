#ifndef PORTFOLIO_MANAGER_H
#define PORTFOLIO_MANAGER_H

#include <string>
#include <memory>
#include <vector>
#include <QDateTime>
#include "database_manager.h"

class PortfolioManager
{
public:
    PortfolioManager(DatabaseManager* dbManager);
    ~PortfolioManager() = default;
    
    // Portfolio operations
    bool addStockToPortfolio(const std::string& symbol, int quantity, double purchasePrice);
    bool removeStockFromPortfolio(const std::string& symbol, int quantity);
    std::vector<std::tuple<std::string, int, double, QDateTime>> getPortfolioStocks() const;
    double getTotalPortfolioValue() const;
    double getPortfolioGainLoss() const;
    
    // Stock operations
    bool updateStockPrice(const std::string& symbol, double newPrice);
    
private:
    DatabaseManager* m_dbManager;
};

#endif // PORTFOLIO_MANAGER_H