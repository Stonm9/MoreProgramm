#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut> //Библиотека для горячих клавиш

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void randFunk(bool); //Генерация случайного числа
    void on_pushButton_4_clicked();
    void slotShortcutF1(); //Горячая клавиша F1
    void slotShortcutF2(); //Горячая клавиша F2
    void slotShortcutF3(); //Горячая клавиша F3
    void lengCoise(); //Выбор языка для отображения
    void readFiles(); //Чтение данных из файлов
    void on_learnButton_clicked();

    void on_repeatButton_clicked();

private:
    Ui::MainWindow *ui;
    QShortcut *keyF1;    // объект горячей клавиши F1
    QShortcut *keyF2;    // объект горячей клавиши F2
    QShortcut *keyF3;    // объект горячей клавиши F3
};
#endif // MAINWINDOW_H
