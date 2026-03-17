#ifndef WATCHLIST_WIDGET_H
#define WATCHLIST_WIDGET_H

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

class WatchlistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WatchlistWidget(QWidget *parent = nullptr);
    ~WatchlistWidget();

    void refreshWatchlist();

private:
    void setupUI();
    void setupConnections();
    void loadWatchlistData();

    // UI Components
    QTableWidget *m_watchlistTable;
    QPushButton *m_refreshButton;
    QPushButton *m_removeButton;
    QPushButton *m_addButton;
    
    // Data manager
    DatabaseManager* m_dbManager;
};

#endif // WATCHLIST_WIDGET_H