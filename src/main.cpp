#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <NetworkRequest.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    uint i = 0;
    const int maxCopyes = 3;
    for (int copy = 0; copy < maxCopyes; ++copy) {
        QTimer* timer = new QTimer;
        timer->setInterval(0);
        timer->setSingleShot(true);
        QObject::connect(timer, &QTimer::timeout, [&] {
            qDebug()<< ++i << QTime::currentTime().toString("hh:mm:ss") << "Started";
            NetworkRequest::loadAsyncS("https://mail.ru", timer, SLOT(start()));
        });
        timer->start();
    }

//  Writer* w = new Writer;
//  for (int reply = 0; reply < 100000; ++reply) {
//      NetworkRequest::loadAsyncS("https://mail.ru", w, SLOT(write()));
//  }

    return a.exec();
}
