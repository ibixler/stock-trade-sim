#include "main_window.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFont>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Stock Trading Simulator");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("StockSim");
    
    // Set a modern style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Set default font
    QFont font = app.font();
    font.setPointSize(10);
    app.setFont(font);
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}