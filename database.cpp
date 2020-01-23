#include "database.h"

DataBase::DataBase(QObject *patern) : QObject(patern) {
}
DataBase::~DataBase(){

}
/*
 * Методи підключення до бази данних*/
void DataBase::connectToDataBase(){
    /*Перевірка на існування бази данних перед підключенням*/

    if(!QFile("C:/example/" DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}
/*
Відновлення базид данних*/
bool DataBase::restoreDataBase() {
    if(this->openDataBase()){
        if(!this->createTable()){
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Can't reanimate database";
        return false;
    }
    return false;
}
/*Методи відкриття бази
*/
bool DataBase::openDataBase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_NAME);
    db.setDatabaseName("С/example/" DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase() {
    db.close();
}
bool DataBase::createTable() {
    QSqlQuery query;
    if(!query.exec("CREATE TABLE " TABLE " ("
                   "id INT(11) NOT NULL AUTO_INCREMENT, "
                   TABLE_NAMES "VARCHAR(50)  NOT NULL,"
                   TABLE_DATES "DATE     NOT NULL,"
                   TABLE_SUMA "INT(20)  NOT NULL,"
                   "PRIMARY KEY(id)"
                   " );"
                   )){
        qDebug() << "DataBase error of cteate " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
bool DataBase::insertIntoTable(const QVariantList &data) {
    QSqlQuery query;
    query.prepare("INSERT INTO" DEVICE "( "
                  DEVICE_NAMES ", "
                  DEVICE_DATES ", "
                  DEVICE_SUMA ", "
                              " ) "
                              "VALUES (:Names ,:Dates ,:Suma);"  );
    query.bindValue(":Names", data[0].toString());
    query.bindValue(":Dates", data[1].toDate());
    query.bindValue(":Suma", data[2].toInt());


    if(!query.exec()) {
        qDebug() << "error insert into" << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
