#include "mainwindow.h"
#include "neatvi/vi.h"
#include <QFontDatabase>
int main(int argc, char *argv[]) {
  QCoreApplication::setApplicationName("qneatvi");
  QApplication a(argc, argv);
  QFontDatabase::addApplicationFont(":/fonts/VazirCode.ttf");
  // a.setLayoutDirection(Qt::LeftToRight);
  main2(argc, argv);
  return 0; // a.exec();
}
