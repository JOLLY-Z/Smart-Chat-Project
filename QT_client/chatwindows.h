#ifndef CHATWINDOWS_H
#define CHATWINDOWS_H

#include <QWidget>
#include<my_tcpsocket.h>
#include<QString>
#include<QTextEdit>
#include<QTextCursor>
#include<QTextBlockFormat>
#include<QTextCharFormat>
#include<QVBoxLayout>
#include<QMap>
#include<memory>
#include"airecv.h"
#include"showMessage.h"
#include"video_call/start_videocall.h"



class TextEditorWindow;


namespace Ui {
class chatWindows;
}

class chatWindows : public QWidget
{
    Q_OBJECT

public:
    explicit chatWindows(QWidget *parent = nullptr);
    ~chatWindows();
    QString strId;
    void to_show_friendmessage(QString username,QString message);
    void to_show_ai_recv(QString str);
    void displayDoc(QString information);

    void addMessage(const QString& text, bool isOutgoing,QString usernmae="");


private slots:
    void on_pushButton_clicked();


    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_pushButton_2_clicked();


    void on_toolButton_4_clicked();


    void on_toolButton_5_clicked();

private:
    Ui::chatWindows *ui;
    my_TcpSocket *m_tcpsocket;
    QMap<int,QString> m;
    int i=-1;
    QString ai_to_recv;//用于智能回复
    bool isTansrom=true;
    ApiHandler *handler;

    // QTextCharFormat *textCharFormat_m;
    // QTextCharFormat *textCharFormat_h;
    std::unique_ptr<TextEditorWindow> textEdit;
    int flag=0;
    QColor color;
    QFont font;
    int fontsize=0;

    int chatSize=60;

    //获得视频通话类型的指针
    start_videoCall *m_s;


};



#include <QWidget>
#include <QTextEdit>
#include <QFontComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QTextCursor>
#include <QTextCharFormat>

class TextEditorWindow : public QDialog {
    Q_OBJECT

public:
    TextEditorWindow(QWidget *parent = nullptr) : QDialog(parent) {
        // 创建控件
        textEdit = new QTextEdit(this);
        textEdit->setPlaceholderText("选好字体格式以后在此测试");
        fontComboBox = new QFontComboBox(this);
        fontSizeSpinBox = new QSpinBox(this);
        fontSizeSlider = new QSlider(Qt::Horizontal, this);
        colorButton = new QPushButton("选择文字颜色", nullptr);

        // 设置字体大小
        fontSizeSpinBox->setRange(1, 100);
        fontSizeSpinBox->setValue(28);

        fontSizeSlider->setRange(1, 100);
        fontSizeSlider->setValue(28);

        // 布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(fontComboBox);
        layout->addWidget(fontSizeSpinBox);
        layout->addWidget(fontSizeSlider);
        layout->addWidget(colorButton);
        layout->addWidget(textEdit);

        // 信号与槽
        connect(fontComboBox, &QFontComboBox::currentFontChanged, this, &TextEditorWindow::updateFont);
        connect(fontSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TextEditorWindow::updateFontSize);
        connect(fontSizeSlider, &QSlider::valueChanged, this, &TextEditorWindow::updateFontSize);
        connect(colorButton, &QPushButton::clicked, this, &TextEditorWindow::chooseColor);

        // 初始化界面
        updateFont();
        updateFontSize();

    }
    ~TextEditorWindow(){

    }
    QFont font;
    int fontsize;
    QColor color;


private slots:
    void updateFont() {
        font = fontComboBox->currentFont();
        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format;
        format.setFont(font);
        cursor.mergeCharFormat(format);
        textEdit->setTextCursor(cursor);
    }

    void updateFontSize() {
        this->fontsize= fontSizeSpinBox->value();
        if (this->fontsize != fontSizeSlider->value()) {
            fontSizeSlider->setValue(this->fontsize);
        }
        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format;
        format.setFontPointSize(fontsize);
        cursor.mergeCharFormat(format);
        textEdit->setTextCursor(cursor);
    }

    void chooseColor() {

        this->color = QColorDialog::getColor(Qt::black, nullptr, "选择文字颜色");
        if (color.isValid()) {
            QTextCursor cursor = textEdit->textCursor();
            QTextCharFormat format;
            format.setForeground(color);
            cursor.mergeCharFormat(format);
            textEdit->setTextCursor(cursor);
        }
    }

private:
    QTextEdit *textEdit;
    QFontComboBox *fontComboBox;
    QSpinBox *fontSizeSpinBox;
    QSlider *fontSizeSlider;
    QPushButton *colorButton;

};

#endif // CHATWINDOWS_H


