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
    QToolButton *save;
    QToolButton *del;
    QPlainTextEdit *text;
    QPushButton *testbtn;
    QHBoxLayout *hblayout1;
    QHBoxLayout *hblayout2;

    //getExistsDirectory alternative
    void getDir(QString path, bool up);
    QWidget *wid;
    QVBoxLayout *vb;
    QHBoxLayout *hb1;

    QLabel *ldir;
    QDir *dir;
    QString currentPath;
    QLabel *curdir;

    //for config path and saves
    QDir *configPath;
    QFile *configFile;
    QDir *savePath;
    QFile *saveFile;
    QString sconfig;
    QString ssave;
    const QString configName = "saverconfig";

    /*new*/
    QString HomePath;
    QDir *HPath;
    QString LastText;
    bool fWrite(QString path, QString tofile);
    QString fRead(QString path);
    void msg(QString text, QString title);

public:

    QVBoxLayout *vblayout;

public slots:
    void test();
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
