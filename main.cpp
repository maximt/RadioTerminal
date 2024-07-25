#include "radioterminal.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RadioTerminal w;
    w.show();

    return a.exec();
}
