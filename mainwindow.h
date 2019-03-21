#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QString>
#include <QListWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QMenu>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QFileSystemWatcher>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

#include <QFile>
#include <QTextStream>
#include <QTextCodec>


#include <QLabel>

#include <QPushButton>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void SetWidgets();
    void SetMenu();




    Ui::MainWindow *ui;
    QFileSystemWatcher watcher;

    QString CurrentDirName, CurrentFileName;
    QListWidget *pListDirs, *pListFileNames;
    QTextEdit *pTextEdit;
    QLineEdit *pleCurrPath;

    QRadioButton *prbCodeCP1251, *prbCodeUTF_8;
    QCheckBox *pcbBold, *pcbItalic, *pcbUnderlined;
    QComboBox *pcbColor;
private slots:
    void About();
    void Help();
    void ChangeFont();
    void ResetFont();
    void RefreshLists();

    void OpenDlgFile();
    void SaveCurrentFile();
    void SaveDlgFile();

    void ChangeCurrDir(QListWidgetItem *pNewDir);
    void OpenCurrentFile();
    void SelectFile(QListWidgetItem *pFilename);




};

#endif // MAINWINDOW_H
