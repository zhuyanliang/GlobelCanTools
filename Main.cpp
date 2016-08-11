#include <QtGui/QApplication>
#include "MainWindow.h"
#include <QTextCodec>
#include <QDesktopWidget>
#include <QtCore>
#include <QItemSelection>

int main(int argc, char *argv[])
{
    QSettings settings;

    QCoreApplication::setOrganizationName("GlobeCanTools");
    QCoreApplication::setOrganizationDomain("Globe.com");
    QCoreApplication::setApplicationName("CanGui");

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    qRegisterMetaType<QItemSelection>("QItemSelection");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setFixedSize(w.width(),w.height());

    w.move((QApplication::desktop()->width() - w.width())/2,
           (QApplication::desktop()->height() - w.height())/2);

    return a.exec();
}
