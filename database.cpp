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
/*Методи відкриття бази*/
bool DataBase::openDataBase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_NAME);
    db.setDatabaseName("D:/example/" DATABASE_NAME);
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
    if(!query.exec("CREATE TABLE" TABLE "("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   TABLE_NAMES "NAMES    NOT NULL,"
                   TABLE_DATE "DATE     NOT NULL,"
                   TABLE_SUM "VARCHAR(255)    NOT NULL,"
                  /* TABLE_CARNUMB "INTEGER    NOT NULL,"
                   TABLE_MK "MARK      NOT NULL" */
                   " )"
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
    query.prepare("INSERT INTO" TABLE "( "
                  TABLE_NAMES ", "
                  TABLE_DATE ", "
                  TABLE_SUM ", "
                 /* TABLE_CARNUMB ", "
                  TABLE_MK" ) " */
                                "VALUES (:Names ,:Date ,:Sum)"
                  );
    query.bindValue(":Names", data[0].toString());
    query.bindValue(":Date", data[1].toDate());
    query.bindValue(":Sum", data[2].toInt());
//    query.bindValue(":CarNumb", data[3].toInt());
//    query.bindValue(":Mark", data[4].toString());


    if(!query.exec()) {
        qDebug() << "error insert into" << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
