# Stock Trading Simulator

A modern C++ stock trading simulator application built with Qt and SQLite, featuring live stock data simulation, portfolio management, and chart visualization.

## Features

- **Real-time Stock Data Simulation**: Simulated live stock price updates
- **Portfolio Management**: Track your stocks, purchases, and gains/losses
- **Watchlist**: Add and track favorite stocks
- **Interactive Charts**: Visualize stock price movements with point selection
- **SQLite Database**: Persistent storage for portfolio and watchlist data
- **Modern C++**: Uses smart pointers, RAII, and modern C++17 features

## Requirements

- C++17 compatible compiler (GCC 7+ or Clang 5+)
- Qt5 (Core, Widgets, Charts, Sql modules)
- CMake 3.16 or higher (for CMake build)
- Make (for Makefile build)

## Installation

### Using CMake (Recommended)

1. **Install dependencies**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install qt5-default qtdeclarative5-dev libqt5charts5-dev cmake
   
   # macOS with Homebrew
   brew install qt cmake
   
   # CentOS/RHEL/Fedora
   sudo dnf install qt5-qtbase qt5-qtcharts cmake
   ```

2. **Build the project**:
   ```bash
   cd stock_trading_simulator
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the application**:
   ```bash
   ./bin/stock_trading_simulator
   ```

### Using Makefile

1. **Install dependencies**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install qt5-default qtdeclarative5-dev libqt5charts5-dev pkg-config
   
   # macOS with Homebrew
   brew install qt pkg-config
   
   # CentOS/RHEL/Fedora
   sudo dnf install qt5-qtbase qt5-qtcharts pkgconfig
   ```

2. **Build the project**:
   ```bash
   cd stock_trading_simulator
   make
   ```

3. **Run the application**:
   ```bash
   ./bin/stock_trading_simulator
   ```

## Project Structure

```
stock_trading_simulator/
├── src/                 # Source files
│   ├── main.cpp         # Main application entry point
│   ├── main_window.cpp  # Main window implementation
│   ├── stock_data.cpp   # Stock data management
│   ├── database_manager.cpp  # Database operations
│   ├── portfolio_manager.cpp # Portfolio management
│   ├── stock_chart_widget.cpp # Chart visualization
│   ├── portfolio_widget.cpp  # Portfolio display
│   └── watchlist_widget.cpp  # Watchlist display
├── include/             # Header files
│   ├── main_window.h
│   ├── stock_data.h
│   ├── database_manager.h
│   ├── portfolio_manager.h
│   ├── stock_chart_widget.h
│   ├── portfolio_widget.h
│   └── watchlist_widget.h
├── CMakeLists.txt       # CMake build configuration
├── Makefile             # Makefile build configuration
└── README.md            # This file
```

## Usage

### Main Interface

The application features three main tabs:

1. **Market Search**: Browse stocks and search for specific ones
2. **My Portfolio**: View your current holdings and performance
3. **Watchlist**: Track stocks you're interested in

### Features

- **Buying Stocks**: Select a stock from the market search tab and click "Buy Stock"
- **Portfolio Tracking**: View current holdings with profit/loss calculations
- **Watchlist Management**: Add/remove stocks from your watchlist
- **Interactive Charts**: Click on chart points to see detailed information
- **Live Updates**: Stock prices update automatically every minute

### Database

The application uses SQLite to store:
- Stock information (symbol, name, price)
- Portfolio holdings (stock ID, quantity, purchase price, date)
- Watchlist entries

Database file is created in the user's Documents folder with the name `stock_trading.db`.

## Implementation Details

### Modern C++ Features Used

- **Smart Pointers**: `std::unique_ptr` and `std::shared_ptr` for automatic memory management
- **RAII**: Resource Acquisition Is Initialization for safe resource handling
- **Move Semantics**: Efficient object transfers using move constructors
- **Modern STL**: `std::vector`, `std::tuple`, and other modern containers
- **Qt Smart Pointers**: Qt's `QScopedPointer` and `QSharedPointer`

### Architecture

The application follows a layered architecture:
1. **Presentation Layer**: UI components (MainWindow, PortfolioWidget, WatchlistWidget)
2. **Business Logic Layer**: PortfolioManager, DatabaseManager
3. **Data Layer**: StockData, Database operations

### Threading

While not strictly necessary for this simulator, the application is designed to support:
- Background data fetching
- Database operations on separate threads
- Live price update mechanisms

## Future Enhancements

- Integration with real stock APIs (Alpha Vantage, Yahoo Finance, etc.)
- Advanced charting with multiple timeframes
- Portfolio performance analytics
- Backtesting features
- User authentication and multiple user support
- Export functionality for portfolio data
- More sophisticated trading algorithms

## License

This project is available under the MIT License.