#include "main_window.h"
#include "stock_data.h"
#include "portfolio_manager.h"
#include "database_manager.h"
#include "api_manager.h"
#include "stock_chart_widget.h"
#include "portfolio_widget.h"
#include "watchlist_widget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QTimer>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QLabel>
#include <QSplitter>
#include <QGroupBox>
#include <QComboBox>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tabWidget(new QTabWidget)
    , m_searchTab(new QWidget)
    , m_portfolioTab(new QWidget)
    , m_watchlistTab(new QWidget)
    , m_searchLineEdit(new QLineEdit)
    , m_searchButton(new QPushButton("Search"))
    , m_stockTable(new QTableWidget)
    , m_buyButton(new QPushButton("Buy Stock"))
    , m_portfolioWidget(nullptr)
    , m_watchlistWidget(nullptr)
    , m_updateTimer(new QTimer(this))
    , m_apiManager(std::make_unique<ApiManager>())
{
    setupUI();
    setupDatabase();
    setupApi();
    setupConnections();
    initializeData();

    // Start live updates
    m_updateTimer->start(60000); // Update every minute
}

MainWindow::~MainWindow()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void MainWindow::setupUI()
{
    setWindowTitle("Stock Trading Simulator");
    resize(1200, 800);

    // Create main layout
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);

    // Create central widget and set layout
    auto *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Setup tabs
    m_tabWidget->addTab(m_searchTab, "Market Search");
    m_tabWidget->addTab(m_portfolioTab, "My Portfolio");
    m_tabWidget->addTab(m_watchlistTab, "Watchlist");

    // Setup search tab
    auto *searchLayout = new QVBoxLayout(m_searchTab);
    auto *searchBarLayout = new QHBoxLayout;

    searchBarLayout->addWidget(new QLabel("Search:"));
    searchBarLayout->addWidget(m_searchLineEdit);
    searchBarLayout->addWidget(m_searchButton);

    searchLayout->addLayout(searchBarLayout);

    // Stock table
    m_stockTable->setRowCount(0);
    m_stockTable->setColumnCount(5);
    m_stockTable->setHorizontalHeaderLabels({"Symbol", "Name", "Price", "Change", "Change %"});
    m_stockTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_stockTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_stockTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    searchLayout->addWidget(m_stockTable);

    // Buy button
    searchLayout->addWidget(m_buyButton);

    // Setup portfolio tab
    m_portfolioWidget = new PortfolioWidget(m_portfolioTab);
    auto *portfolioLayout = new QVBoxLayout(m_portfolioTab);
    portfolioLayout->addWidget(m_portfolioWidget);

    // Setup watchlist tab
    m_watchlistWidget = new WatchlistWidget(m_watchlistTab);
    auto *watchlistLayout = new QVBoxLayout(m_watchlistTab);
    watchlistLayout->addWidget(m_watchlistWidget);
}

void MainWindow::setupDatabase()
{
    // Create database connection
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/stock_trading.db";
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        QMessageBox::critical(this, "Database Error",
                             "Failed to open database: " + m_db.lastError().text());
        return;
    }

    // Create tables if they don't exist
    QSqlQuery query(m_db);

    // Create stocks table
    query.exec("CREATE TABLE IF NOT EXISTS stocks ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "symbol TEXT UNIQUE NOT NULL, "
               "name TEXT NOT NULL, "
               "current_price REAL NOT NULL, "
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");

    // Create portfolio table
    query.exec("CREATE TABLE IF NOT EXISTS portfolio ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "stock_id INTEGER, "
               "quantity INTEGER NOT NULL, "
               "purchase_price REAL NOT NULL, "
               "purchase_date DATETIME NOT NULL, "
               "FOREIGN KEY (stock_id) REFERENCES stocks (id))");

    // Create watchlist table
    query.exec("CREATE TABLE IF NOT EXISTS watchlist ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "stock_id INTEGER, "
               "added_date DATETIME DEFAULT CURRENT_TIMESTAMP, "
               "FOREIGN KEY (stock_id) REFERENCES stocks (id))");

    m_dbManager = std::make_unique<DatabaseManager>(m_db);
    m_portfolioManager = std::make_unique<PortfolioManager>(m_dbManager.get());
}

void MainWindow::setupConnections()
{
    connect(m_searchButton, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(m_buyButton, &QPushButton::clicked, this, &MainWindow::onBuyStock);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateStockData);
    connect(m_stockTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::onBuyStock);
    
    // Connect API signals
    if (m_apiManager) {
        connect(m_apiManager.get(), &ApiManager::stockDataReceived, 
                this, &MainWindow::onStockDataReceived);
        connect(m_apiManager.get(), &ApiManager::errorOccurred, 
                this, &MainWindow::onApiError);
    }
}
    // This will be implemented to update chart when stock is selected
}

void MainWindow::initializeData()
{
    // Load some initial stock data
    loadStockData();

    // Fetch live data
    fetchLiveStockData();
}

