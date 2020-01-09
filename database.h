#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QRegExpValidator>

#define DATABASE_HOSTNAME "ExampleDataBase"
#define DATABASE_NAME "DataBase.db" // ім'я файлу для SQLlite

#define TABLE "List"
#define TABLE_NAMES "Names" // П.І.П
#define TABLE_DATE "Date" // Дата штрафу
#define TABLE_SUM "Sum" // Сума штрафу
#define TABLE_CARNUMB "CarNumb" // Номер автомобіля
#define TABLE_MK "Mark" // Марка авто

//#define TABLE_MESSAGE "Message"
//#define TABLE_RANDOM "Random"

class DataBase : public QObject {
    Q_OBJECT
public:
    explicit DataBase(QObject *patern = 0);
    ~DataBase();
    void connectToDataBase();
    bool insertIntoTable(const QVariantList &data);

private:
    QSqlDatabase db;
private:
    bool openDataBase();
    bool restoreDataBase();
void closeDataBase();
bool createTable();
};
#endif
