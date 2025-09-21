#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>
#include<QString>
namespace Ui {
class addfriend;
}

class addfriend : public QDialog
{
    Q_OBJECT

public:
    explicit addfriend(QWidget *parent = nullptr);
    QString returnStr();
    ~addfriend();

signals:
    void transformStr(QString str);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::addfriend *ui;
};

#endif // ADDFRIEND_H
