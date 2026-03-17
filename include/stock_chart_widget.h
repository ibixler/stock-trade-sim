#ifndef STOCK_CHART_WIDGET_H
#define STOCK_CHART_WIDGET_H

#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QMouseEvent>
#include <QPointF>
#include <QTimer>
#include <memory>
#include <vector>
#include <string>

QT_CHARTS_USE_NAMESPACE

class StockData;

class StockChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StockChartWidget(QWidget *parent = nullptr);
    ~StockChartWidget();

    void setStockData(const std::string& symbol, const std::vector<std::pair<QDateTime, double>>& data);
    void clearChart();

signals:
    void pointSelected(QPointF point);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setupChart();
    void updateChart();
    void addDataPoint(const QDateTime& timestamp, double price);
    void updateChartWithSelection(QPointF point);

    QChartView *m_chartView;
    QChart *m_chart;
    QLineSeries *m_series;
    QDateTimeAxis *m_xAxis;
    QValueAxis *m_yAxis;
    
    std::string m_currentSymbol;
    std::vector<std::pair<QDateTime, double>> m_chartData;
    
    QPointF m_selectionStart;
    QPointF m_selectionEnd;
    bool m_isSelecting;
    QTimer *m_selectionTimer;
};

#endif // STOCK_CHART_WIDGET_H