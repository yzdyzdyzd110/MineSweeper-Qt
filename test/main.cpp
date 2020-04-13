#include "widget.h"
#include <QApplication>
#include "logic.h"
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWidget w;
   w.show();
   return a.exec();
}
