#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

class QTcpSocket;

class Client : public QObject
{
    Q_OBJECT

public:
    Client(bool = false);
    virtual ~Client();
    void requestNewAnswer(const QString&, uint, const QString &,
                          bool = false);

private slots:
    void readAnswer();
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();

signals:
    void answer(const QString&);
    void error(const QString &);

private:
    QTcpSocket *tcpSocket;
    QString input;
    quint16 blockSize;
    bool console;
};

#endif
