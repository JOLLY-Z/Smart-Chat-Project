#ifndef AIRECV_H
#define AIRECV_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

enum class aiModel{
    commonAi,
    deepseek_r1,
    deepseek_v3
};

class ApiHandler : public QObject
{
    Q_OBJECT

public:
    explicit ApiHandler(QObject *parent = nullptr);
    void startProcess(aiModel aimodel);
    void setProblem(QString str);

signals:
    void reciveString(QString str);


private:
    QNetworkAccessManager *manager;
    QString accessToken;

    void getAccessToken();
    void handleTokenResponse(QNetworkReply *reply);
    void sendChatRequest();
    void handleChatResponse(QNetworkReply *reply);
    void deepseek_r1(QString model);
    QString problem;
};



#endif // AIRECV_H
