#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    vblayout = new QVBoxLayout();

    addCat = new QToolButton();
    addCat->setText("+");

    delCat = new QToolButton();
    delCat->setText("-");

    save = new QToolButton();
    save->setText("+");

    del = new QToolButton();
    del->setText("-");

    catList = new QComboBox();
    catList->addItem("Категория");

    saveList = new QComboBox();
    saveList->addItem("Заметка");

    text = new QPlainTextEdit();

    hblayout1 = new QHBoxLayout();
    hblayout1->addWidget(catList);
    hblayout1->addWidget(addCat);
    hblayout1->addWidget(delCat);

    hblayout2 = new QHBoxLayout();
    hblayout2->addWidget(saveList);
    hblayout2->addWidget(save);
    hblayout2->addWidget(del);

    vblayout->addLayout(hblayout1);
    vblayout->addLayout(hblayout2);
    vblayout->addWidget(text);

    testbtn = new QPushButton("test");
    vblayout->addWidget(testbtn);

    configFile = new QFile();
    configPath = new QDir();
    saveFile = new QFile();
    savePath = new QDir();

    connect(testbtn, SIGNAL(clicked(bool)), this, SLOT(test()));
    connect(addCat, SIGNAL(clicked(bool)), this, SLOT(categoryAdd()));
    connect(catList, SIGNAL(currentIndexChanged(int)), this, SLOT(categoryChanche()));
    connect(delCat, SIGNAL(clicked(bool)), this, SLOT(categoryRemove()));
    connect(saveList, SIGNAL(currentIndexChanged(int)), this, SLOT(textListChanged()));

    HomePath = QDir::homePath();
    if(QApplication::platformName() == "android"){
        HomePath += "/Saver";
    }else{
        if(QApplication::platformName() == "windows"){
            //проверяем наличие конфига
            HomePath += "/Saver";
        }else{
            //linux
            HomePath += "/.local/share/Saver";
        }
    }

    HPath = new QDir(HomePath);
    if(!HPath->exists()){
        HPath->mkdir(HomePath);
    }

    listDirs(HomePath);
    listFiles(HomePath);
}

MainWindow::~MainWindow()
{
    if(configFile->isOpen())
        configFile->close();
    if(saveFile->isOpen())
        saveFile->close();
}

//тестовая кнопка, переделается в "сохранить"
void MainWindow::test(){
    QString info;
    info = HomePath + "\r\n";
    info += QApplication::platformName() + "\r\n";
    text->setPlainText(info);
}

//добавление категории
void MainWindow::categoryAdd(){
    QString str = QInputDialog::getText(0, "Input", "Name",
                                        QLineEdit::Normal);
    if(str == "") {
        return;
    }
    QDir dir;
    //dir.setPath(HomePath + "/" + str);
    dir.mkdir(HomePath + "/" + str);
    catList->addItem(str);
}
//удаление категории
void MainWindow::categoryRemove(){
    if(catList->currentIndex() == 0){
        return;
    }
    QMessageBox *msg = new QMessageBox("Удаление категории",
                                       catList->currentText() + " удаляем?",
                                       QMessageBox::Information,
                                       QMessageBox::Yes,
                                       QMessageBox::No,
                                       QMessageBox::Cancel | QMessageBox::Escape);

    int n = msg->exec();
    delete msg;
    if(n == QMessageBox::Yes){
        QDir dir;
        dir.setPath(HomePath + "/" + catList->currentText());
        dir.removeRecursively();
        catList->removeItem(catList->currentIndex());
    }
}
//выбор категории
void MainWindow::categoryChanche() {
    if(catList->currentIndex() == 0){
        listFiles(HomePath);
    }else{
        listFiles(HomePath + "/" + catList->currentText());
    }
}
//сохранение заметки
void MainWindow::textSave(){
    QString path = HomePath;
    QString filename = "";
    bool newsave;

    if(catList->currentIndex() == 0){
        //если категория корневая
        path += "/";
    }else{
        //если выбранная категория
        path += "/" + catList->currentText() + "/";
    }

    if(saveList->currentIndex() == 0){
        //если не выбрана заметка, спрашиваем имя заметки
        filename = QInputDialog::getText(0, "Сохранение заметки", "Имя заметки", QLineEdit::Normal);
        if(filename == "") return;
        newsave = true;
    }else{
        //есть выбранная заметка, пишем в неё
        filename = saveList->currentText();
        path += filename;
        newsave = false;
    }

    //собственно сама запись в файл
    QFile fwrite;
    fwrite.setFileName(path + filename);
    if(fwrite.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream ts(&fwrite);
        ts.setCodec("UTF-8");
        ts << text->toPlainText();
        fwrite.close();
        if(newsave){
            saveList->addItem(filename);
        }
    }
}
//удаление заметки
void MainWindow::textRemove(){

}
//выбор заметки
void MainWindow::textListChanged(){
    QString path = HomePath;
    if(catList->currentIndex() != 0){
        path += "/" + catList->currentText() + "/";
    }else{
        path += "/";
    }
    if(saveList->currentIndex() > 0){
        path += saveList->currentText();
        text->setPlainText(path);
    }else{
        return;
    }

    if(text->toPlainText().count() > 0){
        //нужно ли сохранить
    }else{

    }

}

void MainWindow::listDirs(QString path){
    catList->clear();
    QDir dir;
    dir.setPath(path);
    QStringList list = dir.entryList(QDir::Dirs);
    catList->addItem("Категория");
    foreach (QString item, list) {
        if(item != "." && item != "..")
            catList->addItem(item);
    }
}

//список файлов/заметок
void MainWindow::listFiles(QString path){
    saveList->clear();
    QDir dir;
    dir.setPath(path);
    QStringList list = dir.entryList(QDir::Files);
    saveList->addItem("Заметка");
    foreach (QString item, list) {
        if(item != "." && item != "..")
            saveList->addItem(item);
    }
}

//запись
bool MainWindow::fWrite(QString path, QString tofile){
    QFile f(path);
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream wStream(&f);
        wStream.setCodec("UTF-8");
        wStream << tofile;
        f.close();
        return true;
    }

    return false;
}

//чтение
QString MainWindow::fRead(QString path){
    QFile f(path);
    if(!f.open(QIODevice::ReadOnly|QIODevice::Text)){
        //not open
        return "";
    }

    QByteArray data = f.readAll();
    return QString(data);
}

void MainWindow::msg(QString text, QString title = QApplication::applicationName()){
    QMessageBox(QMessageBox::Information, title, text, QMessageBox::Ok);
}
