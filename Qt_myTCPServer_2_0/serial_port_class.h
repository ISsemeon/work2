#ifndef SERIAL_PORT_CLASS_H
#define SERIAL_PORT_CLASS_H
enum typeoftransmottion{GSM,LTE,UTMS,GPS};

enum operatorsForconvert{_convertMTS=1,_convertMEGAFON=2,_convertHCC=3,_convertSMART=4,_convertTELE2=20,_convertBEELINE=99};


#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include <QThread>
#include <QRegExp>





QByteArray convertoperator(int operator_value,bool &correct_operator);

class Serial_port_class
{


public:
    Serial_port_class();
    QByteArray write_Data(typeoftransmottion type);

    void settings();//функция настройки ком порта
    void found_com_port(QString port_name);//функция поиска ком порта
    void changeOperator();//функция смены оператора
    void AddCommandsGSM()//функция добавления команд GSM
    {
         command_vectorGSM.clear();
         command_vectorGSM.push_back("AT+CNMP=13\r");
         command_vectorGSM.push_back("at+ccinfo\r");
         command_vectorGSM.push_back("at+ccinfo\r");
         command_vectorGSM.push_back("at+ccinfo\r");


        /*25001 - MTC
          25002 - Megafon
          25003 НСС
          25004 Смартс
          25020 - Tele2
          25099 - Beeline*/
    }
    void AddCommandsLTE()//функция добавления команд LTE
    {
        command_vectorLTE.clear();

        command_vectorLTE.push_back("AT+CNMP=38\r");
        command_vectorLTE.push_back("AT+CNMP=38\r");
        command_vectorLTE.push_back("AT+cnbp=0x0002000008400380,0x000007FF03DF3FFF\r");
        command_vectorLTE.push_back("AT+cmgrmi=4\r");
        command_vectorLTE.push_back("AT+cmgrmi=4\r");

        command_vectorLTE.push_back("AT+cnbp=0x40000000,0x0000000000000004\r");
        command_vectorLTE.push_back("AT+cmgrmi=4\r");

        command_vectorLTE.push_back("AT+cnbp=0x40000000,0x0000000000000040\r");
        command_vectorLTE.push_back("AT+cmgrmi=4\r");

        command_vectorLTE.push_back("AT+cnbp=0x40000000,0x0000000000080000\r");
        command_vectorLTE.push_back("AT+cmgrmi=4\r");

//        command_vectorLTE.push_back("AT+cnbp=0x40000000,0x0000002000000000\r");
//        command_vectorLTE.push_back("AT+cmgrmi=4\r");




    }
    void AddCommandsUTMS()//функция добавления команд UTMS
    {
        command_vectorUTMS.clear();
        command_vectorUTMS.push_back("AT+CNMP=14\r");
        command_vectorUTMS.push_back("AT+cnsvsx=2,0\r");

    }
    void AddCommandsGPS()//функция добавления команд GPS
    {
        command_vectorGPS.push_back("AT+CGPS=1\r");
        command_vectorGPS.push_back("AT+CGPSINFO\r");
    }
   void convert_operator(int oper);//функиця преобразованиея номера оператора в его название
   void changeOperator(bool val){operatorchanged=val;};//функиця при смене оператора


private:
    QSerialPort *port;
    QVector<QString> command_vectorGSM;
    QVector<QString> command_vectorUTMS;
    QVector<QString> command_vectorGPS;
    QVector<QString> command_vectorLTE;
    QString current_request;
    bool operatorchanged;


};

 QByteArray parse(QByteArray string,typeoftransmottion type);//функиця парсинга прриходящих данных

#endif // SERIAL_PORT_CLASS_H
