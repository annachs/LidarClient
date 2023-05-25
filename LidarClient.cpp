#include "LidarClient.h"

LidarClient::LidarClient(const QString &strHost, int nPort, QWidget *parent)
    : QWidget(parent)
{
    //Stop command for lidar
    m_stopAnswer = QByteArray::fromHex("02 73 45 41 20 4C 4D 44 73 63 61 6E 64 61 74 61 20 30 03");

    //Buttons setup
    auto lastScan = new QPushButton("Last scan", this);
    auto continScan = new QPushButton("Continuous scanning", this);
    auto stopContinScan = new QPushButton("Stop continuous scanning", this);
    connect(lastScan, &QPushButton::clicked,
            this, &LidarClient::onLastScanClicked);
    connect(continScan, &QPushButton::clicked,
            this, &LidarClient::onContinScanClicked);
    connect(stopContinScan, &QPushButton::clicked,
            this, &LidarClient::onStopContinScanClicked);

    //Plotter setup
    m_customPlot = new QCustomPlot(this);

    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickStep(100);

    auto p = m_customPlot->xAxis;
    p->setRange(-700, 3500);
    p->setTicker(fixedTicker);
    p->setTickLabels(false);
    p->setTicks(false);
    QFont font = p->labelFont();
    font.setPointSize(14);
    p->grid()->setPen(QPen(QColor(128, 255, 128)));

    p = m_customPlot->yAxis;
    p->setRange(-1000, 3000);
    p->setTicker(fixedTicker);
    p->setTickLabels(false);
    p->setTicks(false);
    p->grid()->setPen(QPen(QColor(128, 255, 128)));

    p = m_customPlot->yAxis2;
    p->setVisible(true);
    p->setTickLabels(false);
    p->setTicks(false);

    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    //Layout setup
    auto layout = new QGridLayout(this);
    layout->addWidget(lastScan, 0, 0, 1, 1);
    layout->addWidget(continScan, 0, 1, 1, 1);
    layout->addWidget(stopContinScan, 0, 2, 1, 1);
    layout->addWidget(m_customPlot, 1, 0, 1, 3);

    //Socket setup
    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->connectToHost(strHost, nPort);
    connect(m_tcpSocket, &QTcpSocket::connected,
            this, &LidarClient::slotConnected);
    connect(m_tcpSocket, &QTcpSocket::readyRead,
            this, &LidarClient::slotReadyRead);
}

void LidarClient::slotReadyRead()
{
    //This slot is called when data is received from lidar
    qint64 bytesAvailable = m_tcpSocket->bytesAvailable();
    if (bytesAvailable) {
       QByteArray answer = m_tcpSocket->readAll();
       for (int i = 0; i < answer.size(); i ++) {
           m_wholePackage.append(answer[i]);
           if (m_wholePackage.contains(QByteArray::fromHex("03")) &&
               m_wholePackage != m_stopAnswer) {
               m_distanse.clear();
               QList<QByteArray> list = m_wholePackage.split(' ');
               int startDataIndex = 26;
               int endDataIndex = list.size() - 7;
               for (int j = startDataIndex; j <= endDataIndex; j++) {
                   m_distanse.append(list[j].toInt(nullptr, 16));
               }
               m_wholePackage.clear();
               convertDistanсe();
           }
       }
    }   
}

void LidarClient::slotConnected()
{
    //This slot is called when connection with lidar is established
    isConnected = true;
}

void LidarClient::convertDistanсe()
{
    //Function for converting the distance measured by lidar to coordinate axes
    double step = qDegreesToRadians(0.5); //angular resolution in radians
    double angle = qDegreesToRadians(-45.0); //start scanning angle

    m_x.clear();
    m_y.clear();
    for (int i = 0; i < m_distanse.size(); i++) {
        double x = cos(angle) * m_distanse[i];
        double y = sin(angle) * m_distanse[i];
        m_x.append(x);
        m_y.append(y);
        angle += step;
    }

    showScan();
}

void LidarClient::showScan()
{
    //Convertial results drawing function
    m_customPlot->clearPlottables();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::blue);
    m_customPlot->addGraph();
    m_customPlot->graph()->setPen(pen);
    m_customPlot->graph()->setLineStyle(QCPGraph::lsNone);
    m_customPlot->graph()->setAdaptiveSampling(false);
    m_customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot));
    m_customPlot->graph()->setData(m_x, m_y);
    m_customPlot->replot();
}

void LidarClient::onLastScanClicked()
{
    //This slot is called when "Last scan" button clicked
    if (isConnected) {
       m_command = QByteArray::fromHex("02 73 52 4E 20 4C 4D 44 73 63 61 6E 64 61 74 61 03");
       m_tcpSocket->write(m_command, m_command.size());
   }
}

void LidarClient::onContinScanClicked()
{
    //This slot is called when "Continuous scanning" button clicked
    if (isConnected) {
        m_command = QByteArray::fromHex("02 73 45 4E 20 4C 4D 44 73 63 61 6E 64 61 74 61 20 31 03");
        m_tcpSocket->write(m_command, m_command.size());
    }
}

void LidarClient::onStopContinScanClicked()
{
    //This slot is called when "Stop continuous scanning" button clicked
    if (isConnected) {
        m_command = QByteArray::fromHex("02 73 45 4E 20 4C 4D 44 73 63 61 6E 64 61 74 61 20 30 03");
        m_tcpSocket->write(m_command, m_command.size());
    }
}
