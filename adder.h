#ifndef ADDER_H
#define ADDER_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

#include <database.h>

namespace Ui {
class AddDevice;
}

class AddDevice : public QDialog
{
    Q_OBJECT

public:
    explicit AddDevice(int row = -1, QWidget *parent = 0);
    ~AddDevice();

signals:
    void signalReady();

private slots:
    void on_buttonBox_accepted();
    void updateButtons(int row);

private:
    Ui::AddDevice         *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;

private:
    void setupModel();
    void createUI();
    void accept();
};

#endif // ADDER_H
