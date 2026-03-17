#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QMessageBox>
#include <memory>
#include <vector>
#include <string>

QT_CHARTS_USE_NAMESPACE

class StockData;
class PortfolioManager;
class DatabaseManager;
class ApiManager;
class StockChartWidget;
class PortfolioWidget;
class WatchlistWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearch();
    void onBuyStock();
    void onAddToFavorites();
    void updateStockData();
    void onChartPointSelected(QPointF point);

private:
    void setupUI();
    void setupDatabase();
    void setupApi();
    void loadStockData();
    void setupConnections();
    void initializeData();
    void fetchLiveStockData();

    // UI Components
    QTabWidget *m_tabWidget;
    QWidget *m_searchTab;
    QWidget *m_portfolioTab;
    QWidget *m_watchlistTab;
    
    // Search tab widgets
    QLineEdit *m_searchLineEdit;
    QPushButton *m_searchButton;
    QTableWidget *m_stockTable;
    QPushButton *m_buyButton;
    
    // Portfolio tab widgets
    PortfolioWidget *m_portfolioWidget;
    
    // Watchlist tab widgets
    WatchlistWidget *m_watchlistWidget;
    
    // Data managers
    std::unique_ptr<DatabaseManager> m_dbManager;
    std::unique_ptr<PortfolioManager> m_portfolioManager;
    std::unique_ptr<ApiManager> m_apiManager;
    
    // Stock data
    std::vector<std::unique_ptr<StockData>> m_stocks;
    
    // Timer for live updates
    QTimer *m_updateTimer;
    
    // Database connection
    QSqlDatabase m_db;
};

#endif // MAIN_WINDOW_H