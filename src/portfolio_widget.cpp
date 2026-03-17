#include "portfolio_widget.h"
#include "database_manager.h"
#include "portfolio_manager.h"
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

PortfolioWidget::PortfolioWidget(QWidget *parent)
    : QWidget(parent)
    , m_portfolioTable(new QTableWidget)
    , m_refreshButton(new QPushButton("Refresh"))
    , m_sellButton(new QPushButton("Sell Stock"))
    , m_totalValueLabel(new QLabel)
    , m_dbManager(nullptr)
    , m_portfolioManager(nullptr)
{
    setupUI();
    setupConnections();
    loadPortfolioData();
}

PortfolioWidget::~PortfolioWidget()
{
}

void PortfolioWidget::setupUI()
{
    // Set up table
    m_portfolioTable->setRowCount(0);
    m_portfolioTable->setColumnCount(6);
    m_portfolioTable->setHorizontalHeaderLabels({"Symbol", "Quantity", "Purchase Price", "Current Price", "Value", "Gain/Loss"});
    m_portfolioTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_portfolioTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_portfolioTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Set up layout
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_portfolioTable);
    
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_refreshButton);
    buttonLayout->addWidget(m_sellButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_totalValueLabel);
    
    layout->addLayout(buttonLayout);
}

void PortfolioWidget::setupConnections()
{
    connect(m_refreshButton, &QPushButton::clicked, this, &PortfolioWidget::refreshPortfolio);
    connect(m_sellButton, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "Sell Stock", "Sell stock functionality would be implemented here.");
    });
}

void PortfolioWidget::refreshPortfolio()
{
    loadPortfolioData();
}

void PortfolioWidget::loadPortfolioData()
{
    // Clear existing data
    m_portfolioTable->setRowCount(0);
    
    // In a real implementation, we would fetch from the database
    // For now, we'll add some mock data
    m_portfolioTable->setRowCount(3);
    
    // Mock portfolio data
    m_portfolioTable->setItem(0, 0, new QTableWidgetItem("AAPL"));
    m_portfolioTable->setItem(0, 1, new QTableWidgetItem("10"));
    m_portfolioTable->setItem(0, 2, new QTableWidgetItem("170.00"));
    m_portfolioTable->setItem(0, 3, new QTableWidgetItem("175.43"));
    m_portfolioTable->setItem(0, 4, new QTableWidgetItem("1754.30"));
    m_portfolioTable->setItem(0, 5, new QTableWidgetItem("+54.30"));
    
    m_portfolioTable->setItem(1, 0, new QTableWidgetItem("MSFT"));
    m_portfolioTable->setItem(1, 1, new QTableWidgetItem("5"));
    m_portfolioTable->setItem(1, 2, new QTableWidgetItem("325.00"));
    m_portfolioTable->setItem(1, 3, new QTableWidgetItem("330.12"));
    m_portfolioTable->setItem(1, 4, new QTableWidgetItem("1650.60"));
    m_portfolioTable->setItem(1, 5, new QTableWidgetItem("+25.60"));
    
    m_portfolioTable->setItem(2, 0, new QTableWidgetItem("NVDA"));
    m_portfolioTable->setItem(2, 1, new QTableWidgetItem("3"));
    m_portfolioTable->setItem(2, 2, new QTableWidgetItem("400.00"));
    m_portfolioTable->setItem(2, 3, new QTableWidgetItem("420.56"));
    m_portfolioTable->setItem(2, 4, new QTableWidgetItem("1261.68"));
    m_portfolioTable->setItem(2, 5, new QTableWidgetItem("+61.68"));
    
    // Update total value label
    double totalValue = 4666.58;
    m_totalValueLabel->setText(QString("Total Portfolio Value: $%1").arg(totalValue, 0, 'f', 2));
}