#include "my_client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    my_client w;

    w.show();
    return a.exec();

}
