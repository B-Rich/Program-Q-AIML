#include <QtGui>
#include "dialog.h"
#include "client.h"

Dialog::Dialog(Client *client) : client(client)
{
    QLabel *hostLabel = new QLabel(tr("&Server name:"));
    QLabel *portLabel = new QLabel(tr("S&erver port:"));
    QLabel *inputLabel = new QLabel(tr("&Input:"));

    hostLineEdit = new QLineEdit("Localhost");
    portLineEdit = new QLineEdit("32770");
    inputLineEdit = new QLineEdit;
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    hostLabel->setBuddy(hostLineEdit);
    portLabel->setBuddy(portLineEdit);
    inputLabel->setBuddy(inputLineEdit);

    outputTextEdit = new QTextEdit(tr("This client requires that you run the "
                                      "ProgramQ Server as well."));
    outputTextEdit->setReadOnly(true);

    getAnswerButton = new QPushButton(tr("Get Answer"));
    getAnswerButton->setDefault(true);

    QPushButton *quitButton = new QPushButton(tr("Quit"));
    shutDownServerButton = new QPushButton(tr("Shut down server"));

    connect(hostLineEdit, SIGNAL(textChanged(const QString &)),
            SLOT(enableGetAnswerButton()));
    connect(portLineEdit, SIGNAL(textChanged(const QString &)),
            SLOT(enableGetAnswerButton()));
    connect(getAnswerButton, SIGNAL(clicked()),
            SLOT(requestNewAnswer()));
    connect(quitButton, SIGNAL(clicked()), SLOT(close()));
    connect(shutDownServerButton, SIGNAL(clicked()), SLOT(requestNewAnswer()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(getAnswerButton);
    buttonLayout->addWidget(shutDownServerButton);
    buttonLayout->addWidget(quitButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostLineEdit, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(inputLabel, 2, 0);
    mainLayout->addWidget(inputLineEdit, 2, 1);
    mainLayout->addWidget(outputTextEdit, 3, 0, 1, 2);
    mainLayout->addLayout(buttonLayout, 4, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("ProgramQ Client"));
    inputLineEdit->setFocus();

    //Client connections
    connect(client, SIGNAL(error(const QString&)), SLOT(displayError(const QString &)));
    connect(client, SIGNAL(answer(const QString&)), SLOT(readAnswer(const QString &)));
}

Dialog::~Dialog()
{}

void Dialog::requestNewAnswer()
{
    client->requestNewAnswer(hostLineEdit->text(),
                            portLineEdit->text().toUInt(),
                            inputLineEdit->text(),
                            sender() == shutDownServerButton);
}

void Dialog::readAnswer(const QString &answer)
{
    outputTextEdit->setPlainText(answer);
    inputLineEdit->clear();
}

void Dialog::displayError(const QString &msg)
{
    QMessageBox::information(this, tr("ProgramQ Client"),
                             msg);
    getAnswerButton->setEnabled(true);
}

void Dialog::enableGetAnswerButton()
{
    getAnswerButton->setEnabled(!hostLineEdit->text().isEmpty()
                                && !portLineEdit->text().isEmpty());
}
