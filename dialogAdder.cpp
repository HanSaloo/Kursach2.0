#include "dialogAdder.h"
#include "ui_dialogAdder.h"
#include <QDebug>

DialogAdder :: DialogAdder(int row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAdder) {

    ui->setupUi(this);

    /* Метода для инициализации модели,
     * из которой будут транслироваться данные
     * */
    setupModel();

    /* Если строка не задана, то есть равна -1,
     * тогда диалог работает по принципу создания новой записи.
     * А именно, в модель вставляется новая строка и работа ведётся с ней.
     * */
    if(row == -1){
        model->insertRow(model->rowCount(QModelIndex()));
        mapper->toLast();
    /* В противном случае диалог настраивается на заданную запись
     * */
    } else {
        mapper->setCurrentModelIndex(model->index(row,0));
    }

}

DialogAdder::~DialogAdder()
{
    delete ui;
}

/* Метод настройки модели данных и mapper
 * */
void DialogAdder::setupModel()
{
    /* Инициализируем модель и делаем выборку из неё
     * */
    model = new QSqlTableModel(this);
    model->setTable(DEVICE);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    /* Инициализируем mapper и привязываем
     * поля данных к объектам LineEdit
     * */
    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->NamesLineEdit, 1);
    mapper->addMapping(ui->DateLineEdit, 2);
    mapper->addMapping(ui->SumLineEdit, 3);
    /* Ручное подтверждение изменения данных
     * через mapper
     * */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    qDebug() << ui->NamesLineEdit->text();
    /* Подключаем коннекты от кнопок пролистывания
     * к прилистыванию модели данных в mapper
     * */
    connect(ui->previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    /* При изменении индекса в mapper изменяем состояние кнопок
     * */
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));
}


void DialogAdder::on_buttonBox_accepted()
{
    /* SQL-запрос для проверки существования записи
     * с такими же учетными данными.
     * Если запись не существует или находится лишь индекс
     * редактируемой в данный момент записи,
     * то диалог позволяет вставку записи в таблицу данных
     * */
    QSqlQuery query;
    QString str = QString("SELECT EXISTS (SELECT " DEVICE_NAMES " FROM " DEVICE
                          " WHERE ( " DEVICE_NAMES " = '%1' "
                          " OR " DEVICE_DATES " = '%2' )"
                          " AND id NOT LIKE '%3' )")
            .arg(ui->NamesLineEdit->text(),
                 ui->DateLineEdit->text(),
                 model->data(model->index(mapper->currentIndex(),0), Qt::DisplayRole).toString());

    query.prepare(str);
    query.exec();
    query.next();
   /* Если запись существует, то диалог вызывает
     * предупредительное сообщение
     * */
    if(query.value(0) != 0){
        QMessageBox::information(this, tr("Ошибка хоста"),
                               tr("Хост с таким именем или IP-адресом уже существует"));
    /* В противном случае производится вставка новых данных в таблицу
     * и диалог завершается с передачей сигнала для обновления
     * таблицы в главном окне
     * */
    } else {
        mapper->submit();
        model->submitAll();
        emit signalReady();
        this->close();
    }
}



void DialogAdder::accept()
{
qDebug() << ui->NamesLineEdit->text();
}

/* Метод изменения состояния активности кнопок пролистывания
 * */
void DialogAdder::updateButtons(int row)
{
    /* В том случае, если мы достигаем одного из крайних (самый первый или
     * самый последний) из индексов в таблице данных,
     * то мы изменяем состояние соответствующей кнопки на
     * состояние неактивна
     * */
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
}
