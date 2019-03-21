#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetWidgets();

    setWindowTitle("Лаба 1 Qt");
    CurrentDirName = QApplication::applicationDirPath();
    watcher.addPath(CurrentDirName);
    pleCurrPath->setText(CurrentDirName);
    RefreshLists();

    SetMenu();

    connect(&watcher, SIGNAL(directoryChanged(QString)), SLOT(RefreshLists()));
    connect(&watcher, SIGNAL(fileChanged(QString)), SLOT(RefreshLists()));
}

void MainWindow::SetWidgets() {
    CurrentFileName = "";

    //расстановка и коннект

    //едит для тек директории
    pleCurrPath = new QLineEdit;
    pleCurrPath->setToolTip("Текущая директория");
    pleCurrPath->setReadOnly(true);

    //три столпа
    pListDirs = new QListWidget;
    pListFileNames = new QListWidget;
    pTextEdit = new QTextEdit;
    Q_ASSERT(connect(pListDirs, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(ChangeCurrDir(QListWidgetItem*))));
    Q_ASSERT(connect(pListFileNames, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(SelectFile(QListWidgetItem*))));
    pListDirs->setMinimumWidth(160);
    pListDirs->setMinimumHeight(200);
    pListFileNames->setMinimumWidth(160);
    pTextEdit->setMinimumWidth(320);
    pTextEdit->setWordWrapMode(QTextOption::NoWrap);

    //разделитель
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    //кодировка
    prbCodeCP1251 = new QRadioButton("cp1251");
    prbCodeCP1251->setChecked(true);
    prbCodeUTF_8 = new QRadioButton("UTF-8");
    //Q_ASSERT(connect(prbCodeCP1251, SIGNAL(clicked()), SLOT(OpenCurrentFile())));
    //Q_ASSERT(connect(prbCodeUTF_8, SIGNAL(clicked()), SLOT(OpenCurrentFile())));

    //настройка текста
    pcbBold = new QCheckBox("Полужирный");
    pcbItalic = new QCheckBox("Курсив");
    pcbUnderlined = new QCheckBox("Подчеркнутый");
    Q_ASSERT(connect(pcbBold, SIGNAL(clicked()), SLOT(ChangeFont())));
    Q_ASSERT(connect(pcbItalic, SIGNAL(clicked()), SLOT(ChangeFont())));
    Q_ASSERT(connect(pcbUnderlined, SIGNAL(clicked()), SLOT(ChangeFont())));

    //цвет
    pcbColor = new QComboBox;
    pcbColor->setToolTip("Цвет текста");
    pcbColor->addItem("Черный");
    pcbColor->addItem("Зеленый");
    pcbColor->addItem("Красный");
    pcbColor->addItem("Серый");
    pcbColor->addItem("Синий");
    Q_ASSERT(connect(pcbColor, SIGNAL(currentIndexChanged(int)), SLOT(ChangeFont())));

    //сброс
    QPushButton *pbReset = new QPushButton("Сброс");
    pbReset->setToolTip("Сброс параметров текста");
    Q_ASSERT(connect(pbReset, SIGNAL(clicked()), SLOT(ResetFont())));

    //расстановка layouts
    QWidget *w = new QWidget;
    QVBoxLayout *pvlMain = new QVBoxLayout;
        QHBoxLayout *phlEditFiles = new QHBoxLayout;
        QHBoxLayout *phlOptions = new QHBoxLayout;
            QGroupBox *pgbCode = new QGroupBox("Кодировка");
                QVBoxLayout * pvLayCode = new QVBoxLayout;
            QGroupBox *pgbFont = new QGroupBox("Шрифт");
                QVBoxLayout *pvlFont = new QVBoxLayout;
            QVBoxLayout *pvl_Col_Res = new QVBoxLayout;

    pvlMain->addWidget(pleCurrPath);
    pvlMain->addLayout(phlEditFiles);
        phlEditFiles->addWidget(pListDirs, 1);
        phlEditFiles->addWidget(pListFileNames, 1);
        phlEditFiles->addWidget(pTextEdit, 2);
    pvlMain->addWidget(line);
    pvlMain->addLayout(phlOptions);
        phlOptions->addWidget(pgbCode);
            pgbCode->setLayout(pvLayCode);
                pvLayCode->addWidget(prbCodeCP1251);
                pvLayCode->addWidget(prbCodeUTF_8);
        phlOptions->addWidget(pgbFont);
            pgbFont->setLayout(pvlFont);
                pvlFont->addWidget(pcbBold);
                pvlFont->addWidget(pcbItalic);
                pvlFont->addWidget(pcbUnderlined);
        phlOptions->addLayout(pvl_Col_Res);
            pvl_Col_Res->addWidget(pcbColor);
            pvl_Col_Res->addWidget(pbReset);

    w->setLayout(pvlMain);
    setCentralWidget(w);
}

