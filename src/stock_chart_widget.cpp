#include "stock_chart_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QDebug>

StockChartWidget::StockChartWidget(QWidget *parent)
    : QWidget(parent)
    , m_chartView(new QChartView)
    , m_chart(new QChart)
    , m_series(new QLineSeries)
    , m_xAxis(new QDateTimeAxis)
    , m_yAxis(new QValueAxis)
    , m_isSelecting(false)
    , m_selectionTimer(new QTimer(this))
{
    setupChart();
    updateChart();
    
    // Connect selection timer
    connect(m_selectionTimer, &QTimer::timeout, [this]() {
        if (m_isSelecting) {
            m_isSelecting = false;
            // Emit the selected point when timer expires
        }
    });
}

StockChartWidget::~StockChartWidget()
{
}

void StockChartWidget::setupChart()
{
    // Set up chart
    m_chart->addSeries(m_series);
    m_chart->setTheme(QChart::ChartThemeLight);
    m_chart->setTitle("Stock Price Chart");
    
    // Set up axes
    m_xAxis->setFormat("MM/dd\nhh:mm");
    m_xAxis->setTitleText("Time");
    m_xAxis->setLabelsAngle(45);
    
    m_yAxis->setTitleText("Price ($)");
    
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);
    
    m_series->attachAxis(m_xAxis);
    m_series->attachAxis(m_yAxis);
    
    // Set up chart view
    m_chartView->setChart(m_chart);
    m_chartView->setRenderHint(QChartView::Antialiasing);
    m_chartView->setRubberBand(QChartView::RectangleRubberBand);
    
    // Set up layout
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    
    // Set minimum size
    setMinimumSize(600, 400);
}

void StockChartWidget::setStockData(const std::string& symbol, const std::vector<std::pair<QDateTime, double>>& data)
{
    m_currentSymbol = symbol;
    m_chartData = data;
    
    updateChart();
}

void StockChartWidget::updateChart()
{
    m_series->clear();
    
    // Add data points to series
    for (const auto& point : m_chartData) {
        m_series->append(point.first.toMSecsSinceEpoch(), point.second);
    }
    
    if (!m_chartData.empty()) {
        // Set axis ranges
        QDateTime minTime = m_chartData.front().first;
        QDateTime maxTime = m_chartData.back().first;
        double minPrice = m_chartData.front().second;
        double maxPrice = m_chartData.front().second;
        
        for (const auto& point : m_chartData) {
            minPrice = std::min(minPrice, point.second);
            maxPrice = std::max(maxPrice, point.second);
        }
        
        // Add some padding
        double priceRange = maxPrice - minPrice;
        minPrice -= priceRange * 0.05;
        maxPrice += priceRange * 0.05;
        
        m_xAxis->setMin(minTime);
        m_xAxis->setMax(maxTime);
        m_yAxis->setMin(minPrice);
        m_yAxis->setMax(maxPrice);
        
        // Update chart title
        m_chart->setTitle(QString("Stock Price Chart - %1").arg(QString::fromStdString(m_currentSymbol)));
    }
}

void StockChartWidget::clearChart()
{
    m_series->clear();
    m_chartData.clear();
    m_currentSymbol.clear();
}

void StockChartWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isSelecting = true;
        m_selectionStart = event->pos();
        m_selectionEnd = event->pos();
        
        // Start timer to detect when selection is complete
        m_selectionTimer->start(100);
    }
    
    QWidget::mousePressEvent(event);
}

void StockChartWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isSelecting) {
        m_selectionEnd = event->pos();
        // Update visual selection (could add a visual rectangle here)
    }
    
    QWidget::mouseMoveEvent(event);
}

void StockChartWidget::addDataPoint(const QDateTime& timestamp, double price)
{
    m_chartData.emplace_back(timestamp, price);
    updateChart();
}

void StockChartWidget::updateChartWithSelection(QPointF point)
{
    // This method can be used to highlight a selected point
    // Currently it's a placeholder
}