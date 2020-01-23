#include "adder.h"
#include "ui_dialogAdder.h"
#include <QDebug>

AddDevice::AddDevice(int row, QWidget *parent) : QDialog(parent),
    ui(new Ui::AddDevice) {

    ui->setupUi(this);

    /* Методи для ініціалізації моделі,
     * з якої будуть транслюватись данні
     * */
    setupModel();

    /* Якщо стрічка не задана, тобто рівна -1,
     * тоді діалог працює по принципу створення нового запису.
     * А а саме, в модель вставляєтся нова стрічка і робота ведеться з нею.
     * */
    if(row == -1){
        model->insertRow(model->rowCount(QModelIndex()));
        mapper->toLast();
    /* В іншому випадку діалог налаштовується на заданий запис
     * */
    } else {
        mapper->setCurrentModelIndex(model->index(row,0));
    }

    createUI();
}

AddDevice::~AddDevice()
{
    delete ui;
}

/* Метод налаштування моделі данних і mapper
 * */
void AddDevice::setupModel()
{
    /* Ініціалізуєм модель і робимо вибірку з неї
     * */
    model = new QSqlTableModel(this);
    model->setTable(DEVICE);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    /* Ініціалізуєм mapper і привязуємо
     * поля данних до обєктів LineEdit
     * */
    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->HostnameLineEdit, 1);
    mapper->addMapping(ui->IPAddressLineEdit, 2);
    mapper->addMapping(ui->MACLineEdit, 3);
    /* Ручне підтвердження зміни данних
     * через mapper
     * */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    qDebug() << ui->HostnameLineEdit->text();
    /* Подключаем коннекты от кнопок пролистывания
     * к прилистыванию модели данных в mapper
     * */
    connect(ui->previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    /* При изменении индекса в mapper изменяем состояние кнопок
     * */
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));
}

/* Метод для установки валидатора на поле ввода IP и MAC адресов
 * */
void AddDevice::createUI()
{
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->IPAddressLineEdit->setValidator(ipValidator);

    QString macRange = "(?:[0-9A-Fa-f][0-9A-Fa-f])";
    QRegExp macRegex ("^" + macRange
                      + "\\:" + macRange
                      + "\\:" + macRange
                      + "\\:" + macRange
                      + "\\:" + macRange
                      + "\\:" + macRange
                      + "\\:" + macRange + "$");
    QRegExpValidator *macValidator = new QRegExpValidator(macRegex, this);
    ui->MACLineEdit->setValidator(macValidator);
}

//void AddDevice::on_buttonBox_accepted()
//{
//    /* SQL-запрос для проверки существования записи
//     * с такими же учетными данными.
//     * Если запись не существует или находится лишь индекс
//     * редактируемой в данный момент записи,
//     * то диалог позволяет вставку записи в таблицу данных
//     * */
//    QSqlQuery query;
//    QString str = QString("SELECT EXISTS (SELECT " DEVICE_NAMES " FROM " DEVICE
//                          " WHERE ( " DEVICE_NAMES " = '%1' "
//                          " OR " DEVICE_DATE " = '%2' )"
//                          " AND id NOT LIKE '%3' )")
//            .arg(ui->HostnameLineEdit->text(),
//                 ui->IPAddressLineEdit->text(),
//                 model->data(model->index(mapper->currentIndex(),0), Qt::DisplayRole).toString());

//    query.prepare(str);
//    query.exec();
//    query.next();

//    /* Если запись существует, то диалог вызывает
//     * предупредительное сообщение
//     * */
//    if(query.value(0) != 0){
//        QMessageBox::information(this, trUtf8("Ошибка хоста"),
//                                 trUtf8("Хост с таким именем или IP-адресом уже существует"));
//    /* В противном случае производится вставка новых данных в таблицу
//     * и диалог завершается с передачей сигнала для обновления
//     * таблицы в главном окне
//     * */
//    } else {
//        mapper->submit();
//        model->submitAll();
//        emit signalReady();
//        this->close();
//    }
//}

void AddDevice::accept()
{
qDebug() << ui->HostnameLineEdit->text();
}

/* Метод изменения состояния активности кнопок пролистывания
 * */
void AddDevice::updateButtons(int row)
{
    /* В том случае, если мы достигаем одного из крайних (самый первый или
     * самый последний) из индексов в таблице данных,
     * то мы изменяем состояние соответствующей кнопки на
     * состояние неактивна
     * */
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
}
