// Программа для тренировки иностанных слов. 2024 Denis Petrov
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen> // Эту библиотеку использовал чтобы отцентровать форму
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString str = ""; // Для хранения слова
int countWord = 0; //Количество слов
int randNum = 0; //Храним случайное число
int randNumPast = -1; //Храним прошлое случайное число
int* masLearden = nullptr, countLearden = 0; //Храним массив для запомниания выучинных слов
int* masReapeat = nullptr, countReapeat = 0; //Храним массив для повторения выучинных слов
int countWordForLearnEnd = 150; // До какого слова учить
int countWordForLearnStart = 0; // С какого слова учить
bool mode = true; //Режим игры true - обучение, false - повторение
double prozent = 0;

MainWindow::MainWindow(QWidget *parent) // Здесь программа начинает работать
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->setVisible(false);
    //Этот код нужен чтобы отцентровать форму
    move(QGuiApplication::screens().at(0)->geometry().center() - frameGeometry().center());
    keyF1 = new QShortcut(this);   // Инициализируем объект
    keyF1->setKey(Qt::Key_F1);    // Устанавливаем код клавиши
    keyF2 = new QShortcut(this);   // Инициализируем объект
    keyF2->setKey(Qt::Key_F2);    // Устанавливаем код клавиши
    keyF3 = new QShortcut(this);   // Инициализируем объект
    keyF3->setKey(Qt::Key_F3);    // Устанавливаем код клавиши
    // цепляем обработчик нажатия клавиши
    connect(keyF1, SIGNAL(activated()), this, SLOT(slotShortcutF1()));
    connect(keyF2, SIGNAL(activated()), this, SLOT(slotShortcutF2()));
    connect(keyF3, SIGNAL(activated()), this, SLOT(slotShortcutF3()));
    //---------------------------------------
    srand(time(NULL)); // Генерируем случайное зерно
    readFiles(); // Читаем данные из файлов

}
void MainWindow::randFunk(bool word){ //Генерация случайного числа и проверка правильности ввода
    if(mode == true){ //Если режим обучения
        if (countLearden >= countWordForLearnEnd){
            ui->label->setText("Нет слов");
            return;
        }
        bool learned = false;
        do{
            learned = false;
            //qDebug() << "Случайное число " <<randNum;
            randNum = countWordForLearnStart + rand() % countWordForLearnEnd; // Генерируем случайное число на количество слов в словаре
            for (int i = 0; i < countLearden; i++){
                //qDebug() << "Массив " << masLearden[i];
                //Если это число было в списке запомненных, то генерируем его заного
                if (randNum == masLearden[i]){
                    learned = true;
                }
            }
            //Проверка когда осталось последнее слово, чтобы программа не попадала в бесконечный цыкл
            if (randNumPast == randNum && countLearden == countWordForLearnEnd - 1 ){
                break;
            }
        }while(randNumPast == randNum || learned);
        randNumPast = randNum;
    }
    else{//Если режим повторения
        if (countReapeat >= countWordForLearnEnd){
            ui->label->setText("Нет слов");
            return;
        }
        bool repeated = false;
        do{
            repeated = false;
            //qDebug() << "Случайное число " <<randNum;
            randNum = countWordForLearnStart + rand() % countWordForLearnEnd; // Генерируем случайное число на количество слов в словаре
            for (int i = 0; i < countWordForLearnEnd ; i++){
                //qDebug() << "Массив " << masLearden[i];
                //Если это число было в списке запомненных, то генерируем его заного
                if (randNum == masReapeat[i]){
                    repeated = true;
                }
            }
            //Проверка когда осталось последнее слово, чтобы программа не попадала в бесконечный цыкл
            if (randNumPast == randNum && countReapeat == countWordForLearnEnd - 1 ){
                break;
            }
        }while(randNumPast == randNum || repeated);
        randNumPast = randNum;
    }
    //-------------------------------------------------------------------------
    str = ui->listWidget->item(randNum)->text(); //Записываем в строку слово
    if (word){
        //--------------------------------
        ui->label->setText(str.mid(str.indexOf(">") + 1,str.length())); // Чтения слова от пробела и далее
        str = str = str.mid(0, str.indexOf(">")); //Возвращаем слово до 1 пробела
        //--------------------------------
    }
    else
    {
        //--------------------------------
        ui->label->setText(str.mid(0, str.indexOf(">"))); // Чтения слова от пробела и далее
        str = str = str.mid(str.indexOf(">") + 1,str.length()); //Возвращаем слово до 1 пробела
        //--------------------------------
    }

}
void MainWindow::readFiles(){
    //-----Чтение из файла словаря
    //Временный путь /home/dest/Testus/words.txt
    QFile file(QCoreApplication::applicationDirPath() + "/words.txt");
        //Для вывода отнасительного пути
        //qDebug() << "Путь файла: " << QCoreApplication::applicationDirPath() << "\n";
        if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file.atEnd())
            {
                //читаем строку
                QString iso = file.readLine();
                //Удалеем из строки /n
                iso.resize(iso.size() - 1);
                //Записываем в listWidget
                countWord++;
                ui->listWidget->addItem(iso);
            }
        }
        else
        {
            ui->label->setText("Файл не найден");
            qDebug() << "Files not found! /learden.txt and /words.txt";
            qDebug() << "Current work dir: " << QCoreApplication::applicationDirPath() << "\n";
        }
        file.close();
    //------------------------------------------------------------------------
    //Заполяем массив значениями -1
    masLearden = new int [countWord];
    for (int i = 0; i < countWord; i++){
        masLearden[i] = -1;
    }
    //-----Чтение из файла запомниных слов
    QFile file2(QCoreApplication::applicationDirPath() + "/learden.txt");
        if(file2.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            while(!file2.atEnd())
            {
                //читаем строку
                QString iso = file2.readLine();
                //Удалеем из строки /n
                iso.resize(iso.size() - 1);
                //Записываем в listWidget
                masLearden[countLearden] = iso.toInt();
                countLearden++;
            }
        }
        else
        {
            ui->label->setText("Файл не найден");
        }
        ui->label_5->setText("Слов запомнил: " + QString::number(countLearden));
        file2.close();
    //------------------------------------------------------------------------
    ui->label_3->setText("Количество слов в словаре: " + QString::number(countWord));
    prozent = countLearden / (countWord * 0.01); // высчитываем процент запомненных слов
    ui->progressBar->setValue(int(prozent));
    lengCoise(); //вызываем функцию для генерации выбора языка
}
void MainWindow::slotShortcutF1() //Обработка клавиши F1
{
    //Если правильно запомнить, повторить
    if(ui->lineEdit->text() != ""){ //Проверка на пустую строку
        if(ui->lineEdit->text() == str){
            //ui->label_2.setText("Правильно");
            ui->label_2->setText("Правильно");
            ui->lineEdit->setText("");
            on_pushButton_4_clicked();
        }
        else{
            ui->label_2->setText("Не правильно");
        }
    }
}
void MainWindow::slotShortcutF2() //Обработка клавиши F2
{

    on_pushButton_2_clicked();
}
void MainWindow::slotShortcutF3() //Обработка клавиши F3
{
    on_pushButton_3_clicked();
}
void MainWindow::on_pushButton_clicked() //Ктопка Enter
{

    on_lineEdit_returnPressed();
    //ui->label->setText(ui->lineEdit->text());
    //ui->label->setText(ui->listWidget->item(0)->text()); // Присвоение конкретного элемента из listWidget

}
void MainWindow::on_lineEdit_returnPressed() // Если нажата клавиша Enter
{
    if(ui->lineEdit->text() != ""){ //Проверка на пустую строку
        if(ui->lineEdit->text() == str){
            //ui->label_2.setText("Правильно");
            ui->label_2->setText("Правильно");
            ui->lineEdit->setText("");

            lengCoise(); //вызываем функцию для генерации выбора языка
        }
        else{
            ui->label_2->setText("Не правильно");
        }
    }
}
void MainWindow::on_pushButton_2_clicked() //Кнопка подсказка
{
    ui->label_2->setText(str = str.mid(0, str.indexOf(">")));
    ui->lineEdit->setFocus();
}
void MainWindow::on_pushButton_3_clicked() //Кнопка попрустить
{
    lengCoise(); //вызываем функцию для генерации выбора языка
}
void MainWindow::lengCoise(){ //Выбор языка
    //-------На каком языке будут отображаться слова-------------------
    if (ui->radioButtonRusEng->isChecked()){
        randFunk(rand() % 2);
    }
    if (ui->radioButtonRus->isChecked()){
        randFunk(true);
    }
    if (ui->radioButtonEng->isChecked()){
        randFunk(false);
    }
    //-----------------------------------------------------------------
}
void MainWindow::on_pushButton_4_clicked() //Кнопка запомнил, повторил
{
    if(mode == true){
        prozent = countLearden / (countWord * 0.01); // высчитываем процент запомненных слов
        ui->progressBar->setValue(int(prozent));
        masLearden[countLearden] = randNum;
        if (countLearden < countWordForLearnEnd){
            countLearden++;
        }
        lengCoise(); //вызываем функцию для генерации выбора языка
        QFile fileOut(QCoreApplication::applicationDirPath() + "/learden.txt"); // Связываем объект с файлом fileout.txt
        if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
        { // Если файл успешно открыт для записи в текстовом режиме
            QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream
           // и передаем ему адрес объекта fileOut
            for (int i = 0; i < countLearden; i++){
                writeStream << masLearden[i] << "\n"; // Посылаем строку в поток для записи
            }
            fileOut.close(); // Закрываем файл
        }
        ui->label_5->setText("Слов запомнил: " + QString::number(countLearden));
    }else{
        masReapeat[countReapeat] = randNum;
        if (countReapeat < countWordForLearnEnd){
            countReapeat++;
        }
        lengCoise(); //вызываем функцию для генерации выбора языка
        ui->label_5->setText("Слов повторил: " + QString::number(countReapeat));
    }
}
void MainWindow::on_learnButton_clicked() //Кнопка учить
{
    ui->label_13->setText("Вы находитесь в режиме: <b>Обучения</b>");
    mode = true;
    ui->pushButton_4->setText("Запомнил");
    ui->label_15->setText("F1 - Запомнить");
    ui->label_5->setText("Слов запомнил: " + QString::number(countLearden));
    countWordForLearnStart = ui->LearnStart->text().toInt();
    countWordForLearnEnd = ui->LearnEnd->text().toInt();

    lengCoise();
}
void MainWindow::on_repeatButton_clicked() //Кнопка повторить
{
    ui->label_13->setText("Вы находитесь в режиме: <b>Повторения</b>");
    mode = false;
    ui->label_15->setText("F1 - Повторил");
    ui->pushButton_4->setText("Повторил");
    countReapeat = 0;
    ui->label_5->setText("Слов повторил: " + QString::number(countReapeat));
    countWordForLearnStart = ui->repeatStart->text().toInt();
    countWordForLearnEnd = ui->repeatEnd->text().toInt() - ui->repeatStart->text().toInt();

    masReapeat = new int [countWordForLearnEnd];
    for (int i = 0; i < countWordForLearnEnd; i++){
        masReapeat[i] = -1;
    }
    lengCoise();
}
MainWindow::~MainWindow() // Удаляет форму
{
    delete[] masReapeat;
    delete[] masLearden;
    delete ui;
}
