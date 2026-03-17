#ifndef API_MANAGER_H
#define API_MANAGER_H

#include <string>
#include <memory>
#include <vector>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QDateTime>
#include <QMap>

class StockData;

class ApiManager : public QObject
{
    Q_OBJECT

public:
    explicit ApiManager(QObject *parent = nullptr);
    ~ApiManager();

    // Fetch stock data from Twelve Data API
    void fetchStockData(const std::string& symbol);
    void fetchMultipleStocks(const std::vector<std::string>& symbols);
    void fetchStockHistory(const std::string& symbol, int days = 30);
    
    // Set API key
    void setApiKey(const std::string& apiKey);
    
    // Check if API key is set
    bool isApiKeySet() const { return !m_apiKey.empty(); }

signals:
    void stockDataReceived(const std::string& symbol, double price, double change, double changePercent);
    void stockHistoryReceived(const std::string& symbol, const std::vector<std::pair<QDateTime, double>>& history);
    void errorOccurred(const std::string& error);

private slots:
    void onNetworkReply(QNetworkReply* reply);
    void onTimeout();

private:
    void setupNetworkManager();
    std::string buildStockUrl(const std::string& symbol);
    std::string buildHistoryUrl(const std::string& symbol, int days);
    void parseStockResponse(const std::string& symbol, const QJsonObject& data);
    void parseHistoryResponse(const std::string& symbol, const QJsonObject& data);
    void handleNetworkError(QNetworkReply* reply);

    std::string m_apiKey;
    std::unique_ptr<QNetworkAccessManager> m_networkManager;
    QTimer* m_timeoutTimer;
    QMap<std::string, QDateTime> m_requestTimestamps;
};

#endif // API_MANAGER_H