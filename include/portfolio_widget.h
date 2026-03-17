#ifndef PORTFOLIO_WIDGET_H
#define PORTFOLIO_WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <memory>
#include <vector>
#include <string>

class DatabaseManager;
class PortfolioManager;

class PortfolioWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PortfolioWidget(QWidget *parent = nullptr);
    ~PortfolioWidget();

    void refreshPortfolio();

private:
    void setupUI();
    void setupConnections();
    void loadPortfolioData();

    // UI Components
    QTableWidget *m_portfolioTable;
    QPushButton *m_refreshButton;
    QPushButton *m_sellButton;
    QLabel *m_totalValueLabel;
    
    // Data managers
    DatabaseManager* m_dbManager;
    PortfolioManager* m_portfolioManager;
};

#endif // PORTFOLIO_WIDGET_H