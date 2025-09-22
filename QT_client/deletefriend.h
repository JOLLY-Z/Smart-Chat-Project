#ifndef DELETEFRIEND_H
#define DELETEFRIEND_H

#include <QDialog>

namespace Ui {
class deletefriend;
}

class deletefriend : public QDialog
{
    Q_OBJECT

public:
    explicit deletefriend(QWidget *parent = nullptr);
    ~deletefriend();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::deletefriend *ui;
};

#endif // DELETEFRIEND_H
