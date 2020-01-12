#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"

#include <QMainWindow>
#include <QSqlTableModel>

#include <database.h>
#include <dialogAdder.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addDeviceButton_clicked();
    void slotUpdateModels();
    void slotEditRecord(QModelIndex index);

private:
    Ui::MainWindow              *ui;
    DataBase                    *db;
    QSqlTableModel              *modelDevice;

private:
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
};

#endif
