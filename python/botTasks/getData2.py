from pybit.unified_trading import HTTP
import time

session = HTTP(testnet=True)
response = session.get_tickers(
    category="inverse",
    symbol="BTCUSD",
)

# Извлекаем данные
symbol = response['result']['list'][0]['symbol']
last_price = response['result']['list'][0]['indexPrice']
timestamp = response['time']

# Конвертируем метку времени в читаемый формат
time_readable = time.strftime('%Y-%m-%d %H:%M:%S', time.gmtime(timestamp / 1000))

# Вывод
print(f"Валютная пара: {symbol}")
print(f"Цена: {last_price}")
print(f"Время: {time_readable}")