void MainWindow::loadStockData()
{
    // Load stock data from database or API
    m_stockTable->setRowCount(0);

    // For now, we'll add some mock data, but this would be replaced with real API calls
    std::vector<std::tuple<std::string, std::string, double, double, double>> mockStocks = {
        {"AAPL", "Apple Inc.", 175.43, 2.34, 1.35},
        {"MSFT", "Microsoft Corp.", 330.12, -1.23, -0.37},
        {"GOOGL", "Alphabet Inc.", 138.21, 4.56, 3.43},
        {"AMZN", "Amazon.com Inc.", 145.67, 3.45, 2.43},
        {"TSLA", "Tesla Inc.", 245.89, -5.67, -2.25},
        {"NVDA", "NVIDIA Corp.", 420.56, 12.34, 3.03},
        {"META", "Meta Platforms", 320.45, 8.90, 2.86},
        {"NFLX", "Netflix Inc.", 410.23, -2.34, -0.57}
    };

    for (const auto& stock : mockStocks) {
        int row = m_stockTable->rowCount();
        m_stockTable->insertRow(row);

        m_stockTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(std::get<0>(stock))));
        m_stockTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(std::get<1>(stock))));
        m_stockTable->setItem(row, 2, new QTableWidgetItem(QString::number(std::get<2>(stock), 'f', 2)));
        m_stockTable->setItem(row, 3, new QTableWidgetItem(QString::number(std::get<3>(stock), 'f', 2)));
        m_stockTable->setItem(row, 4, new QTableWidgetItem(QString::number(std::get<4>(stock), 'f', 2) + "%"));
    }
}

void MainWindow::onSearch()
{
    QString searchTerm = m_searchLineEdit->text().trimmed();
    if (searchTerm.isEmpty()) {
        loadStockData();
        return;
    }

    // In a real app, this would search the database or API
    // For now, we'll simulate a search with mock data
    loadStockData();
}

void MainWindow::onBuyStock()
{
    int currentRow = m_stockTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a stock to buy.");
        return;
    }

    QString symbol = m_stockTable->item(currentRow, 0)->text();
    QString priceStr = m_stockTable->item(currentRow, 2)->text();
    double price = priceStr.toDouble();

    // Create buy dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Buy Stock");
    dialog.setModal(true);

    auto *layout = new QVBoxLayout(&dialog);
    auto *formLayout = new QFormLayout;

    auto *quantitySpinBox = new QSpinBox(&dialog);
    quantitySpinBox->setRange(1, 10000);
    quantitySpinBox->setValue(1);

    auto *priceLabel = new QLabel(QString("Price: $%1").arg(price, 0, 'f', 2), &dialog);
    priceLabel->setStyleSheet("font-weight: bold;");

    formLayout->addRow("Quantity:", quantitySpinBox);
    formLayout->addRow("Price:", priceLabel);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int quantity = quantitySpinBox->value();
        double totalCost = quantity * price;

        // In a real app, we would check user's balance and process the transaction
        QMessageBox::information(this, "Purchase Successful",
                                QString("Successfully bought %1 shares of %2 for $%3")
                                .arg(quantity).arg(symbol).arg(totalCost, 0, 'f', 2));

        // Add to portfolio
        if (m_portfolioManager) {
            m_portfolioManager->addStockToPortfolio(symbol.toStdString(), quantity, price);
            m_portfolioWidget->refreshPortfolio();
        }
    }
}

void MainWindow::updateStockData()
{
    // Fetch live data from API
    fetchLiveStockData();
}

void MainWindow::onAddToFavorites()
{
    int currentRow = m_stockTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a stock to add to favorites.");
        return;
    }

    QString symbol = m_stockTable->item(currentRow, 0)->text();

    if (m_dbManager) {
        m_dbManager->addToWatchlist(symbol.toStdString());
        QMessageBox::information(this, "Added to Watchlist",
                                QString("Successfully added %1 to watchlist").arg(symbol));
    }
}

void MainWindow::onChartPointSelected(QPointF point)
{
    // This will be implemented in the chart widget
    qDebug() << "Point selected on chart:" << point;
}

void MainWindow::setupApi()
{
    // Try to get API key from .env file
    QDir projectDir("./");
    QFile envFile(projectDir.absoluteFilePath(".env"));
    if (envFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&envFile);
        QString line;
        while (!in.atEnd()) {
            line = in.readLine();
            if (line.startsWith("TWELVE_DATA_KEY=")) {
                std::string apiKey = line.mid(16).toStdString(); // Remove "TWELVE_DATA_KEY=" prefix
                m_apiManager->setApiKey(apiKey);
                break;
            }
        }
        envFile.close();
    }
    
    // Try to get API key from environment variable if not found in .env
    if (!m_apiManager->isApiKeySet()) {
        // No need for duplicate setupApi function - it's already defined

void MainWindow::fetchLiveStockData()
{
    if (m_apiManager && m_apiManager->isApiKeySet()) {
        // Fetch data for a few key stocks
        std::vector<std::string> symbols = {"AAPL", "MSFT", "GOOGL", "AMZN", "TSLA", "NVDA"};
        m_apiManager->fetchMultipleStocks(symbols);
    }
}

void MainWindow::onStockDataReceived(const std::string& symbol, double price, double change, double changePercent)
{
    // Update the stock table with real data
    for (int i = 0; i < m_stockTable->rowCount(); ++i) {
        if (m_stockTable->item(i, 0)->text() == QString::fromStdString(symbol)) {
            m_stockTable->item(i, 2)->setText(QString::number(price, 'f', 2));
            m_stockTable->item(i, 3)->setText(QString::number(change, 'f', 2));
            m_stockTable->item(i, 4)->setText(QString::number(changePercent, 'f', 2) + "%");
            break;
        }
    }
}

void MainWindow::onApiError(const std::string& error)
{
    qDebug() << "API Error:" << QString::fromStdString(error);
    QMessageBox::warning(this, "API Error", QString::fromStdString(error));
}
