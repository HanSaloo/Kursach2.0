#include "dialogAdder.h"
#include "ui_dialogAdder.h"
#include <QDebug>

DialogAdder :: DialogAdder(int row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAdder) {

    ui->setupUi(this);

    /* Методу для ініціалізації моделі,
     * з якої будуть транслюватися дані
     * */
    setupModel();

    /* Якщо рядок не заданий, то дорівнює -1,
     * тогда диалог работает по принципу создания новой записи.
     * А саме, в модель вставляється новий рядок і робота ведеться з нею.
     * */
    if(row == -1){
        model->insertRow(model->rowCount(QModelIndex()));
        mapper->toLast();
        /* В іншому випадку діалог налаштовується на заданий запис.
     * */
    } else {
        mapper->setCurrentModelIndex(model->index(row,0));
    }
}

DialogAdder::~DialogAdder()
{
    delete ui;
}

/* Метод вимірювання моделі даних і mapper
 * */
void DialogAdder::setupModel()
{
    /* Ініціалізіруєм модель і робимо вибірку з неї
     * */
    model = new QSqlTableModel(this);
    model->setTable(DEVICE);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    /* Ініціалізіруєм mapper і прив'язуємо до неї
     * поля даних до об'єктів LineEdit
     * */
    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->NamesLineEdit, 1);
    mapper->addMapping(ui->DateLineEdit, 2);
    mapper->addMapping(ui->SumLineEdit, 3);
    /* Ручне підтвердження зміни даних
     * через mapper
     * */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    qDebug() << ui->NamesLineEdit->text();
    /*  Підключаємо коннект від кнопок гортання
     * до гортання моделі даних в mapper
     * */
    connect(ui->previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    /* При зміні індекса в mapper змінюєм стан клавіш
     * */
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));
}


void DialogAdder::on_buttonBox_accepted()
{
    /* SQL-запит для перевірки існування запису    * з такими ж обліковими даними.
     * Якщо запис не існує або знаходиться лише індекс
     * редагованого в даний момент записи,
     * то діалог дозволяє вставку записи в таблицю даних
     * */
    QSqlQuery query;
    QString str = QString( "SELECT EXISTS (SELECT " DEVICE_NAMES " FROM " DEVICE
                           " WHERE ( " DEVICE_NAMES " = '%1' "
                                                    " OR " DEVICE_DATES " = '%2' )"
                                                                        " AND id NOT LIKE '%3' )" )
            .arg(ui->NamesLineEdit->text(),
                 ui->DateLineEdit->text(),
                 model->data(model->index(mapper->currentIndex(),0), Qt::DisplayRole).toString());

    query.prepare(str);
    query.exec();
    query.next();
    /* Якщо запис існує, то діалог викликає
     * попереджувальне повідомлення
     * */
   /*  if(query.value(0) != 0){
        QMessageBox::information(this, tr("Помилка хоста"),
                                 tr("Хост з таким ім'ям вже існує"));
        /* В іншому випадку проводиться вставка нових даних в таблицю
     * і діалог завершується з передачею сигналу для оновлення
     * таблиці в головному вікні
     *
    } else { */
        mapper->submit();
        model->submitAll();
        emit signalReady();
        this->close();
    }
//}


void DialogAdder::accept()
{
    qDebug() << ui->NamesLineEdit->text();
}

/* Метод зміни стану активності клавіш гортання
 * */
void DialogAdder::updateButtons(int row)
{
    /* У тому випадку, якщо ми досягаємо одного з крайніх (найперший або
     * останій) з індексів в таблиці даних,  
  * то ми змінюємо стан відповідної клавіши на
     * стан не активна
     * */
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
}
