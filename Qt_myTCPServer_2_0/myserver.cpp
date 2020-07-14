#include "myserver.h"
myserver::myserver()
{
    settings= new QSettings("settings.ini",QSettings::IniFormat,this);

    loadSettings();

    serial1.found_com_port(COM_port1_name);
    serial2.found_com_port(COM_port2_name);
    serial3.found_com_port(COM_port3_name);
    serial4.found_com_port(COM_port4_name);


    start_server();

    savesettings();

    qDebug()<<"port:"<<port1;
}

myserver::~myserver()
{
}

void myserver::start_server()
{
if((this->listen(QHostAddress::Any,port1)))
{
    qDebug()<<"listening";
}
else
{
    qDebug()<<"not listening";
}
}

void myserver::incomingConnection(int socket_Descriptor)
{

    socket=new QTcpSocket(this);
    socket->setSocketDescriptor(socket_Descriptor);
    qDebug()<<socket_Descriptor<<"client connected";

   connect(socket,&QTcpSocket::readyRead,this,&myserver::sockReady);
   connect(socket,&QTcpSocket::disconnected,this,&myserver::sockDisk);

   socket->write("__ready");
  }

void myserver::sockReady()
{

if(socket->waitForConnected(500))
{
    socket->waitForReadyRead(500);
    Data="";
    Data=QString(socket->readAll());

    //считываение запроса(оператор и тип запроса)
    QByteArray first_request;
    QByteArray second_request;
    QByteArray third_request;


    if(Current_operator!=Data.toInt()%10)
    {
        serial1.changeOperator(true);
        serial2.changeOperator(true);
        serial3.changeOperator(true);
        serial4.changeOperator(true);
        Current_operator=Data.toInt()%10;
    }


    int Current_request=Data.toInt()/10;


    qDebug()<<"Current_operator"<<Current_operator<<"Current_request"<<Current_request;

    serial1.convert_operator(Current_operator);
    serial2.convert_operator(Current_operator);
    serial3.convert_operator(Current_operator);
    //serial4.convert_operator(Current_operator);



    if(Data=="__close")
    {

        sockDisk();      
    }
    if(Current_request==1)//GSM
    {
        socket->write("__radd");
        socket->write(serial1.write_Data(typeoftransmottion::GSM));
        socket->write(serial2.write_Data(typeoftransmottion::GSM));
        socket->write(serial3.write_Data(typeoftransmottion::GSM));


        //socket->write(serial4.write_Data(typeoftransmottion::GSM));
    }
    else if(Current_request==2)//LTE
    {
        socket->write("__radd");
        socket->write(serial1.write_Data(typeoftransmottion::LTE));

        socket->write(serial2.write_Data(typeoftransmottion::LTE));

        socket->write(serial3.write_Data(typeoftransmottion::LTE));


      //socket->write(serial4.write_Data(typeoftransmottion::LTE));
    }
     else if(Current_request==4)//ALL
    {
        socket->write("__radd");
        for(int i=0;i<20;i++)
        {
         socket->write(serial1.write_Data(typeoftransmottion::GSM));
         QThread::msleep(500);
        }
        socket->write(serial2.write_Data(typeoftransmottion::LTE));
        socket->write(serial3.write_Data(typeoftransmottion::UTMS));

    }
    else if(Current_request==3)//UTMS
    {
        socket->write("__radd");
        socket->write(serial1.write_Data(typeoftransmottion::UTMS));
        socket->write(serial2.write_Data(typeoftransmottion::UTMS));
        socket->write(serial3.write_Data(typeoftransmottion::UTMS));

       // socket->write(serial4.write_Data(typeoftransmottion::UTMS));
        /**/
    }

}

}


void myserver::sockDisk()
{
qDebug()<<"disconected";
socket->deleteLater();
}

void myserver::loadSettings()
{
  //подключение 1й антенки (загрузка из файла)
  this->port1=settings->value("port_number",4444).toInt();
  this->COM_port1_name=settings->value("com_port_name1","COM13").toString();
  this->COM_port2_name=settings->value("com_port_name2","COM29").toString();
  this->COM_port3_name=settings->value("com_port_name3","COM3").toString();
  this->COM_port4_name=settings->value("com_port_name4","COM7").toString();




}

void myserver::savesettings()
{
settings->setValue("port_number",port1);
settings->setValue("com_port_name1",COM_port1_name);
settings->setValue("com_port_name2",COM_port2_name);
settings->setValue("com_port_name3",COM_port3_name);
settings->setValue("com_port_name4",COM_port4_name);

}







