cmake_minimum_required(VERSION 3.16)
project(stock_trading_simulator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt modules
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts Sql Network)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/main_window.cpp
    src/stock_data.cpp
    src/database_manager.cpp
    src/portfolio_manager.cpp
    src/stock_chart_widget.cpp
    src/portfolio_widget.cpp
    src/watchlist_widget.cpp
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} 
    Qt5::Core 
    Qt5::Widgets 
    Qt5::Charts 
    Qt5::Sql
    Qt5::Network
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)