#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QComboBox *catList;
    QToolButton *addCat;
    QToolButton *delCat;
    QComboBox *saveList;
    QToolButton *del;
    QPlainTextEdit *text;
    QPushButton *testbtn;
    QHBoxLayout *hblayout1;
    QHBoxLayout *hblayout2;

    QString HomePath;
    QString LastText;
    bool fWrite(QString path, QString tofile);
    QString fRead(QString path);
    void msg(QString text, QString title);

public:

    QVBoxLayout *vblayout;

public slots:
    void categoryAdd();
    void categoryRemove();
    void categoryChanche();
    void textSave();
    void textRemove();
    void textListChanged();
    void listDirs(QString path);
    void listFiles(QString path);

};

#endif // MAINWINDOW_H
