#include <QApplication>
#include "LidarClient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString lidarIp = "192.168.0.1";
    int lidarPort = 2112;

    LidarClient client(lidarIp, lidarPort);
    client.setGeometry(400, 200, 1000, 800);
    client.show();

    return a.exec();
}
