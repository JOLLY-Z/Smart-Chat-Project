#ifndef CHECKPASSWORD_H
#define CHECKPASSWORD_H

#include <QDialog>

namespace Ui {
class checkpassword;
}

class checkpassword : public QDialog
{
    Q_OBJECT

public:
    explicit checkpassword(QWidget *parent = nullptr);
    ~checkpassword();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::checkpassword *ui;
};

#endif // CHECKPASSWORD_H
