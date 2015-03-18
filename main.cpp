#include "main_window.h"
#include "global.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(AppName);
    Global::Start();
    
    int ret = a.exec();
    Global::End();

    return ret;
}
