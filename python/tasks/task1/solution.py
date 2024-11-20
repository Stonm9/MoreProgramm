def strict(func):
    def wrapper(*args, **kwargs):
        # Получаем аннотации типов аргументов функции
        annotations = func.__annotations__
        
        # Проверяем переданные аргументы на соответствие аннотациям
        for (arg_value, (arg_name, arg_type)) in zip(args, annotations.items()):
            if arg_name == 'return':
                continue  # Пропускаем проверку возвращаемого значения
            if not isinstance(arg_value, arg_type):
                raise TypeError(
                    f"Аргумент '{arg_name}' должен быть типа {arg_type.__name__}, но получил {type(arg_value).__name__}"
                )
        # Вызываем исходную функцию с корректными аргументами
        return func(*args, **kwargs)
    return wrapper

@strict
def sum_two(a: int, b: int) -> int:
    return a + b

# Тесты
print(sum_two(1, 2))       # >>> 3
print(sum_two(1, "test"))     # >>> TypeError
