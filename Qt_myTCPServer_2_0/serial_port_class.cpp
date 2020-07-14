#include "serial_port_class.h"
#include<QDebug>
Serial_port_class::Serial_port_class()
{

}
void Serial_port_class::settings()
{
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->open(QIODevice::ReadWrite);
}

void Serial_port_class::found_com_port(QString port_name)
{
    for(auto port_info : QSerialPortInfo::availablePorts())
    {
        port=new QSerialPort(port_info);
        if(port_info.portName()==port_name)
        {
        settings();
       qDebug()<<"work"<<port_name;


        port->close();
        break;
        operatorchanged=true;
        }
    }
}

void Serial_port_class::changeOperator()
{

    port->write(current_request.toUtf8());
    port->write(current_request.toUtf8());
    port->waitForBytesWritten(-1);

    qDebug()<<"changeOPERAToR";
    qDebug()<<port->readAll();


}

void Serial_port_class::convert_operator(int oper)
{
    qDebug()<<oper;
    if(oper==1){current_request=QString("AT+cmssn=25001\r");}//mts
    else if(oper==2){current_request=QString("AT+cmssn=25002\r");}//megafon
    else if(oper==3){current_request=QString("AT+cmssn=25003\r");}//hcc
    else if(oper==4){current_request=QString("AT+cmssn=25004\r");}//smart
    else if(oper==5){current_request=QString("AT+cmssn=25020\r");}//tele2
    else if(oper==6){current_request=QString("AT+cmssn=25099\r");}//beline
    else{qDebug()<<"ALLOPEERATORS";}
    AddCommandsGPS();
    AddCommandsGSM();
    AddCommandsLTE();
    AddCommandsUTMS();

}

QByteArray convertoperator(int operator_value,bool &correct_operator)
{

    switch (operator_value) {
    case operatorsForconvert::_convertHCC:
    {
       return "HCC";
    }
    case operatorsForconvert::_convertMTS:
    {
        return "MTS";
    }
    case operatorsForconvert::_convertTELE2:
    {
         return "TELE2";
    }
    case operatorsForconvert::_convertSMART:
    {
         return "SMARTS";
    }
    case operatorsForconvert::_convertBEELINE:
    {
       return "BEELINE";
    }
    case operatorsForconvert::_convertMEGAFON:
    {
         return "MEGAFON";
    }
    default:
    {
        correct_operator=false;
        return "";
    }
}
}


QByteArray Serial_port_class::write_Data(typeoftransmottion type)
{





    port->open(QIODevice::ReadWrite);


    QByteArray buf;

    switch (type) {
    case typeoftransmottion::GSM:
    {

        if(operatorchanged)//сброс при смене оператора
        {
           changeOperator();
           operatorchanged=0;
        }

        for(auto &command: command_vectorGSM)
        {
            port->write(command.toUtf8());
            port->waitForBytesWritten(-1);
            port->waitForReadyRead(2000);
            buf+=port->readAll();
            qDebug()<<buf;

        }


        break;
    }
    case typeoftransmottion::LTE:
    {

        if(operatorchanged)//сброс при смене оператора
        {
            qDebug()<<"change operator";
           changeOperator();
           operatorchanged=0;
        }
        for(auto &command: command_vectorLTE)
        {

            port->write(command.toUtf8());

            port->waitForBytesWritten(-1);
            port->waitForReadyRead(-1);
            QThread::msleep(800);
            buf+=parse(port->readAll(),type);
            qDebug()<<buf;
        }
        break;
    }
    case typeoftransmottion::UTMS:
    {

        for(auto &command: command_vectorUTMS)
        {

            port->write(command.toUtf8());
            port->write(current_request.toUtf8());
            port->waitForReadyRead(2000);
            port->waitForReadyRead(-1);
            QThread::msleep(2300);
            buf+= port->readAll();
            qDebug()<<buf;

        }
        break;
    }
    case typeoftransmottion::GPS:
    {for(auto &command: command_vectorGPS)
        {
            port->write(command.toUtf8());
            QThread::msleep(2300);
            port->waitForBytesWritten(-1);
            port->waitForReadyRead(-1);

            buf+=port->readAll();//чтение с порта
        //        emit socket->readyRead();
        }
        break;
    }
    }


    qDebug()<<"++++++++++++++++++++++++++++++++++++++/n/n/n";
   port->close();
   if(type==typeoftransmottion::LTE)
   {
       return buf;
   }
   return parse(buf,type);//которая идет в метод parse который ее немного преобразует но на выходе так и так строка(массив байт)

}


