#ifndef DIALOGADDER_H
#define DIALOGADDER_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

#include <database.h>

namespace Ui {
class dialogAdder { };
}

class dialogAdder : public QDialog
{
    Q_OBJECT

public:
    explicit dialogAdder(int row = -1, QWidget *parent = 0);
    ~dialogAdder();

signals:
    void signalReady();

private slots:
    void on_buttonBox_accepted();
    void updateButtons(int row);

private:
    Ui::dialogAdder         *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;

private:
    void setupModel();
    void createUI();
    void accept();
};

#endif // DIALOGADDER_H
