#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    vblayout = new QVBoxLayout();

    addCat = new QToolButton();
    addCat->setText("+");

    delCat = new QToolButton();
    delCat->setText("-");

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
    hblayout2->addWidget(del);

    vblayout->addLayout(hblayout1);
    vblayout->addLayout(hblayout2);
    vblayout->addWidget(text);

    testbtn = new QPushButton("Сохранить");
    vblayout->addWidget(testbtn);

    connect(testbtn, SIGNAL(clicked(bool)), this, SLOT(textSave()));
    connect(addCat, SIGNAL(clicked(bool)), this, SLOT(categoryAdd()));
    connect(catList, SIGNAL(currentIndexChanged(int)), this, SLOT(categoryChanche()));
    connect(delCat, SIGNAL(clicked(bool)), this, SLOT(categoryRemove()));
    connect(saveList, SIGNAL(currentIndexChanged(int)), this, SLOT(textListChanged()));
    connect(del, SIGNAL(clicked(bool)), this, SLOT(textRemove()));

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

    QDir *HPath = new QDir(HomePath);
    if(!HPath->exists()){
        HPath->mkdir(HomePath);
    }
    delete HPath;

    listDirs(HomePath);
    listFiles(HomePath);
}

MainWindow::~MainWindow()
{

}


void MainWindow::test(){
    fWrite(HomePath + "/testfile", "test text");
}

//добавление категории
void MainWindow::categoryAdd(){
    QString str = QInputDialog::getText(0, "Добавление категории", "Название категории",
                                        QLineEdit::Normal);
    if(str == "") {
        return;
    }
    QDir dir;
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
        newsave = false;
    }

    path += filename;
    if(fWrite(path, text->toPlainText())){
        if(newsave) saveList->addItem(filename);
    }

    text->clear();
}

//удаление заметки
void MainWindow::textRemove(){
    if(saveList->currentIndex() == 0) return;
    QMessageBox m;
    m.setText("Удаляем " + saveList->currentText() + "?");
    m.addButton(QMessageBox::Yes);
    m.addButton(QMessageBox::No);
    if(m.exec() == QMessageBox::No){
        return;
    }

    QString path = HomePath;
    if(catList->currentIndex() == 0){
        path += "/";
    }else{
        path += "/" + catList->currentText() + "/";
    }
    path += saveList->currentText();

    QFile file;
    file.setFileName(path);
    if(file.remove()){
        saveList->removeItem(saveList->currentIndex());
    }
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
    }else{
        return;
    }

    if(text->toPlainText().count() > 0){
        //нужно ли сохранить
        QMessageBox m;
        m.setText("Есть несохранённая заметка, открыть новую?");
        m.addButton(QMessageBox::Yes);
        m.addButton(QMessageBox::No);
        if(m.exec() == QMessageBox::Yes){
            text->setPlainText(fRead(path));
        }else{
            return;
        }
    }else{
        text->setPlainText(fRead(path));
    }

}

//список каталогов
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
        return "";
    }

    QByteArray data = f.readAll();
    return QString(data);
}

void MainWindow::msg(QString text, QString title = QApplication::applicationName()){
    QMessageBox(QMessageBox::Information, title, text, QMessageBox::Ok);
}
