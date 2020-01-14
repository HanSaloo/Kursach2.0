#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new DataBase();
    db->connectToDataBase();

    for(int i = 0; i < 4; i++) {
        QVariantList data;
        int random = qrand();
        data.append(QDate::currentDate());
        data.append(QTime::currentTime());

        data.append(random);

        data.append("Get msg of " + QString::number(random));

        db->insertIntoTable(data);
    }
    this->setupModel(TABLE,
                     QStringList()
                     << tr("id")
                     << tr("Names")
                     << tr("Date")
                     << tr("Sum")

                     );
    this->createUI();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupModel(const QString &tableName, const QStringList &headers) {
    modelDevice = new QSqlTableModel(this);
    modelDevice->setTable(tableName);

    for (int i = 0,j = 0;i < modelDevice->columnCount(); i++, j++) {
        modelDevice->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    modelDevice->setSort(0,Qt::AscendingOrder);
}

void MainWindow::createUI() {
    ui->tableView->setModel(modelDevice);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    modelDevice->select();


}