void MainWindow::SetMenu() {
    QMenu *pmFile = new QMenu("&Файл");
    pmFile->addAction("&Открыть", this, SLOT(OpenDlgFile()), Qt::CTRL + Qt::Key_O);
    pmFile->addAction("&Сохранить", this, SLOT(SaveCurrentFile()), Qt::CTRL + Qt::Key_S);
    pmFile->addAction("Сохранить &как...", this, SLOT(SaveDlgFile()), Qt::CTRL + Qt::SHIFT + Qt::Key_S);



    pmFile->addSeparator();
    pmFile->addAction("&Выйти", this, SLOT(close()));

    QMenu *pmHelp = new QMenu("&?");
    pmHelp->addAction("&Автор", this, SLOT(About()));
    pmHelp->addAction("&О программе", this, SLOT(Help()), Qt::Key_F1);


    ui->menuBar->addMenu(pmFile);
    ui->menuBar->addMenu(pmHelp);
}

void MainWindow::RefreshLists() {
    pListDirs->clear();
    QDir dir(CurrentDirName);
    QStringList subDirNames = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    pListDirs->addItem("..");
    pListDirs->addItems(subDirNames);

    pListFileNames->clear();
    subDirNames = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    pListFileNames->addItems(subDirNames);
}

void MainWindow::ChangeFont() {
    QFont font;
    font.setBold(pcbBold->isChecked());
    font.setItalic(pcbItalic->isChecked());
    font.setUnderline(pcbUnderlined->isChecked());

    pTextEdit->setFont(font);
    switch (pcbColor->currentIndex()) {
        case 0: pTextEdit->setTextColor(Qt::black); break;
        case 1: pTextEdit->setTextColor(Qt::green); break;
        case 2: pTextEdit->setTextColor(Qt::red); break;
        case 3: pTextEdit->setTextColor(Qt::gray); break;
        case 4: pTextEdit->setTextColor(Qt::blue); break;
    }
    pTextEdit->setText(pTextEdit->toPlainText());
}

void MainWindow::ResetFont() {
    pcbBold->setChecked(false);
    pcbItalic->setChecked(false);
    pcbUnderlined->setChecked(false);
    pcbColor->setCurrentIndex(0);
    ChangeFont();
}

void MainWindow::About() {
    QMessageBox::information(0, "Автор",
                             "Студент МАИ\n"
                             "Группа: М8О-213Б-17\n"
                             "ФИО: Семенов Сергей Дмитриевич");
}

void MainWindow::Help() {
    QMessageBox::information(0, "О программе",
                             "Программа открывает и редактирует текстовые файлы в кодировках \"cp1251\" и \"UTF-8\" с возможностью сохранения.\n");
}

void MainWindow::ChangeCurrDir(QListWidgetItem *pNewDir) {
    watcher.removePath(CurrentDirName);

    QDir move_dir(CurrentDirName);
    move_dir.cd(pNewDir->text());
    CurrentDirName = move_dir.absolutePath();
    pleCurrPath->setText(CurrentDirName);
    watcher.addPath(CurrentDirName);
    RefreshLists();
}

void MainWindow::OpenDlgFile() {
    QFileDialog dlg;
    QString Filename = dlg.getOpenFileName(0, "open string", "", "");
    if (!Filename.isEmpty()) {
        CurrentFileName = Filename;
        QDir dir(Filename);
        dir.cdUp();
        CurrentDirName = dir.absolutePath();
        pleCurrPath->setText(CurrentDirName);
        RefreshLists();
        OpenCurrentFile();
    }
}

void MainWindow::OpenCurrentFile() {
    if (CurrentFileName.length() < 1)
        return;

    QFile fin;
    fin.setFileName(CurrentFileName);
    if (!fin.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Ошибка", QString("Ошибка открытия файла!\nИмя файла: \"") + CurrentFileName + "\".");
        return;
    }

    QTextStream stream(&fin);

    if (prbCodeCP1251->isChecked()) {
        stream.setCodec(QTextCodec::codecForName("cp1251"));
    }
    else if (prbCodeUTF_8->isChecked()) {
        stream.setCodec(QTextCodec::codecForName("UTF-8"));
    }

    pTextEdit->setText(stream.readAll());

    fin.close();
}

void MainWindow::SaveDlgFile() {
    QFileDialog dlg;
    QString Filename = dlg.getSaveFileName(0, "save string", "", "");
    if (!Filename.isEmpty()) {
        CurrentFileName = Filename;
        QDir dir(Filename);
        dir.cdUp();
        CurrentDirName = dir.absolutePath();
        pleCurrPath->setText(CurrentDirName);
        RefreshLists();
        SaveCurrentFile();
    }
}

void MainWindow::SelectFile(QListWidgetItem *pFilename) {
    CurrentFileName = CurrentDirName + "/" + pFilename->text();
    OpenCurrentFile();
}

void MainWindow::SaveCurrentFile() {
    if (CurrentFileName.length() < 1)
        return;

    if (QMessageBox::question(0, "?", "Сохранить изменения?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        QFile fout;
        fout.setFileName(CurrentFileName);
        if (!fout.open(QFile::WriteOnly)) {
            QMessageBox::information(0, "Ошибка", QString("Ошибка открытия файла ") + "\"" + CurrentFileName + "\" на запись!");
            return;
        }

        QTextStream stream(&fout);

        if (prbCodeCP1251->isChecked()) {
            stream.setCodec(QTextCodec::codecForName("cp1251"));
            qDebug() << stream.codec()->name();
        }
        else {
            stream.setCodec(QTextCodec::codecForName("UTF-8"));
        }
        stream << pTextEdit->toPlainText();

        fout.close();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}




