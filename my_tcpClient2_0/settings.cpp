#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    ui->radioButtonGSM->setChecked(true);
    ui->uniqvalues_cb->setChecked(true);
    ui->ALLRB->setChecked(true);

}

settings::~settings()
{
    delete ui;
}

QString settings::get_host()
{
    return host;
}

quint16 settings::get_port()
{
    return port;
}

typeoftransmottion settings::getType()
{
if(ui->radioButtonALL->isChecked())
    return  typeoftransmottion::ALL;
else if(ui->radioButtonUTMS->isChecked())
        return  typeoftransmottion::UTMS;
else if(ui->radioButtonLTE->isChecked())
        return  typeoftransmottion::LTE;
else if(ui->radioButtonGSM->isChecked())
    return  typeoftransmottion::GSM;
}

client_operator settings::getOperator()
{
    if(ui->TELE2RB->isChecked()){return client_operator::TELE2;}
    else if(ui->HCCRB->isChecked()){return client_operator::HCC;}
    else if(ui->BEELINERB->isChecked()){return client_operator::BEELINE;}
    else if(ui->mtsRB->isChecked()){return client_operator::MTS;}
    else if(ui->SMARTRB->isChecked()){return client_operator::SMART;}
    else if(ui->MEGAFONRB->isChecked()){return client_operator::MEGAFON;}
    else{return client_operator::ALLOPERATORS;}
}

bool settings::isUnicvalues()
{
return ui->uniqvalues_cb->isChecked();
}

void settings::on_pushButton_clicked()
{
   host=ui->lineEdit_2->text();
   port=ui->lineEdit->text().toInt();

   close();
}
