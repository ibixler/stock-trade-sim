#include "api_manager.h"
#include "stock_data.h"
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkRequest>
#include <QDateTime>
#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDir>

ApiManager::ApiManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(std::make_unique<QNetworkAccessManager>(this))
    , m_timeoutTimer(new QTimer(this))
{
    setupNetworkManager();
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &ApiManager::onTimeout);
}

ApiManager::~ApiManager()
{
}

void ApiManager::setupNetworkManager()
{
    // Set up network manager with timeout
    connect(m_networkManager.get(), &QNetworkAccessManager::finished,
            this, &ApiManager::onNetworkReply);
}

void ApiManager::setApiKey(const std::string& apiKey)
{
    m_apiKey = apiKey;

    // Try to read API key from .env file if not set
    if (m_apiKey.empty()) {
        // Check for .env file in the project directory
        QDir projectDir("./");
        QFile envFile(projectDir.absoluteFilePath(".env"));
        if (envFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&envFile);
            QString line;
            while (!in.atEnd()) {
                line = in.readLine();
                if (line.startsWith("TWELVE_DATA_KEY=")) {
                    m_apiKey = line.mid(16).toStdString(); // Remove "TWELVE_DATA_KEY=" prefix
                    break;
                }
            }
            envFile.close();
        }
    }
    
    // Try to read API key from file if still not set
    if (m_apiKey.empty()) {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/stock_trading_simulator/api_key.txt";
        QFile file(configPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            m_apiKey = in.readLine().toStdString();
            file.close();
        }
    }
}

void ApiManager::fetchStockData(const std::string& symbol)
{
    if (m_apiKey.empty()) {
        emit errorOccurred("API key not set. Please provide a valid API key.");
        return;
    }

    QString url = QString::fromStdString(buildStockUrl(symbol));

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Add to request timestamps for tracking
    m_requestTimestamps[symbol] = QDateTime::currentDateTime();

    QNetworkReply* reply = m_networkManager->get(request);
    m_timeoutTimer->start(10000); // 10 second timeout

    Q_UNUSED(reply);
}

void ApiManager::fetchMultipleStocks(const std::vector<std::string>& symbols)
{
    if (m_apiKey.empty()) {
        emit errorOccurred("API key not set. Please provide a valid API key.");
        return;
    }

    // Build batch request for multiple stocks
    std::string symbolList;
    for (size_t i = 0; i < symbols.size(); ++i) {
        if (i > 0) symbolList += ",";
        symbolList += symbols[i];
    }

    QString url = QString("https://api.twelvedata.com/time_series?symbol=%1&interval=1min&outputsize=1&apikey=%2")
                  .arg(QString::fromStdString(symbolList))
                  .arg(QString::fromStdString(m_apiKey));

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = m_networkManager->get(request);
    m_timeoutTimer->start(10000); // 10 second timeout

    Q_UNUSED(reply);
}

void ApiManager::fetchStockHistory(const std::string& symbol, int days)
{
    if (m_apiKey.empty()) {
        emit errorOccurred("API key not set. Please provide a valid API key.");
        return;
    }

    QString url = QString::fromStdString(buildHistoryUrl(symbol, days));

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Add to request timestamps for tracking
    m_requestTimestamps[symbol] = QDateTime::currentDateTime();

    QNetworkReply* reply = m_networkManager->get(request);
    m_timeoutTimer->start(10000); // 10 second timeout

    Q_UNUSED(reply);
}

std::string ApiManager::buildStockUrl(const std::string& symbol)
{
    return "https://api.twelvedata.com/time_series?symbol=" + symbol +
           "&interval=1min&outputsize=1&apikey=" + m_apiKey;
}

std::string ApiManager::buildHistoryUrl(const std::string& symbol, int days)
{
    // For simplicity, we'll use daily data for history
    return "https://api.twelvedata.com/time_series?symbol=" + symbol +
           "&interval=1day&outputsize=" + std::to_string(days) +
           "&apikey=" + m_apiKey;
}

void ApiManager::onNetworkReply(QNetworkReply* reply)
{
    m_timeoutTimer->stop();

    if (reply->error() != QNetworkReply::NoError) {
        handleNetworkError(reply);
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || doc.isEmpty()) {
        emit errorOccurred("Failed to parse API response");
        reply->deleteLater();
        return;
    }

    QJsonObject root = doc.object();

    // Check if we have an error in the response
    if (root.contains("error")) {
        emit errorOccurred(root["error"].toString().toStdString());
        reply->deleteLater();
        return;
    }

    // Check if this is a stock data response
    if (root.contains("values")) {
        // This is a time series response
        QJsonArray values = root["values"].toArray();
        if (!values.isEmpty()) {
            QJsonObject latest = values[0].toObject();

            // Extract data from the response
            QString symbol = root["symbol"].toString();
            double price = latest["close"].toDouble();
            double open = latest["open"].toDouble();
            double change = price - open;
            double changePercent = (change / open) * 100;

            emit stockDataReceived(symbol.toStdString(), price, change, changePercent);
        }
    } else if (root.contains("data")) {
        // This might be a history
 response
        parseHistoryResponse("", root);
    }
    
    reply->deleteLater();
}

void ApiManager::onTimeout()
{
    emit errorOccurred("API request timeout");
}

void ApiManager::parseStockResponse(const std::string& symbol, const QJsonObject& data)
{
    // Parse stock data from API response
    // This is a simplified version - real implementation would need more detailed parsing
    if (data.contains("close")) {
        double price = data["close"].toDouble();
        double open = data["open"].toDouble();
        double change = price - open;
        double changePercent = (change / open) * 100;
        
        emit stockDataReceived(symbol, price, change, changePercent);
    }
}

void ApiManager::parseHistoryResponse(const std::string& symbol, const QJsonObject& data)
{
    std::vector<std::pair<QDateTime, double>> history;
    
    if (data.contains("values")) {
        QJsonArray values = data["values"].toArray();
        for (const QJsonValue& value : values) {
            QJsonObject obj = value.toObject();
            
            // Parse date and price
            QString datetimeStr = obj["datetime"].toString();
            double price = obj["close"].toDouble();
            
            // Convert datetime string to QDateTime
            QDateTime dateTime = QDateTime::fromString(datetimeStr, "yyyy-MM-dd HH:mm:ss");
            if (dateTime.isValid()) {
                history.emplace_back(dateTime, price);
            }
        }
        
        emit stockHistoryReceived(symbol, history);
    }
}

void ApiManager::handleNetworkError(QNetworkReply* reply)
{
    QString error = reply->errorString();
    emit errorOccurred(error.toStdString());
}

void ApiManager::onTimeout()
{
    emit errorOccurred("API request timeout");
}