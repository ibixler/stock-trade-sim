#include "database_manager.h"
#include "stock_data.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

DatabaseManager::DatabaseManager(const QSqlDatabase& db)
    : m_db(db)
{
}

bool DatabaseManager::addStock(const std::string& symbol, const std::string& name, double price)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT OR REPLACE INTO stocks (symbol, name, current_price) VALUES (?, ?, ?)");
    query.addBindValue(QString::fromStdString(symbol));
    query.addBindValue(QString::fromStdString(name));
    query.addBindValue(price);
    
    if (!query.exec()) {
        qDebug() << "Database error (addStock):" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::updateStockPrice(const std::string& symbol, double price)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE stocks SET current_price = ?, updated_at = CURRENT_TIMESTAMP WHERE symbol = ?");
    query.addBindValue(price);
    query.addBindValue(QString::fromStdString(symbol));
    
    if (!query.exec()) {
        qDebug() << "Database error (updateStockPrice):" << query.lastError().text();
        return false;
    }
    
    return true;
}

std::unique_ptr<StockData> DatabaseManager::getStock(const std::string& symbol)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id, symbol, name, current_price FROM stocks WHERE symbol = ?");
    query.addBindValue(QString::fromStdString(symbol));
    
    if (!query.exec()) {
        qDebug() << "Database error (getStock):" << query.lastError().text();
        return nullptr;
    }
    
    if (query.next()) {
        auto stock = std::make_unique<StockData>(
            query.value("symbol").toString().toStdString(),
            query.value("name").toString().toStdString(),
            query.value("current_price").toDouble()
        );
        return stock;
    }
    
    return nullptr;
}

std::vector<std::unique_ptr<StockData>> DatabaseManager::getAllStocks()
{
    std::vector<std::unique_ptr<StockData>> stocks;
    
    QSqlQuery query(m_db);
    query.prepare("SELECT id, symbol, name, current_price FROM stocks");
    
    if (!query.exec()) {
        qDebug() << "Database error (getAllStocks):" << query.lastError().text();
        return stocks;
    }
    
    while (query.next()) {
        auto stock = std::make_unique<StockData>(
            query.value("symbol").toString().toStdString(),
            query.value("name").toString().toStdString(),
            query.value("current_price").toDouble()
        );
        stocks.push_back(std::move(stock));
    }
    
    return stocks;
}

bool DatabaseManager::addStockToPortfolio(int stockId, int quantity, double purchasePrice, const QDateTime& purchaseDate)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO portfolio (stock_id, quantity, purchase_price, purchase_date) VALUES (?, ?, ?, ?)");
    query.addBindValue(stockId);
    query.addBindValue(quantity);
    query.addBindValue(purchasePrice);
    query.addBindValue(purchaseDate);
    
    if (!query.exec()) {
        qDebug() << "Database error (addStockToPortfolio):" << query.lastError().text();
        return false;
    }
    
    return true;
}

std::vector<std::tuple<int, std::string, int, double, QDateTime>> DatabaseManager::getPortfolioStocks()
{
    std::vector<std::tuple<int, std::string, int, double, QDateTime>> stocks;
    
    QSqlQuery query(m_db);
    query.prepare("SELECT p.id, s.symbol, p.quantity, p.purchase_price, p.purchase_date "
                  "FROM portfolio p "
                  "JOIN stocks s ON p.stock_id = s.id");
    
    if (!query.exec()) {
        qDebug() << "Database error (getPortfolioStocks):" << query.lastError().text();
        return stocks;
    }
    
    while (query.next()) {
        auto stock = std::make_tuple(
            query.value("id").toInt(),
            query.value("symbol").toString().toStdString(),
            query.value("quantity").toInt(),
            query.value("purchase_price").toDouble(),
            query.value("purchase_date").toDateTime()
        );
        stocks.push_back(stock);
    }
    
    return stocks;
}

bool DatabaseManager::addToWatchlist(const std::string& symbol)
{
    int stockId = getStockId(symbol);
    if (stockId <= 0) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("INSERT OR IGNORE INTO watchlist (stock_id) VALUES (?)");
    query.addBindValue(stockId);
    
    if (!query.exec()) {
        qDebug() << "Database error (addToWatchlist):" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::removeFromWatchlist(const std::string& symbol)
{
    int stockId = getStockId(symbol);
    if (stockId <= 0) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM watchlist WHERE stock_id = ?");
    query.addBindValue(stockId);
    
    if (!query.exec()) {
        qDebug() << "Database error (removeFromWatchlist):" << query.lastError().text();
        return false;
    }
    
    return true;
}

std::vector<std::string> DatabaseManager::getWatchlistSymbols()
{
    std::vector<std::string> symbols;
    
    QSqlQuery query(m_db);
    query.prepare("SELECT s.symbol FROM watchlist w "
                  "JOIN stocks s ON w.stock_id = s.id");
    
    if (!query.exec()) {
        qDebug() << "Database error (getWatchlistSymbols):" << query.lastError().text();
        return symbols;
    }
    
    while (query.next()) {
        symbols.push_back(query.value("symbol").toString().toStdString());
    }
    
    return symbols;
}

int DatabaseManager::getStockId(const std::string& symbol)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM stocks WHERE symbol = ?");
    query.addBindValue(QString::fromStdString(symbol));
    
    if (!query.exec()) {
        qDebug() << "Database error (getStockId):" << query.lastError().text();
        return -1;
    }
    
    if (query.next()) {
        return query.value("id").toInt();
    }
    
    return -1;
}

bool DatabaseManager::stockExists(const std::string& symbol)
{
    return getStockId(symbol) > 0;
}