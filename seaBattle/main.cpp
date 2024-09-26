// Copyright (c) 2024 Petrov Denis Nikolaevich
// Морской бой. Она из первых игр которую написал еще под windows в стадии обучения C++, без применения ООП.
#ifdef _WIN32
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;
//---------------Дизайн игры---------------------
const char dFrame = '*' /*'*'*/; // рамка
const char dSpace = ' ' /*' '*/; // пустота
const char dShip = '+' /*'+'*/; // корабль
const char dMiss = '-' /*'-'*/; // промах
const char dHit = 'X' /*'X'*/; // попадание

#define colors  case dHit: \
                     SetColor(4, 0); \
                break; \
                case dFrame: \
                     SetColor(3, 0); \
                break; \
                case dShip: \
                     SetColor(6, 0); \
                break; \
                default: \
                     SetColor(7, 0); \
                break; 
//---------------------------------------------

#define hit(XX) map[masShip[XX][0]][masShip[XX][1]] == dHit /*'X'*/



struct cord {
    int index = -1;
    bool status = true;
    int lines = 0;
    int colums = 0;
};
struct hardAIhit {
    int lines = 0;
    int colums = 0;
    bool statusWound = false; //Статус попадания
    int firstLines = 0;
    int firstColums = 0;
    int pass = 1;
    int randNum = 0;
    cord up;
    cord down;
    cord left;
    cord right;
};
struct settings {
    bool _menVScomp; //Выбор режима человек - комьютер
    bool _shipHend; //Выбор режима ручная или автоматическа расстановка
    bool _compAIsimple; //Сложность комьютера
    bool _setHendYes; //Принимает значение true кода все корабали в ручном режиме расставлены
};
void SetColor(int text, int bg) {
    //Функция для изменения цвета текста в консоле
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}
void setcur(int x, int y)
{
    // функция для плавной перерисовки экрана
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};
bool generateAutoArrangement(char map[][14], int mapSize, int ship, int masShip[][2], int* count) {
    //Функция для заполнения массива автораставленными кораблями
    int lines = 0; // строки
    int columns = 0; //столбцы
    int vert = 0; //Положение коробля 0 горизонтальный 1 вертикальный
    lines = rand() % 10 + 3;
    columns = rand() % 10 + 3;
    vert = rand() % 2;
    //vert = 0;
    if (vert == 0) {
        for (int i = lines - 1; i <= lines + 1; i++)
        {
            for (int j = columns - 1; j <= columns + ship; j++)
            {
                if (map[i][j] == dShip /*'+'*/ || map[lines][j + 1] == dFrame /*'*'*/) {
                    return 1;
                }
            }
        }

        for (int i = columns; i < columns + ship; i++)
        {
            map[lines][i] = dShip /*'+'*/;
            masShip[*count][0] = lines;
            masShip[*count][1] = i;
            *count = *count + 1;
        }
        return 0;
    }
    else
    {

        for (int i = lines - 1; i <= lines + ship; i++)
        {
            for (int j = columns - 1; j <= columns + 1; j++)
            {
                if (map[i][j] == dShip /*'+'*/ || map[i + 1][columns] == dFrame /*'*'*/) {

                    return 1;
                }
            }
        }

        for (int i = lines; i < lines + ship; i++)
        {
            map[i][columns] = dShip /*'+'*/;
            masShip[*count][0] = i;
            masShip[*count][1] = columns;
            *count = *count + 1;
        }

        return 0;
    }

}
void ships(char map[][14], int mapSize, int masShip[][2]) {
    int count = 0;
    do {//Генерируем 4x палубный корабль
    } while (generateAutoArrangement(map, mapSize, 4, masShip, &count));

    for (int i = 0; i < 2; i++)//Генерируем 3x палубные корабли
    {
        do {
        } while (generateAutoArrangement(map, mapSize, 3, masShip, &count));
    }
    for (int i = 0; i < 3; i++)//Генерируем 2x палубные корабли
    {
        do {
        } while (generateAutoArrangement(map, mapSize, 2, masShip, &count));
    }
    for (int i = 0; i < 4; i++)//Генерируем 1x палубные корабли
    {
        do {
        } while (generateAutoArrangement(map, mapSize, 1, masShip, &count));
    }
}
void generateEmptyMaps(char mapUser[][14], char mapComputer[][14], char mapComputerSpace[][14], int mapSize) {
    //Генерируеться пустой шаблон карты
    char sumbolPusto[14]{ "             " };
    char sumbol[14]{ ' ',' ',' ', 'A','B','C','D','I','J','K','L','M','O',' ' };
    //char topAndbottom[14]{ "  ***********" };
    char topAndbottom[14]{ ' ',' ', dFrame, dFrame , dFrame , dFrame , dFrame , dFrame , dFrame , dFrame , dFrame , dFrame , dFrame };
    char numbers[15]{ "   1234567890 " };
    char numbers2[15]{ "            1 " };


    for (int i = 0; i < mapSize; i++) //Строки
    {
        for (int j = 0; j < mapSize; j++) // Столбцы
        {
            mapUser[i][j] = dSpace;
        }
    }
    //Заполнение массива
    for (int i = 0; i < mapSize; i++) //Строки
    {
        for (int j = 0; j < mapSize; j++) // Столбцы
        {
            if (i == 0) //Заполняем первую строку
                mapUser[i][j] = sumbolPusto[j];
            if (i == 1) //Заполняем первую строку
                mapUser[i][j] = sumbol[j];

            if (i == 2 || i == mapSize - 1)
                mapUser[i][j] = topAndbottom[j]; // Рисуем вторую и нижнию строку

            if (j == 0)
                mapUser[i][j] = numbers2[i]; //Рисуем вертикальные цыфры
            if (j == 1)
                mapUser[i][j] = numbers[i]; //Рисуем вертикальные цыфры

            if ((j == 2 || j == mapSize - 1) && i > 1 && i < mapSize) //Рисуем вертикальные линии
                mapUser[i][j] = dFrame;

            mapComputer[i][j] = mapUser[i][j];
            mapComputerSpace[i][j] = mapUser[i][j];
        }
    }
}
void hitControlFilling(char map[][14], int masShip[][2], int shipCount, int firstIndex) {
    //Заполение пространства вокруг убитого корабля
    if (masShip[firstIndex][0] == masShip[firstIndex + 1][0]) {
        for (int i = masShip[firstIndex][0] - 1; i <= masShip[firstIndex][0] + 1; i++)
        {
            for (int j = masShip[firstIndex][1] - 1; j <= masShip[firstIndex][1] + shipCount; j++)
            {
                if (map[i][j] == dFrame /*'*'*/ || map[i][j] == dHit /*'X'*/) {
                }
                else {
                    map[i][j] = dMiss /*'-'*/;
                }
            }
        }
    }
    else
    {
        for (int i = masShip[firstIndex][0] - 1; i <= masShip[firstIndex][0] + shipCount; i++)
        {
            for (int j = masShip[firstIndex][1] - 1; j <= masShip[firstIndex][1] + 1; j++)
            {
                if (map[i][j] == dFrame /*'*'*/ || map[i][j] == dHit /*'X'*/) {

                }
                else {
                    map[i][j] = dMiss /*'-'*/;
                }
            }
        }
    }
}
bool handShip(char map[][14], int masShip[][2], int shipCount, int posY, int posX, int curcorPozish, int* countForMasShip) {
    //Контроль возможности поставить корабль
    bool setYes = false;
    short count = 0;
    if (curcorPozish == true) { //Горизонтальная установка
        for (int i = 0; i < shipCount; i++) // 1 . Проверка на возможность установки корабля
        {
            if (map[posY][posX + i] != ' ') {
                count++;
            }
        }
        if (count == 0) {   //2.Проверка на возможность установки корабля
            for (int i = posX - 1; i <= posX + shipCount; i++)
            {
                for (int j = posY - 1; j <= posY + 1; j++)
                {
                    if (map[j][i] == dShip /*'+'*/)
                        count++;
                }
            }
        }

        if (count == 0) {// Устанавливаем корабыль
            for (int i = 0; i < shipCount; i++)
            {
                map[posY][posX + i] = dShip /*'+'*/;

                masShip[*countForMasShip][0] = posY; //lines
                masShip[*countForMasShip][1] = posX + i; //collum
                *countForMasShip = *countForMasShip + 1;
            }
            return true;
        }
        else {
            return false;
        }
    }
    else { // Вертикальная установка
        for (int i = 0; i < shipCount; i++) // 1 . Проверка на возможность установки корабля
        {
            if (map[posY + i][posX] != ' ') {
                count++;
            }
        }
        if (count == 0) {   //2.Проверка на возможность установки корабля
            for (int i = posX - 1; i <= posX + 1; i++)
            {
                for (int j = posY - 1; j <= posY + shipCount; j++)
                {
                    if (map[j][i] == dShip /*'+'*/)
                        count++;
                }
            }
        }
        if (count == 0) { // Устанавливаем корабыль
            for (int i = 0; i < shipCount; i++)
            {
                map[posY + i][posX] = dShip /*'+'*/;
                masShip[*countForMasShip][0] = posY + i;
                masShip[*countForMasShip][1] = posX;
                *countForMasShip = *countForMasShip + 1;
            }
            return true;
        }
        else {
            return false;
        }
    }

}
void hitControl(char map[][14], int masShip[][2]) {
    //Контроль попадания в корабли
    if (hit(0) && hit(1) && hit(2) && hit(3))
        hitControlFilling(map, masShip, 4, 0);
    if (hit(4) && hit(5) && hit(6))
        hitControlFilling(map, masShip, 3, 4);
    if (hit(7) && hit(8) && hit(9))
        hitControlFilling(map, masShip, 3, 7);
    if (hit(10) && hit(11))
        hitControlFilling(map, masShip, 2, 10);
    if (hit(12) && hit(13))
        hitControlFilling(map, masShip, 2, 12);
    if (hit(14) && hit(15))
        hitControlFilling(map, masShip, 2, 14);
    if (hit(16))
        hitControlFilling(map, masShip, 1, 16);
    if (hit(17))
        hitControlFilling(map, masShip, 1, 17);
    if (hit(18))
        hitControlFilling(map, masShip, 1, 18);
    if (hit(19))
        hitControlFilling(map, masShip, 1, 19);
}
bool simpleAI(char mapUser[][14], int masShipUser[][2], int& pointComputer) {
    //Простой ИИ компьютера
    int randNumberComputerlines = rand() % 10 + 3;
    int randNumberComputerColumns = rand() % 10 + 3;
    if (mapUser[randNumberComputerlines][randNumberComputerColumns] == dMiss /*'-'*/ ||
        mapUser[randNumberComputerlines][randNumberComputerColumns] == dHit /*'X'*/) {
        return 1;
    }

    if (mapUser[randNumberComputerlines][randNumberComputerColumns] == dShip /*'+'*/) {
        mapUser[randNumberComputerlines][randNumberComputerColumns] = dHit /*'X'*/;
        pointComputer++;
        if (pointComputer == 20) return 0;
        return 1;
    }
    else {
        mapUser[randNumberComputerlines][randNumberComputerColumns] = dMiss /*'-'*/;
        return 0;
    }

}
bool averageAI(char mapUser[][14], int masShipUser[][2], int& pointComputer, hardAIhit& HAiHit) {
    //Средний ИИ компьютера
    int randNumberComputerlines = 0;
    int randNumberComputerColumns = 0;
    if (HAiHit.statusWound == true) {
        //Проверка направления для удара
        if (mapUser[HAiHit.lines + 1][HAiHit.colums] == dFrame /*'*'*/ || mapUser[HAiHit.lines + 1][HAiHit.colums] == dMiss /*'-'*/ || mapUser[HAiHit.lines + 1][HAiHit.colums] == dHit /*'X'*/) HAiHit.down.status = false;
        if (mapUser[HAiHit.lines - 1][HAiHit.colums] == dFrame /*'*'*/ || mapUser[HAiHit.lines - 1][HAiHit.colums] == dMiss /*'-'*/ || mapUser[HAiHit.lines - 1][HAiHit.colums] == dHit /*'X'*/) HAiHit.up.status = false;
        if (mapUser[HAiHit.lines][HAiHit.colums + 1] == dFrame /*'*'*/ || mapUser[HAiHit.lines][HAiHit.colums + 1] == dMiss /*'-'*/ || mapUser[HAiHit.lines][HAiHit.colums + 1] == dHit /*'X'*/) HAiHit.right.status = false;
        if (mapUser[HAiHit.lines][HAiHit.colums - 1] == dFrame /*'*'*/ || mapUser[HAiHit.lines][HAiHit.colums - 1] == dMiss /*'-'*/ || mapUser[HAiHit.lines][HAiHit.colums - 1] == dHit /*'X'*/) HAiHit.left.status = false;

        if (HAiHit.left.status == 0 && HAiHit.right.status == 0 && HAiHit.down.status == 0 && HAiHit.up.status == 0) { //Если все направления для стрельбы закончились
            randNumberComputerlines = rand() % 10 + 3;
            randNumberComputerColumns = rand() % 10 + 3;

            HAiHit.statusWound = false;
            HAiHit.down.status = true;
            HAiHit.up.status = true;
            HAiHit.right.status = true;
            HAiHit.left.status = true;
        }
        else
        {
            if (HAiHit.down.status == true) {
                randNumberComputerlines = HAiHit.lines + 1;
                randNumberComputerColumns = HAiHit.colums;
            }
            else if (HAiHit.up.status == true) {
                randNumberComputerlines = HAiHit.lines - 1;
                randNumberComputerColumns = HAiHit.colums;
            }
            else if (HAiHit.right.status == true) {
                randNumberComputerColumns = HAiHit.colums + 1;
                randNumberComputerlines = HAiHit.lines;
            }
            else if (HAiHit.left.status == true) {
                randNumberComputerColumns = HAiHit.colums - 1;
                randNumberComputerlines = HAiHit.lines;
            }
        }
    }
    else
    {
        randNumberComputerlines = rand() % 10 + 3;
        randNumberComputerColumns = rand() % 10 + 3;
    }
    if (mapUser[randNumberComputerlines][randNumberComputerColumns] == dMiss /*'-'*/ ||
        mapUser[randNumberComputerlines][randNumberComputerColumns] == dHit /*'X'*/) {
        return 1;
    }

    if (mapUser[randNumberComputerlines][randNumberComputerColumns] == dShip /*'+'*/) {
        mapUser[randNumberComputerlines][randNumberComputerColumns] = dHit /*'X'*/;

        hitControl(mapUser, masShipUser);
        HAiHit.lines = randNumberComputerlines;
        HAiHit.colums = randNumberComputerColumns;
        HAiHit.statusWound = true; // Статус ранен

        pointComputer++;
        if (pointComputer == 20) return 0;
        return 1;
    }
    else {
        mapUser[randNumberComputerlines][randNumberComputerColumns] = dMiss /*'-'*/;
        return 0;
    }
}
bool hardAI(char mapUser[][14], int masShipUser[][2], int& pointComputer, hardAIhit& HAiHit) {
    //Сложный ИИ компьютера
    bool randStatus = true;
    if (HAiHit.statusWound == true) {


        if (mapUser[HAiHit.firstLines - 1][HAiHit.firstColums] == dMiss /*'-'*/ || mapUser[HAiHit.firstLines - 1][HAiHit.firstColums] == dFrame /*'*'*/) HAiHit.up.status = false;
        if (mapUser[HAiHit.firstLines + 1][HAiHit.firstColums] == dMiss /*'-'*/ || mapUser[HAiHit.firstLines + 1][HAiHit.firstColums] == dFrame /*'*'*/) HAiHit.down.status = false;
        if (mapUser[HAiHit.firstLines][HAiHit.firstColums - 1] == dMiss /*'-'*/ || mapUser[HAiHit.firstLines][HAiHit.firstColums - 1] == dFrame /*'*'*/) HAiHit.left.status = false;
        if (mapUser[HAiHit.firstLines][HAiHit.firstColums + 1] == dMiss /*'-'*/ || mapUser[HAiHit.firstLines][HAiHit.firstColums + 1] == dFrame /*'*'*/) HAiHit.right.status = false;

        //Выесняем сколько направлений открыто и сколько надо сгенерировать случайных чисел
        if (HAiHit.up.status == true) {
            HAiHit.up.lines = HAiHit.firstLines;
            HAiHit.up.colums = HAiHit.firstColums;
            HAiHit.up.index = 1; // Присваемаем индекс направления
        }
        if (HAiHit.down.status == true) {
            HAiHit.down.lines = HAiHit.firstLines;
            HAiHit.down.colums = HAiHit.firstColums;
            HAiHit.down.index = 2;
        }
        if (HAiHit.left.status == true) {
            HAiHit.left.lines = HAiHit.firstLines;
            HAiHit.left.colums = HAiHit.firstColums;
            HAiHit.left.index = 3;
        }
        if (HAiHit.right.status == true) {
            HAiHit.right.lines = HAiHit.firstLines;
            HAiHit.right.colums = HAiHit.firstColums;
            HAiHit.right.index = 4;
        }
        if (HAiHit.pass < 2) {
            do {
                HAiHit.randNum = rand() % 4 + 1;
                if (HAiHit.up.status == false && HAiHit.down.status == false && HAiHit.left.status == false && HAiHit.right.status == false) {
                    break;
                }
                else {
                    randStatus = (HAiHit.up.status == true && HAiHit.up.index == HAiHit.randNum) ||
                        (HAiHit.down.status == true && HAiHit.down.index == HAiHit.randNum) ||
                        (HAiHit.left.status == true && HAiHit.left.index == HAiHit.randNum) ||
                        (HAiHit.right.status == true && HAiHit.right.index == HAiHit.randNum);
                }
            } while (!randStatus);
        }

        if (HAiHit.up.status == false && HAiHit.down.status == false && HAiHit.left.status == false && HAiHit.right.status == false) {
            //Если все направления для стрельбы закрыты
            HAiHit.up.index = -1; HAiHit.down.index = -1; HAiHit.left.index = -1; HAiHit.right.index = -1;
            HAiHit.statusWound = false;
            HAiHit.up.status = true;
            HAiHit.down.status = true;
            HAiHit.left.status = true;
            HAiHit.right.status = true;
            HAiHit.pass = 1;
            return 1;
        }
        else {


            //--------------------------------------------------------------
            if (HAiHit.up.status == true && HAiHit.up.index == HAiHit.randNum) {

                if (mapUser[HAiHit.up.lines - HAiHit.pass][HAiHit.up.colums] == dShip /*'+'*/) {
                    mapUser[HAiHit.up.lines - HAiHit.pass][HAiHit.up.colums] = dHit /*'X'*/;
                    hitControl(mapUser, masShipUser);
                    pointComputer++;
                    HAiHit.pass++;
                    HAiHit.left.status = false;
                    HAiHit.right.status = false;
                    return 1;
                }
                else {
                    if (mapUser[HAiHit.up.lines - HAiHit.pass][HAiHit.up.colums] == dFrame /*'*'*/) {
                        HAiHit.up.status = false;
                        HAiHit.pass = 1;
                        return 1;
                    }
                    mapUser[HAiHit.up.lines - HAiHit.pass][HAiHit.up.colums] = dMiss /*'-'*/;
                    HAiHit.pass = 1;
                    HAiHit.up.status = false;
                    return 0;
                }
            }
            //--------------------------------------------------------------
            if (HAiHit.down.status == true && HAiHit.down.index == HAiHit.randNum) {

                if (mapUser[HAiHit.down.lines + HAiHit.pass][HAiHit.down.colums] == dShip /*'+'*/) {
                    mapUser[HAiHit.down.lines + HAiHit.pass][HAiHit.down.colums] = dHit /*'X'*/;
                    pointComputer++;
                    hitControl(mapUser, masShipUser);
                    HAiHit.pass++;
                    HAiHit.left.status = false;
                    HAiHit.right.status = false;
                    return 1;
                }
                else {
                    if (mapUser[HAiHit.down.lines + HAiHit.pass][HAiHit.down.colums] == dFrame /*'*'*/) {
                        HAiHit.down.status = false;
                        HAiHit.pass = 1;
                        return 1;
                    }
                    mapUser[HAiHit.down.lines + HAiHit.pass][HAiHit.down.colums] = dMiss /*'-'*/;
                    HAiHit.pass = 1;
                    HAiHit.down.status = false;
                    return 0;
                }
            }
            //--------------------------------------------------------------
            if (HAiHit.left.status == true && HAiHit.left.index == HAiHit.randNum) {

                if (mapUser[HAiHit.left.lines][HAiHit.left.colums - HAiHit.pass] == dShip /*'+'*/) {
                    mapUser[HAiHit.left.lines][HAiHit.left.colums - HAiHit.pass] = dHit /*'X'*/;
                    hitControl(mapUser, masShipUser);
                    pointComputer++;
                    HAiHit.pass++;
                    HAiHit.up.status = false;
                    HAiHit.down.status = false;
                    return 1;
                }
                else {
                    if (mapUser[HAiHit.left.lines][HAiHit.left.colums - HAiHit.pass] == dFrame /*'*'*/) {
                        HAiHit.left.status = false;
                        HAiHit.pass = 1;
                        return 1;
                    }
                    mapUser[HAiHit.left.lines][HAiHit.left.colums - HAiHit.pass] = dMiss /*'-'*/;
                    HAiHit.pass = 1;
                    HAiHit.left.status = false;
                    return 0;
                }
            }
            //--------------------------------------------------------------
            if (HAiHit.right.status == true && HAiHit.right.index == HAiHit.randNum) {

                if (mapUser[HAiHit.right.lines][HAiHit.right.colums + HAiHit.pass] == dShip /*'+'*/) {
                    mapUser[HAiHit.right.lines][HAiHit.right.colums + HAiHit.pass] = dHit /*'X'*/;
                    hitControl(mapUser, masShipUser);
                    pointComputer++;
                    HAiHit.pass++;
                    HAiHit.up.status = false;
                    HAiHit.down.status = false;
                    return 1;
                }
                else {
                    if (mapUser[HAiHit.right.lines][HAiHit.right.colums + HAiHit.pass] == dFrame /*'*'*/) {
                        HAiHit.right.status = false;
                        HAiHit.pass = 1;
                        return 1;
                    }
                    mapUser[HAiHit.right.lines][HAiHit.right.colums + HAiHit.pass] = dMiss /*'-'*/;
                    HAiHit.pass = 1;
                    HAiHit.right.status = false;
                    return 0;
                }
            }
        }
        return 0;
    }
    else {
        int randNumberComputerlines = rand() % 10 + 3;
        int randNumberComputerColumns = rand() % 10 + 3;
        if (mapUser[randNumberComputerlines][randNumberComputerColumns] == dMiss /*'-'*/ ||
            mapUser[randNumberComputerlines][randNumberComputerColumns] == dHit /*'X'*/) {
            return 1;
        }

        if (mapUser[randNumberComputerlines][randNumberComputerColumns] == dShip /*'+'*/) {
            mapUser[randNumberComputerlines][randNumberComputerColumns] = dHit /*'X'*/;
            hitControl(mapUser, masShipUser);
            HAiHit.firstLines = randNumberComputerlines;
            HAiHit.firstColums = randNumberComputerColumns;
            HAiHit.statusWound = true;
            pointComputer++;
            if (pointComputer == 20) return 0;
            return 1;
        }
        else {
            mapUser[randNumberComputerlines][randNumberComputerColumns] = dMiss /*'-'*/;
            return 0;
        }
    }
}
void logo() {
    // Логотип игры
    cout << endl;
    SetColor(3, 0);
    char text[10][116] = {{"      *     *                                                                        ******                        "},
                          {"     ***   ***                                                           **         **                       **    "},
                          {"     ** * * **    ******     ******     *****    **   **    ******    **    **      **          ******    **    ** "},
                          {"     **  *  **   **////**   **////**   **    *   **  **    **////**   **  ****      *******    **////**   **  **** "},
                          {"     **     **   **  //**   **  //**   **        ****      **  //**   ** ** **      **  //**   **  //**   ** ** ** "},
                          {"     **     **   **  //**   **  //**   **    *   **  **    **  //**   ****  **      **  //**   **  //**   ****  ** "},
                          {"     **     **    ******    *******     *****    **   **    ******    **    **      *******     ******    **    ** "},
                          {"                            **                                                                                     "},
                          {"                           **                                                         Работу выполнил: Петров Д.Н. "},
                          {"                          **                                                                                       "}, };

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 116; j++)
        {
            cout << text[i][j];
        }
        cout << endl;
    }
    cout << endl << endl << endl;
    SetColor(7, 0);
}
void menu(settings* setting, bool* startGame) {
    char key = ' ';
    logo();
    cout << "                                                 Выберите режим игры:" << endl;
    cout << "                                               <---------------------->" << endl;
    cout << "                                                1. Человек - компьютер" << endl;
    cout << "                                                2. Компьютер - компьютер" << endl;
    cout << "                                                0. Для выхода" << endl;

    do {
        key = _getch();
        if (key == '1')
            setting->_menVScomp = true;
        if (key == '2')
            setting->_menVScomp = false;
        if (key == '0') {
            *startGame = false;
            return;
        }
    } while (key != '1' && key != '2' && key != '0');

    //system("cls");
    setcur(0, 0);
    logo();
    if (setting->_menVScomp != false) {
        cout << "                                                 Расстановка кораблей:          " << endl;
        cout << "                                               <---------------------->            " << endl;
        cout << "                                                1. Расстановка вручную           " << endl;
        cout << "                                       2. Комьютер расставит корабли автоматически" << endl;
        cout << "                                                                            " << endl;
        do {
            key = _getch();
            if (key == '1')
                setting->_shipHend = true;
            if (key == '2')
                setting->_shipHend = false;
        } while (key != '1' && key != '2');
    }
    setting->_setHendYes = false;

    setcur(0, 0);
    logo();
    cout << "                                               Сложность игры комьютера:" << endl;
    cout << "                                               <---------------------->" << endl;
    cout << "                                                 1. Легкий компьютер                    " << endl;
    cout << "                                                 2. Сложный компьютер                   " << endl;
    cout << "                                                                                        " << endl;
    do {
        key = _getch();
        if (key == '1')
            setting->_compAIsimple = true;
        if (key == '2')
            setting->_compAIsimple = false;
    } while (key != '1' && key != '2');
    system("cls");
}
int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    bool startGame = true;

    while (startGame) { // Главный игровой цикл
        //-------------------------------------------------------
        //---Этот код нужен чтобы убрать мерцание курсора
        void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO structCursorInfo;
        GetConsoleCursorInfo(handle, &structCursorInfo);
        structCursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(handle, &structCursorInfo);
        //--------------------------------------------------------
        int masShipComputer[20][2]; // Для хранения координат кораблей Комьютера
        int masShipUser[20][2]; // Для хранения координат кораблей Игрока

        char key = ' ';
        string messege = "", messege2 = messege = "                                                                                 ";
        bool cycle = true; // Состояние игровых циклов
        const int mapSize = 14;
        int pointsUser = 0, pointsComputer = 0; // Суммарное количество попаданий

        char mapUser[mapSize][mapSize]{};
        char mapComputer[mapSize][mapSize]{};
        char mapComputerSpace[mapSize][mapSize]{};

        hardAIhit HAiHit; // Хранит переменные для умного ИИ
        hardAIhit HAiHit2; // Хранит переменные для умного ИИ 2
        generateEmptyMaps(mapUser, mapComputer, mapComputerSpace, mapSize);


        int posX = 3, posY = 3;
        int startPosX = 3, startPosY = 3;

        int endPosX = mapSize - startPosX;
        int endPosY = mapSize - startPosY;
        settings setting;
        menu(&setting, &startGame);
        if (startGame == false)
            break;
        //Для отладки - ручной выбор пунктов меню
        /*setting._menVScomp = false;
        setting._shipHend = false;
        setting._compAIsimple = false;
        setting._setHendYes = false;*/


        if (setting._menVScomp == true) { // Режим Человек-Комьютер
            if (setting._shipHend == true) { // Ручная расстановка кораблей

                bool curcorPozish = true; // Горизонтальная или вертикальная позиция
                short ships[10]{ 4,3,3,2,2,2,1,1,1,1 }; //Палубы корабля для расстановки
                short countShips = 0;
                int countForMasShip = 0;
                do // Главный игровой цикл
                {

                    char cursor = '>';
                    if (key == 'd' || key == 'D' || key == 'ў' || key == '‚') {
                        messege = messege2;
                        if (posX <= endPosX) posX++;
                    }
                    if (key == 'a' || key == 'A' || key == 'д' || key == '”') {
                        messege = messege2;
                        if (posX > startPosX) posX--;
                    }
                    if (key == 'w' || key == 'W' || key == 'ж' || key == '–') {
                        messege = messege2;
                        if (posY > startPosY) posY--;
                    }
                    if (key == 's' || key == 'S' || key == 'л' || key == '›') {
                        messege = messege2;
                        if (posY <= endPosY) posY++;
                    }
                    if (key == 'q' || key == 'Q' || key == '©' || key == '‰') {
                        messege = messege2;
                        if (curcorPozish == true)
                            curcorPozish = false;
                        else
                            curcorPozish = true;
                    }
                    if (key == '0') {
                        break;
                    }
                    //system("cls");
                    setcur(0, 0);
                    //setlocale(LC_ALL, "Russian");
                    //system("color 07");
                    SetColor(7, 0);
                    cout << "\tДля управления используйте кнопки "; SetColor(6, 0); cout << "'w' 'a' 's' 'd'"; SetColor(7, 0);
                    cout << " и кнопку "; SetColor(6, 0); cout << "'e'"; SetColor(7, 0); cout << " чтобы поставить корабль. Выход в меню "; SetColor(6, 0); cout << "'0'" << endl;
                    SetColor(7, 0);
                    cout << "\t\t\t\t\t\t  Повернуть корабль"; SetColor(6, 0); cout << "'q'\n" << endl;
                    SetColor(7, 0);
                    cout << "\t\t\t\t      Найдите место для установки "; SetColor(6, 0); cout << ships[countShips]; SetColor(7, 0); cout << " палубного корабля\n" << endl;
                    cout << "\t\t\t\t\t\t        Ваше поле" << endl;
                    //setlocale(LC_ALL, "C"); // Служит для использования extended ascii table c++


                    for (int i = 0; i < mapSize; i++) // Рисуем поле
                    {
                        cout << "                                             ";
                        for (int j = 0; j < mapSize; j++)
                        {
                            if (i == posY && j == posX) {
                                if (curcorPozish == true) { //Горизонтальная установка
                                    SetColor(6, 0);
                                    cout << "> ";
                                    if (key == 'e' || key == 'E' || key == 'г' || key == '“') {
                                        if (true == handShip(mapUser, masShipUser, ships[countShips], posY, posX, curcorPozish, &countForMasShip)) {
                                            countShips++;

                                            if (countShips >= 10)
                                                cycle = false;
                                        }
                                        else {

                                            messege = "                                       Не подходящее место для установки корабля!";

                                        }
                                    }
                                }
                                else { //Вертикальная установка
                                    SetColor(6, 0);
                                    cout << "v ";
                                    if (key == 'e' || key == 'E' || key == 'г' || key == '“') {
                                        if (true == handShip(mapUser, masShipUser, ships[countShips], posY, posX, curcorPozish, &countForMasShip)) {
                                            countShips++;

                                            if (countShips >= 10)
                                                cycle = false;
                                        }
                                        else {

                                            messege = "                                       Не подходящее место для установки корабля!";

                                        }
                                    }
                                }

                            }
                            else {
                                //Раскрашиваем поле
                                switch (mapUser[i][j]) {
                                    colors
                                }
                                cout << mapUser[i][j] << ' ';
                            }
                        }
                        //Для отладки
                        /*cout << "   ";
                        for (int j = 0; j < mapSize; j++)
                        {
                            cout << mapUser[i][j] << ' ';
                        }*/
                        cout << endl;
                    }
                    SetColor(4, 0);
                    cout << messege << endl;
                    SetColor(7, 0);
                    key = _getch();// Слушаем нажатие клавиши

                } while (cycle);
                setting._shipHend = false;
                setting._setHendYes = true;
                cycle = true;
                system("cls");
            }
            if (setting._shipHend == false) {
                //--------------------------------------------------------------------
                ships(mapComputer, mapSize, masShipComputer);
                if (setting._setHendYes == false) {
                    ships(mapUser, mapSize, masShipUser);
                }
                //--------------------------------------------------------------------
                do // Главный игровой цикл
                {
                    switch (key) {
                    case 'd': if (posX <= endPosX) posX++; break;
                    case 'D': if (posX <= endPosX) posX++; break;
                    case 'ў': if (posX <= endPosX) posX++; break;
                    case '‚': if (posX <= endPosX) posX++; break;

                    case 'a': if (posX > startPosX) posX--; break;
                    case 'A': if (posX > startPosX) posX--; break;
                    case 'д': if (posX > startPosX) posX--; break;
                    case '”': if (posX > startPosX) posX--; break;

                    case 'w': if (posY > startPosY) posY--; break;
                    case 'W': if (posY > startPosY) posY--; break;
                    case 'ж': if (posY > startPosY) posY--; break;
                    case '–': if (posY > startPosY) posY--; break;

                    case 's': if (posY <= endPosY) posY++; break;
                    case 'S': if (posY <= endPosY) posY++; break;
                    case 'л': if (posY <= endPosY) posY++; break;
                    case '›': if (posY <= endPosY) posY++; break;

                    default:
                        break;
                    }
                    if (key == '0') {
                        break;
                    }

                    //if (key == 'q') { // Для отладки
                    //    pointsUser = 19;
                    //    //do {} while (hardAI(mapUser, masShipUser, pointsComputer));
                    //}

                    //system("cls");
                    setcur(0, 0);
                    //setlocale(LC_ALL, "Russian");
                    //system("color 07");
                    SetColor(7, 0);
                    cout << "\t   Для управления используйте кнопки "; SetColor(6, 0); cout << "'w' 'a' 's' 'd'"; SetColor(7, 0);
                    cout << " и кнопку "; SetColor(6, 0); cout << "'e'"; SetColor(7, 0); cout << " для выстрела. Выход в меню "; SetColor(6, 0); cout << "'0'" << endl;
                    cout << endl;
                    SetColor(7, 0);
                    cout << "\t\t\t\t   Поле компьютера\t\t     Ваше поле" << endl;
                    //setlocale(LC_ALL, "C"); // Служит для использования extended ascii table c++


                    for (int i = 0; i < mapSize; i++) // Рисуем поле
                    {
                        cout << "                           ";
                        for (int j = 0; j < mapSize; j++)
                        {
                            if (i == posY && j == posX) {
                                SetColor(6, 0);
                                cout << "# ";
                                SetColor(7, 0);
                                if (key == 'e' || key == 'E' || key == 'г' || key == '“') {
                                    if (mapComputerSpace[i][j] == dHit /*'X'*/ || mapComputerSpace[i][j] == dMiss /*'-'*/) {
                                    }
                                    else {
                                        switch (mapComputer[i][j]) {
                                        case dShip /*'+'*/:

                                            mapComputerSpace[i][j] = dHit /*'X'*/;
                                            hitControl(mapComputerSpace, masShipComputer);
                                            pointsUser++;
                                            break;
                                        case ' ':
                                            mapComputerSpace[i][j] = dMiss /*'-'*/;
                                            if (setting._compAIsimple == true) //Если комьютер слабый
                                                do {} while (simpleAI(mapUser, masShipUser, pointsComputer));
                                            else
                                                do {} while (hardAI(mapUser, masShipUser, pointsComputer, HAiHit));
                                            break;
                                        }
                                    }
                                }
                            }
                            else {
                                //Раскрашиваем поле
                                switch (mapComputerSpace[i][j]) {
                                    colors
                                }
                                cout << mapComputerSpace[i][j] << ' ';
                            }
                        }
                        cout << "   ";
                        for (int j = 0; j < mapSize; j++)
                        {
                            //Раскрашиваем поле
                            switch (mapUser[i][j]) {
                                colors
                            }
                            cout << mapUser[i][j] << ' ';
                        }
                        //Для отладки раскоментировать
                        /*cout << "   ";
                        for (int j = 0; j < mapSize; j++)
                        {
                            cout << mapComputer[i][j] << ' ';
                        }*/
                        cout << endl;
                    }
                    SetColor(7, 0);
                    cout << "                                     Ваши очки: " << pointsUser;
                    cout << "                Очки компьютера: " << pointsComputer << endl;
                    if (pointsComputer == 20) {
                        cycle = false;
                        //-----------------------------------------------
                        setcur(0, 0);
                        SetColor(7, 0);
                        cout << "\t   Для управления используйте кнопки "; SetColor(6, 0); cout << "'w' 'a' 's' 'd'"; SetColor(7, 0);
                        cout << " и кнопку "; SetColor(6, 0); cout << "'e'"; SetColor(7, 0); cout << " для выстрела. Выход в меню "; SetColor(6, 0); cout << "'0'" << endl;
                        cout << endl;
                        SetColor(7, 0);
                        cout << "\t\t\t\t   Поле компьютера\t\t     Ваше поле" << endl;
                        for (int i = 0; i < mapSize; i++) // Рисуем поле
                        {
                            cout << "                           ";
                            for (int j = 0; j < mapSize; j++)
                            {
                                //Раскрашиваем поле
                                switch (mapComputerSpace[i][j]) {
                                    colors
                                }
                                cout << mapComputerSpace[i][j] << ' ';
                            }
                            cout << "   ";
                            for (int j = 0; j < mapSize; j++)
                            {
                                //Раскрашиваем поле
                                switch (mapUser[i][j]) {
                                    colors
                                }
                                cout << mapUser[i][j] << ' ';
                            }
                            cout << endl;
                        }
                        //-----------------------------------------------
                        SetColor(7, 0);
                        cout << "                                     Ваши очки: "; SetColor(4, 0); cout << pointsUser; SetColor(7, 0);
                        cout << "                Очки компьютера: "; SetColor(4, 0); cout << pointsComputer << "  " << endl; SetColor(7, 0);
                        cout << endl;
                        cout << "                                                  ---------------" << endl;
                        SetColor(4, 0);
                        cout << "                                                 Компьютер победил!\n";
                        SetColor(7, 0);
                        cout << "                                                  ---------------" << endl;
                        cout << "                                           Для выхода в меню нажмите '0'" << endl;
                        do {
                            key = _getch();
                        } while (key != '0');
                        break;
                    }
                    if (pointsUser == 20) {
                        cycle = false;
                        //-----------------------------------------------
                        setcur(0, 0);
                        SetColor(7, 0);
                        cout << "\t   Для управления используйте кнопки "; SetColor(6, 0); cout << "'w' 'a' 's' 'd'"; SetColor(7, 0);
                        cout << " и кнопку "; SetColor(6, 0); cout << "'e'"; SetColor(7, 0); cout << " для выстрела. Выход в меню "; SetColor(6, 0); cout << "'0'" << endl;
                        cout << endl;
                        SetColor(7, 0);
                        cout << "\t\t\t\t   Поле компьютера\t\t     Ваше поле" << endl;
                        for (int i = 0; i < mapSize; i++) // Рисуем поле
                        {
                            cout << "                           ";
                            for (int j = 0; j < mapSize; j++)
                            {
                                //Раскрашиваем поле
                                switch (mapComputerSpace[i][j]) {
                                    colors
                                }
                                cout << mapComputerSpace[i][j] << ' ';
                            }
                            cout << "   ";
                            for (int j = 0; j < mapSize; j++)
                            {
                                //Раскрашиваем поле
                                switch (mapUser[i][j]) {
                                    colors
                                }
                                cout << mapUser[i][j] << ' ';
                            }
                            cout << endl;
                        }
                        //-----------------------------------------------
                        SetColor(7, 0);
                        cout << "                                     Ваши очки: "; SetColor(4, 0); cout << pointsUser; SetColor(7, 0);
                        cout << "               Очки компьютера: "; SetColor(4, 0); cout << pointsComputer << "  " << endl; SetColor(7, 0);
                        cout << endl;
                        cout << "                                                  ---------------" << endl;
                        SetColor(4, 0);
                        cout << "                                                    Вы победили!\n";
                        SetColor(7, 0);
                        cout << "                                                  ---------------" << endl;
                        cout << "                                           Для выхода в меню нажмите '0'" << endl;
                        do {
                            key = _getch();
                        } while (key != '0');
                        break;
                    }
                    //Для отладки раскоментировать
                    /*cout << "HAiHit.randNum " << HAiHit.randNum << endl;
                    cout << "HAiHit.pass " << HAiHit.pass << endl;
                    cout << "HAiHit.firstLines " << HAiHit.firstLines << " " << endl;
                    cout << "HAiHit.colums " << HAiHit.firstColums << " " << endl;
                    cout << "-------------------" << endl;
                    cout << "HAiHit.up " << HAiHit.up.status << " \tHAiHit.up.index " << HAiHit.up.index << " " << endl;
                    cout << "HAiHit.down " << HAiHit.down.status << " \tHAiHit.down.index " << HAiHit.down.index << " " << endl;
                    cout << "HAiHit.left " << HAiHit.left.status << " \tHAiHit.left.index " << HAiHit.left.index << " " << endl;
                    cout << "HAiHit.right " << HAiHit.right.status << " \tHAiHit.right.index " << HAiHit.right.index << " " << endl;*/

                    key = _getch();// Слушаем нажатие клавиши
                    //Для отладки раскоментировать
                    //cout << posX << " " << posY;
                    //cycle = false;
                    /*for (int i = 0; i < 20; i++)
                    {
                        for (int j = 0; j < 2; j++)
                        {
                            cout << masShipUser[i][j] << " ";
                        }
                        cout << endl;
                    }*/
                } while (cycle);
            }
        }
        else { // Режим Комьютер-Компьютер
            //--------------------------------------------------------------------
            ships(mapComputer, mapSize, masShipComputer);
            ships(mapUser, mapSize, masShipUser);
            //--------------------------------------------------------------------
            do // Игровой цикл
            {
                //system("cls");
                setcur(0, 0);
                //setlocale(LC_ALL, "Russian");
                //system("color 07");
                SetColor(7, 0);
                cout << "\t\t\t\t  Для паузы нажмите кнопку";  SetColor(6, 0);
                cout << "'p'"; SetColor(7, 0); cout << ", для выхода в меню";  SetColor(6, 0); cout << "'0'\n" << endl;
                SetColor(7, 0);
                cout << "\t\t\t\t   Компьютер слева \t          Компьютер справа " << endl;
                //setlocale(LC_ALL, "C"); // Служит для использования extended ascii table c++

                //Реализации паузы в цыкле
                // Тоже считывает нажатие но не останавливает поток
                if (_kbhit()) {
                    key = _getch();
                    if (key == 'p' || key == 'P' || key == '§' || key == '‡') {
                        if (_getch() == 'p' || _getch() == 'P' || _getch() == '§' || _getch() == '‡')
                            continue;
                    }

                    if (key == '0') {
                        break;
                    }
                }

                if (setting._compAIsimple == true) //Комьютер 1
                    do {} while (simpleAI(mapUser, masShipUser, pointsComputer));
                else
                    do {} while (hardAI(mapUser, masShipUser, pointsComputer, HAiHit));

                if (setting._compAIsimple == true) //Комьютер 2
                    do {} while (simpleAI(mapComputer, masShipComputer, pointsUser));
                else
                    do {} while (hardAI(mapComputer, masShipComputer, pointsUser, HAiHit2));

                if (pointsUser == 20 && pointsComputer == 20)
                    pointsUser--;



                for (int i = 0; i < mapSize; i++) // Рисуем поле
                {
                    cout << "                           ";
                    for (int j = 0; j < mapSize; j++)
                    {
                        //Раскрашиваем поле
                        switch (mapUser[i][j]) {
                            colors
                        }
                        cout << mapUser[i][j] << ' ';
                    }

                    cout << "   ";
                    for (int j = 0; j < mapSize; j++)
                    {
                        //Раскрашиваем поле
                        switch (mapComputer[i][j]) {
                            colors
                        }
                        cout << mapComputer[i][j] << ' ';
                    }
                    cout << endl;
                }
                Sleep(300); //Задержка
                SetColor(7, 0);
                cout << "                              Очки комьютера слева  : "; SetColor(4, 0); cout << pointsComputer; SetColor(7, 0);
                cout << "     Очки комьютера справа : "; SetColor(4, 0); cout << pointsUser << " " << endl; SetColor(7, 0);


                if (pointsComputer == 20) {
                    cycle = false;
                    cout << "                                             ---------------------------" << endl;
                    SetColor(4, 0);
                    cout << "                                              Компьютер слева. Победил!\n";
                    SetColor(7, 0);
                    cout << "                                             ---------------------------" << endl;
                    cout << "                                            Для выхода в меню нажмите '0'" << endl;
                    do {
                        key = _getch();
                    } while (key != '0');
                    break;
                }
                if (pointsUser == 20) {
                    cycle = false;
                    cout << "                                           -----------------------------" << endl;
                    SetColor(4, 0);
                    cout << "                                             Компьютер справа. Победил!\n";
                    SetColor(7, 0);
                    cout << "                                           -----------------------------" << endl;
                    cout << "                                            Для выхода в меню нажмите '0'" << endl;
                    do {
                        key = _getch();
                    } while (key != '0');
                    break;
                }
            } while (cycle);
        }
        system("cls");
    }
}
#else
#endif
