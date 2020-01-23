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


    ui->deviceTableView->setModel(modelDevice);     // Встановлюєм модель на TableView
    ui->deviceTableView->setColumnHidden(0, true);    // Приховуємо колону id записів
    // Дозволяєм виділення стрічок
    ui->deviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Встановлюєм режим виділення лише однієї стрічки в таблиці
    ui->deviceTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Встановлюєм розмір колонок по вмісту
    ui->deviceTableView->resizeColumnsToContents();
    ui->deviceTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->deviceTableView->horizontalHeader()->setStretchLastSection(true);

    connect(ui->deviceTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord(QModelIndex)));
}

/* Слот оновлення моделі представлення данних
 * */
void MainWindow::slotUpdateModels()
{
    modelDevice->select();
}

/* Метод для активації діалугу додавання записів в режимі редагування
 * з передачюе індексу вибраної стрічки
 * */
void MainWindow::slotEditRecord(QModelIndex index)
{
    /* Також створюєм діалог і піключаємо його сигнал вавершення роботи
     * до слоту оновелння виду моделі представлення данних, але передаємо
     * в якості параметрів запису стрічки
     * */
    DialogAdder *addDeviceDialog = new DialogAdder(index.row());
    connect(addDeviceDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Виконуєм запуск діалогового вікна
     * */
    addDeviceDialog->setWindowTitle(tr("Редагувати запис"));
    addDeviceDialog->exec();
}

/* Метод для активації діалога додавання записів
 * */



void MainWindow::on_addDeviceButton_clicked()
{
    /* Створюєм діалог і підключаєм його согнал завершення роботи
     * до слоту оновлення виду моделі данних
     * */
    DialogAdder *adderDialog = new DialogAdder();
    connect(adderDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Виконуєм запуск дівлогового вікна
     * */
    adderDialog->setWindowTitle(tr("Додати запис"));
    adderDialog->exec();
}
