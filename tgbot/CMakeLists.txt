cmake_minimum_required(VERSION 3.10)
project(telegram_bot)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем путь к заголовочным файлам
include_directories(
    /usr/local/include
    ${CMAKE_SOURCE_DIR}/include  
)

# Указываем библиотеки, которые будут использоваться
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

# Основной исполняемы файл
add_executable(telegram_bot
    main.cpp
    database.cpp
)
# Исполняемый файл для тестирования
add_executable(telegram_bot_test
    gamebot_public_test.cpp
    database.cpp
)
# Исполняемый файл для просмотра базы данных
add_executable(telegram_bot_readBd
    readBd.cpp
)
# Линкуем необходимые библиотеки
target_link_libraries(telegram_bot
    TgBot
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
    SQLite::SQLite3
    pthread
)
# Линкуем необходимые библиотеки для telegram_bot_test
target_link_libraries(telegram_bot_test
    TgBot
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
    SQLite::SQLite3
    pthread
)
# Линкуем необходимые библиотеки для telegram_bot_readBd
target_link_libraries(telegram_bot_readBd
    SQLite::SQLite3
)