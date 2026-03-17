#include "watchlist_widget.h"
#include "database_manager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QDateTime>
#include <QDebug>

WatchlistWidget::WatchlistWidget(QWidget *parent)
    : QWidget(parent)
    , m_watchlistTable(new QTableWidget)
    , m_refreshButton(new QPushButton("Refresh"))
    , m_removeButton(new QPushButton("Remove from Watchlist"))
    , m_addButton(new QPushButton("Add to Watchlist"))
    , m_dbManager(nullptr)
{
    setupUI();
    setupConnections();
    loadWatchlistData();
}

WatchlistWidget::~WatchlistWidget()
{
}

void WatchlistWidget::setupUI()
{
    // Set up table
    m_watchlistTable->setRowCount(0);
    m_watchlistTable->setColumnCount(4);
    m_watchlistTable->setHorizontalHeaderLabels({"Symbol", "Name", "Price", "Added Date"});
    m_watchlistTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_watchlistTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_watchlistTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Set up layout
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_watchlistTable);
    
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_refreshButton);
    
    layout->addLayout(buttonLayout);
}

void WatchlistWidget::setupConnections()
{
    connect(m_refreshButton, &QPushButton::clicked, this, &WatchlistWidget::refreshWatchlist);
    connect(m_removeButton, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "Remove from Watchlist", "Remove from watchlist functionality would be implemented here.");
    });
    connect(m_addButton, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "Add to Watchlist", "Add to watchlist functionality would be implemented here.");
    });
}

void WatchlistWidget::refreshWatchlist()
{
    loadWatchlistData();
}

void WatchlistWidget::loadWatchlistData()
{
    // Clear existing data
    m_watchlistTable->setRowCount(0);
    
    // In a real implementation, we would fetch from the database
    // For now, we'll add some mock data
    m_watchlistTable->setRowCount(4);
    
    // Mock watchlist data
    m_watchlistTable->setItem(0, 0, new QTableWidgetItem("AAPL"));
    m_watchlistTable->setItem(0, 1, new QTableWidgetItem("Apple Inc."));
    m_watchlistTable->setItem(0, 2, new QTableWidgetItem("175.43"));
    m_watchlistTable->setItem(0, 3, new QTableWidgetItem("2023-03-17"));
    
    m_watchlistTable->setItem(1, 0, new QTableWidgetItem("MSFT"));
    m_watchlistTable->setItem(1, 1, new QTableWidgetItem("Microsoft Corp."));
    m_watchlistTable->setItem(1, 2, new QTableWidgetItem("330.12"));
    m_watchlistTable->setItem(1, 3, new QTableWidgetItem("2023-03-17"));
    
    m_watchlistTable->setItem(2, 0, new QTableWidgetItem("GOOGL"));
    m_watchlistTable->setItem(2, 1, new QTableWidgetItem("Alphabet Inc."));
    m_watchlistTable->setItem(2, 2, new QTableWidgetItem("138.21"));
    m_watchlistTable->setItem(2, 3, new QTableWidgetItem("2023-03-17"));
    
    m_watchlistTable->setItem(3, 0, new QTableWidgetItem("NVDA"));
    m_watchlistTable->setItem(3, 1, new QTableWidgetItem("NVIDIA Corp."));
    m_watchlistTable->setItem(3, 2, new QTableWidgetItem("420.56"));
    m_watchlistTable->setItem(3, 3, new QTableWidgetItem("2023-03-17"));
}