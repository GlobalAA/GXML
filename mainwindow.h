#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstring>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <sstream>
#include "pugixml.hpp"
#include "gxjson.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionClose_triggered();

    void on_actionSave_As_triggered();

private:
    Ui::MainWindow *ui;
    pugi::xml_document doc;
    gxjson gx_file;
    QString filePath;

    void saveKey();
    void saveAsKey();
    void closeKey();
    void updateData();
};
#endif // MAINWINDOW_H
