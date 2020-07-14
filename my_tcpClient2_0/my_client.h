#ifndef MY_CLIENT_H
#define MY_CLIENT_H


#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include "settings.h"






QT_BEGIN_NAMESPACE
namespace Ui { class my_client; }
QT_END_NAMESPACE

class my_client : public QMainWindow
{
    Q_OBJECT

public:
    my_client(QWidget *parent = nullptr);
    ~my_client();
    void sendData();
    void view();



public slots:
    void sockReady();
    void sockDisk();


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void SLOTSending();
    void on_actionSettings_triggered();



signals:
    void newData();

private:
    Ui::my_client *ui;
    settings *ui2;

    QTcpSocket *client_socket;

    QByteArray Data;


    quint16 port;
    QString host;
    QVector<int> myIDvec;
    QVector<QByteArray> VectorTosenD={"1","2","3","4"};//1-GSM//2-LTE//3-UTMS//4-ALL
    int CurrentCommand;
    int addforAllOperators;

    void parse();

    QStandardItemModel * model;
    typeoftransmottion type;
    bool isTypeDataChanged;
    client_operator operator_to_transmittion;




};
#endif // MY_CLIENT_H
