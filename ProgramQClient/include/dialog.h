#ifndef DIALOG_H_
#define DIALOG_H_

#include <QDialog>

class QLineEdit;
class QTextEdit;
class QPushButton;
class Client;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(Client *);
    virtual ~Dialog();

private slots:
    void requestNewAnswer();
    void readAnswer(const QString &);
    void displayError(const QString&);
    void enableGetAnswerButton();

private:
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *inputLineEdit;
    QTextEdit *outputTextEdit;
    QPushButton *getAnswerButton;
    QPushButton *shutDownServerButton;
    Client *client;
};

#endif /*DIALOG_H_*/
