#ifndef LIDARCLIENT_H
#define LIDARCLIENT_H

#include <QTcpSocket>
#include "qcustomplot.h"

class LidarClient : public QWidget
{
    Q_OBJECT

public:
    LidarClient(const QString &strHost, int nPort, QWidget *parent = nullptr);
    ~LidarClient() override {}

private:
    QTcpSocket *m_tcpSocket;
    QVector<int> m_distanse;
    QCustomPlot *m_customPlot;
    QByteArray m_command;
    QByteArray m_stopAnswer;
    QByteArray m_wholePackage;
    bool isConnected = false;
    QVector<double> m_x;
    QVector<double> m_y;

    void convertDistanсe();
    void showScan();

private slots :
    void slotReadyRead();
    void slotConnected();
    void onLastScanClicked();
    void onContinScanClicked();
    void onStopContinScanClicked();
};

#endif // LIDARCLIENT_H
