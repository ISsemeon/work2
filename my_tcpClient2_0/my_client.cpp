 #include "my_client.h"
#include "ui_my_client.h"

my_client::my_client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::my_client)
{


    ui->setupUi(this);
    ui2= new settings(this);
    this->host="";
    this->port=0;

    //установка кнопок(чтоб не начинать передачу без подключения)
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    //создание сокета для общения с клиентом
    client_socket =new QTcpSocket(this);

    connect(client_socket,&QTcpSocket::readyRead,this,&my_client::sockReady);//пришли данные от сервера
    connect(client_socket,&QTcpSocket::disconnected,this,&my_client::sockDisk);//отключение от сервера и закрытие программы
    connect(ui->pushButton_3,&QPushButton::released,this,&my_client::SLOTSending);//нажата кнопка начать общение с сервером
    connect(this,&my_client::newData,this,&my_client::view);//если пришли данные которых не было

   //

    Data="";
    type=typeoftransmottion::UNKNOWN;
    isTypeDataChanged=0;
    CurrentCommand=0;
    addforAllOperators=0;

    //установка дефолтных параметров таблицы
     model=new QStandardItemModel(this);


}

my_client::~my_client()
{
    delete ui;
    delete model;
}


void my_client::on_pushButton_clicked()
{
    host=ui2->get_host();
    port=ui2->get_port();
    type=ui2->getType();
    operator_to_transmittion=ui2->getOperator();



    if((host=="" )||( port== 0)||(type==typeoftransmottion::UNKNOWN))
    {
    QMessageBox::information(this,"Ошибка","Введите IP и Port!");
     on_actionSettings_triggered();
    }
    else if(QByteArray(client_socket->readAll()).size() > 0)
    {
    QMessageBox::information(this,"Ошибка","Соединение уже установлено");
    }
    else
    {
    client_socket->connectToHost(host,port);
    }


}

void my_client::sockDisk()
{
    QMessageBox::information(this,"Сообщение","Cоединенеие разорвано\nЗавершение программы");
    client_socket->close();
    //exit(1);

}

void my_client::sockReady()
{
    if(client_socket->waitForConnected(500))
    {

//считывание

    Data=client_socket->readAll();

    if(Data=="__ready")
    {
        QMessageBox::information(this,"Сообщение","Соединение успешно установлено");
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton->setEnabled(false);

    }
    else if(Data.contains("__radd"))
    {
        parse();
        sendData();
    }
    }
}


void my_client::on_pushButton_2_clicked()
{

    if(client_socket->waitForConnected(500))
    {

        client_socket->write("__close");//close

        ui->pushButton_2->setEnabled(false);

    }
}


void my_client::SLOTSending()
{
host=ui2->get_host();
port=ui2->get_port();
if(type!=ui2->getType())
{
    isTypeDataChanged=1;
    type=ui2->getType();
}


ui->pushButton_2->setEnabled(true);
ui->pushButton_3->setEnabled(false);
sendData();

}
void my_client::sendData()//в зависимости от параметров отправляет данные и принимает их по особенному
{
    if(operator_to_transmittion==client_operator::ALLOPERATORS)
    {
    addforAllOperators++;
    if(addforAllOperators==7)
    {
        addforAllOperators=1;
    }
    }
    if((client_socket->isOpen()) && (client_socket->waitForConnected(500)))
      {
        switch (type) {
        case typeoftransmottion::GSM:
        {
            client_socket->write(VectorTosenD.at(0));
            client_socket->write(QString().number(operator_to_transmittion+addforAllOperators).toUtf8());
            client_socket->waitForBytesWritten(500);
            break;
        }
        case typeoftransmottion::LTE:
        {

            client_socket->write(VectorTosenD.at(1));

            client_socket->write(QString().number(operator_to_transmittion+addforAllOperators).toUtf8());
            client_socket->waitForBytesWritten(500);
            break;
        }
        case typeoftransmottion::UTMS:
        {

            client_socket->write(VectorTosenD.at(2));
           client_socket->write(QString().number(operator_to_transmittion+addforAllOperators).toUtf8());
            client_socket->waitForBytesWritten(500);
            break;
        }
        case typeoftransmottion::ALL:
        {

            client_socket->write(VectorTosenD.at(3));
           client_socket->write(QString().number(operator_to_transmittion+addforAllOperators).toUtf8());
            client_socket->waitForBytesWritten(500);
            break;
        }
        }


    }
}
void my_client::parse()
{
        model->setHorizontalHeaderLabels(QStringList()<<"Type"<<"Country"<<"Channel"<<"Operator"<<"LAC"<<"CID"<<"Power(dBm)");
        QStringList strlist=QString(Data).remove("__radd").split("\n");//парсим строчку
        qDebug()<<Data;
        strlist.pop_back();
        for(auto substr:strlist)
        {
            //выделяем id базовой станции и добавляем их в вектор если он не содержит данного id таким образом мы делаем уникальным вывод
            int id=substr.section(',',5,5).toInt();

            if(((!myIDvec.contains(id))||(!ui2->isUnicvalues()))&&id!=0)
            {
                myIDvec.push_back(id);

                int column=0;//для таблицы
                int i=0;//для дерева
                QTreeWidgetItem *wgt=new QTreeWidgetItem(QStringList(substr.section(',',i,i)));//создание родителя в дереве
                i++;
                for(auto &buf:substr.split(','))
            {
                switch (i) {
                case 1:
                {
                    wgt->addChild(new QTreeWidgetItem(QStringList(QString("Country: ")+substr.section(',',i,i))));
                    break;
                }
                case 2:
                {
                    wgt->addChild(new QTreeWidgetItem(QStringList(QString("Channel: ")+substr.section(',',i,i))));
                    break;
                }
                case 3:
                {
                    wgt->addChild(new QTreeWidgetItem(QStringList(QString("Operator: ")+substr.section(',',i,i))));
                    break;
                }
                case 4:
                {
                    wgt->addChild(new QTreeWidgetItem(QStringList(QString("Location: " )+substr.section(',',i,i))));
                    break;
                }
                case 5:
                {
                    wgt->addChild(new QTreeWidgetItem(QStringList(QString("CID: ")+substr.section(',',i,i))));
                    break;
                }
                case 6:
                {
                    wgt->addChild(new QTreeWidgetItem(QStringList(QString("Power(dBm): ")+substr.section(',',i,i))));
                    break;
                }
                }
                i++;
            }
                ui->treeWidget->addTopLevelItem(wgt);//установка дерев

                for(auto values:substr.split(','))
                {
                    model->setItem(myIDvec.count()-1,column,new QStandardItem(values));
                    column++;
                }
                emit newData();
            }
        }

        if(!ui->pushButton_2->isEnabled())
        {
        ui->pushButton_3->setEnabled(true);
        }
  }

void my_client::on_actionSettings_triggered()
{
ui2->show();
}

void my_client::view()
{
 ui->tableView->setModel(model);
 ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


