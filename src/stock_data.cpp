#include "stock_data.h"
#include <QDateTime>

StockData::StockData(const std::string& symbol, const std::string& name, double price)
    : m_symbol(symbol)
    , m_name(name)
    , m_price(price)
    , m_change(0.0)
    , m_changePercent(0.0)
    , m_timestamp(QDateTime::currentDateTime())
{
}

void StockData::setPrice(double price)
{
    // Calculate change if this is not the first price
    if (m_price > 0) {
        m_change = price - m_price;
        m_changePercent = (m_change / m_price) * 100;
    }
    m_price = price;
    m_timestamp = QDateTime::currentDateTime();
}

void StockData::addPriceHistory(const QDateTime& timestamp, double price)
{
    m_priceHistory.emplace_back(timestamp, price);
}