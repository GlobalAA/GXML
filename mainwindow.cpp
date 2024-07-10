#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui/QFontMetricsF>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QShortcut *updateKey = new QShortcut(QKeySequence(Qt::Key_F5), this);
    QShortcut *saveKey = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    QShortcut *saveAsKey = new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S), this);
    QShortcut *closeKey = new QShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C), this);

    connect(updateKey, &QShortcut::activated, this, &MainWindow::updateData);
    connect(saveKey, &QShortcut::activated, this, &MainWindow::saveKey);
    connect(saveAsKey, &QShortcut::activated, this, &MainWindow::saveAsKey);
    connect(closeKey, &QShortcut::activated, this, &MainWindow::closeKey);

    ui->jsonText->setTabStopDistance(QFontMetricsF(ui->jsonText->font()).horizontalAdvance(' ') * 4);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open xml file"),
        tr("/home"),
        tr("XML files (*.xml)")
    );
    this->filePath = filePath;
    QFile file(filePath);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Error opening file"), tr("Error opening file"));
        return;
    }

    QTextStream in(&file);

    pugi::xml_parse_result result = this->doc.load_file(filePath.toStdString().c_str());

    if (!result) {
        QMessageBox::critical(this, tr("Error parsing XML"), tr("Error parsing XML file"));
        return;
    }

    json jsonObj;

    this->gx_file.xml_to_json(this->doc, jsonObj);

    this->ui->xmlText->setPlainText(in.readAll());
    this->ui->jsonText->setPlainText(QString::fromStdString(jsonObj.dump(2)));

    file.close();
}

void MainWindow::updateData() {
    QString jsonText = this->ui->jsonText->toPlainText();
    json jsonData;

    try {
        jsonData = json::parse(jsonText.toStdString());
    }
    catch (nlohmann::json_abi_v3_11_3::detail::parse_error) {
        QMessageBox::critical(this, tr("Json error"), tr("Parsing error"));
        return;
    }
    catch (nlohmann::json_abi_v3_11_3::detail::type_error) {
        QMessageBox::critical(this, tr("Json error"), tr("Type error"));
        return;
    }

    this->doc.reset();
    pugi::xml_node root = this->doc.append_child("root");

    this->gx_file.json_to_xml(jsonData, root);

    std::ostringstream oss;
    for (pugi::xml_node_iterator it = this->doc.begin(); it != this->doc.end(); ++it)
        it->print(oss, " ");
    QString xml = QString::fromStdString(oss.str());

    this->ui->xmlText->setPlainText(xml);
    this->ui->jsonText->setPlainText(QString::fromStdString(jsonData.dump(4)));
}

void MainWindow::saveKey() {
    if (!this->doc.save_file(this->filePath.toStdString().c_str(), " ")) {
        QMessageBox::critical(this, tr("File saving error"), tr("File saving error"));
        return;
    }
}

void MainWindow::closeKey() {
    if (!this->doc.empty()) {
        this->doc.reset();
    }
    if (this->filePath.length() > 0) {
        this->filePath = "";
    }
    this->ui->xmlText->setPlainText("");
    this->ui->jsonText->setPlainText("");
}

void MainWindow::saveAsKey() {
    QString saveFilePath = QFileDialog::getSaveFileName(
        this,
        tr("Open xml file"),
        tr("/home"),
        tr("XML files (*.xml)")
        );

    QFile file(saveFilePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << this->ui->xmlText->toPlainText() << Qt::endl;
        return;
    }
    QMessageBox::critical(this, tr("Error saving file"), tr("Error open file for save"));
}

void MainWindow::on_actionSave_triggered()
{
    this->saveKey();
}


void MainWindow::on_actionClose_triggered()
{
    this->closeKey();
}


void MainWindow::on_actionSave_As_triggered()
{
    this->saveAsKey();
}

