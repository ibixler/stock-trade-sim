#ifndef STOCK_DATA_H
#define STOCK_DATA_H

#include <string>
#include <memory>
#include <vector>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>

class StockData
{
public:
    StockData(const std::string& symbol, const std::string& name, double price);
    ~StockData() = default;
    
    // Getters
    const std::string& getSymbol() const { return m_symbol; }
    const std::string& getName() const { return m_name; }
    double getPrice() const { return m_price; }
    double getChange() const { return m_change; }
    double getChangePercent() const { return m_changePercent; }
    const QDateTime& getTimestamp() const { return m_timestamp; }
    const std::vector<std::pair<QDateTime, double>>& getPriceHistory() const { return m_priceHistory; }
    
    // Setters
    void setPrice(double price);
    void setChange(double change) { m_change = change; }
    void setChangePercent(double changePercent) { m_changePercent = changePercent; }
    void addPriceHistory(const QDateTime& timestamp, double price);
    
    // Utility methods
    double getCurrentValue() const { return m_price; }
    double getTotalValue(int quantity) const { return m_price * quantity; }
    
private:
    std::string m_symbol;
    std::string m_name;
    double m_price;
    double m_change;
    double m_changePercent;
    QDateTime m_timestamp;
    std::vector<std::pair<QDateTime, double>> m_priceHistory;
};

#endif // STOCK_DATA_H