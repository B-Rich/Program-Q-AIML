#include <QtGui>
#include <QtNetwork>
#include "client.h"

Client::Client(bool console) : console(console)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), SLOT(readAnswer()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(connected()), SLOT(connected()));
}

Client::~Client()
{}

void Client::requestNewAnswer(const QString& host, uint port,
                              const QString &msg,
                              bool shutDownServer)
{
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(host, port);
    if (!shutDownServer)
        input = msg;
    else
        input = "shut down ProgramQ server";
}

void Client::connected()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << input;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    tcpSocket->write(block);
}

void Client::readAnswer()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0)
    {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }
    if (tcpSocket->bytesAvailable() < blockSize)
        return;

    QString _answer;
    in >> _answer;
    if (console)
    {
        printf("%s\n", qPrintable(_answer));
        qApp->quit();
    }
    else
        emit answer(_answer);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    QString msg;
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        return;
    case QAbstractSocket::HostNotFoundError:
        msg = "The host was not found. Please check the\n"
              "host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        msg = "The connection was refused by the peer.\n"
              "Make sure the ProgramQ is running in server mode,\n"
              "and check that the host name and port\n"
              "settings are correct.";
        break;
    default:
        msg = QString("The following error occurred:\n%1.")
              .arg(tcpSocket->errorString());
    }
    if (console)
    {
        qWarning(qPrintable(msg));
        qApp->quit();
    }
    else
        emit error(msg);
}
