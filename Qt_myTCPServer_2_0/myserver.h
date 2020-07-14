#ifndef MYSERVER_H
#define MYSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include "serial_port_class.h"
#include <QSettings>



class myserver:public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();
    void start_server();//проверка прослушивания порта и запуск сервера
    void incomingConnection(int socket_Descriptor);//входное соединение создание сокетного подключения
    void sockReady();//
    void sockDisk();
    void loadSettings();
    void savesettings();

private:
    QTcpSocket *socket;
    QString Data;
    Serial_port_class serial1;
    Serial_port_class serial2;
    Serial_port_class serial3;
    Serial_port_class serial4;

    qint16 port1;
    QString COM_port1_name;
    QString COM_port2_name;
    QString COM_port3_name;
    //для следующих реализаций
    QString COM_port4_name;

    QSettings * settings;

   int Current_operator;
};



#endif // MYSERVER_H
