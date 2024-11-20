# Работает не быстро потребуется время чтобы он скачал все нужные страницы
import requests
from bs4 import BeautifulSoup
import csv

def get_animals_by_letter():
    base_url = "https://ru.wikipedia.org/wiki/Категория:Животные_по_алфавиту"
    animals_count = {chr(c): 0 for c in range(ord('А'), ord('Я') + 1)}
    
    while base_url:
        print(f"Читаем страницу: {base_url}")  # Отслеживание текущей страницы
        response = requests.get(base_url)
        if response.status_code != 200:
            print(f"Ошибка доступа к {base_url}: статус {response.status_code}")
            break
        
        soup = BeautifulSoup(response.text, 'html.parser')

        # Собираем записи животных
        entries = soup.select('.mw-category-group ul li a')
        for entry in entries:
            name = entry.text.strip()
            if name:  # Проверяем, что строка не пустая
                first_letter = name[0].upper()
                if first_letter in animals_count:
                    animals_count[first_letter] += 1

        # Ищем ссылку на следующую страницу
        next_page = soup.find('a', text='Следующая страница')
        base_url = "https://ru.wikipedia.org" + next_page['href'] if next_page else None

    return animals_count

def save_to_csv(data, filename):
    with open(filename, 'w', encoding='utf-8', newline='') as file:
        writer = csv.writer(file)
        for letter, count in sorted(data.items()):
            writer.writerow([letter, count])

if __name__ == "__main__":
    print("Сбор данных с Википедии...")
    animals_data = get_animals_by_letter()
    save_to_csv(animals_data, 'beasts.csv')
    print("Данные успешно сохранены в файл beasts.csv.")
