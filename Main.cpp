#include <QtGui/QApplication>
#include "MainWindow.h"
#include <QTextCodec>
#include <QDesktopWidget>
#include <QtCore>
#include <QItemSelection>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("GlobeCanTools");
    QCoreApplication::setOrganizationDomain("Globe.com");
    QCoreApplication::setApplicationName("CanGui");

    QApplication::addLibraryPath("./plugins");
    QApplication::addLibraryPath(QApplication::applicationDirPath());
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);
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
