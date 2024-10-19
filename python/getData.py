import requests

url = 'https://api.bybit.com/v5/market/tickers?category=spot'
response = requests.get(url)

# Проверка статуса запроса
if response.status_code == 200:
    try:
        result = response.json()  # Попытка декодировать JSON
        print(result)
    except requests.exceptions.JSONDecodeError:
        print("Ошибка декодирования JSON. Ответ от сервера: ", response.text)
else:
    print(f"Ошибка запроса. Код статуса: {response.status_code}")
    print("Ответ сервера:", response.text)