QByteArray parse(QByteArray string, typeoftransmottion type)
{
    QByteArray buftoreturn="";
    bool correct_oper=true;
    qDebug()<<"\nparse\n";
    switch (type)
    {
        case typeoftransmottion::GSM:
    {
        QStringList strlist=QString(string).split(QRegExp("[\[]{1}[A-Za-z]{1,}CELL[0-9]{0,}[\]]{1},"));//парсим строчку
        strlist.pop_front();

        //тут строки
        qDebug()<<strlist;

        for(auto substring:strlist)
        {
            substring=substring.section('\r',0,0);
            substring=substring.section(',',0,6);
            substring.remove(QRegExp("[^0-9,-]"));

            buftoreturn+="GSM,";
            buftoreturn+=substring.section(',',1,1)+",";//код страны
            buftoreturn+=substring.section(',',0,0)+",";//канал
            buftoreturn+=convertoperator(substring.section(',',2,2).toInt(),correct_oper)+",";//имя оператора
            if(!correct_oper)
            {
                buftoreturn.clear();
                break;
            }
            buftoreturn+=substring.section(',',3,3)+",";//LAC
            buftoreturn+=substring.section(',',4,4)+",";//CID
            buftoreturn+=substring.section(',',6,6)+"dBm\n";//power
        }
        qDebug()<<"\nparse end\n";
        return buftoreturn;
        break;
    }
    case typeoftransmottion::UTMS:
    {

       QStringList strlist=QString(string).split(QRegExp("[\+]{1}CNSVSX: "));
       for(auto substr:strlist)
       {


            buftoreturn+="UTMS,";
            substr=substr.section('\r',0,0);
            qDebug()<<substr;
            buftoreturn+=substr.section(',',4,4).left(3)+",";
            buftoreturn+=substr.section(',',5,5)+",";
            buftoreturn+=convertoperator(substr.section(',',4,4).right(2).toInt(),correct_oper)+",";


            bool ok=true;
            buftoreturn+=QString::number(substr.section(',',3,3).right(4).toUInt(&ok,16))+",";//lac я


            int cid=substr.section(',',2,2).toInt();
            int rncid = cid / 65536;
            int cid2 = cid - rncid * 65536;

            buftoreturn+=(QString::number(cid2).toUtf8())+",";
            buftoreturn+="-"+substr.section(',',6,6)+"dBm\n";
            qDebug()<<buftoreturn;

       }
       qDebug()<<"\nparse end\n";

        return buftoreturn;
        break;
    }
    case typeoftransmottion::LTE:
    {

        QStringList strlist=QString(string).split(QRegExp("[\+]{1}CMGRMI: "));

        for(auto substr:strlist)
        {
            qDebug()<<substr;
            if(!substr.contains("Serving_Cell"))
            {
                continue;
            }
            else
            {
            qDebug()<<"contains///start parsing";
            substr=substr.section('\r',0,0);
            substr.remove("Serving_Cell");



             buftoreturn+="LTE,";

             buftoreturn+=substr.section(',',2,2)+",";//код страны

             buftoreturn+="-,";//канал

             bool correct_operator=1;
             buftoreturn+=convertoperator(substr.section(',',3,3).toInt(),correct_oper)+",";
             if(!correct_oper)
             {
                 buftoreturn.clear();
                 break;
             }

             buftoreturn+=substr.section(',',4,4)+",";//lac

             buftoreturn+=substr.section(',',6,6)+",";
             //Serving_Cell,1275,250,20,19656,2,199865612,3,4,4,10,397,-177,-1036,-672,22\r\n\r\n
             buftoreturn+="-"+substr.section(',',15,15)+"dBm\n";

            }
        }
        qDebug()<<buftoreturn;
        qDebug()<<"\nparse end\n";

        return buftoreturn;

    }
    }
}
