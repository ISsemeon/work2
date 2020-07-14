#ifndef SETTINGS_H
#define SETTINGS_H

enum typeoftransmottion{GSM,LTE,UTMS,ALL,UNKNOWN};
enum client_operator{ALLOPERATORS=0,MTS=1,MEGAFON=2,HCC=3,SMART=4,TELE2=5,BEELINE=6};


#include <QDialog>

namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();
    QString get_host();
     quint16 get_port();
    typeoftransmottion getType();
    client_operator getOperator();
    bool isUnicvalues();

private slots:
     void on_pushButton_clicked();


private:
    Ui::settings *ui;
    QString host;
    quint16 port;




};

#endif // SETTINGS_H
