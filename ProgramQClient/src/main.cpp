#include <QApplication>

#include "dialog.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QString usage = QString ("%1 [-input question [-server host_name]"
                             " [-port port_number] ]").arg(argv[0]);
    QApplication app(argc, argv);
    QString input(""), server("localhost"), port("32770");
    bool error = false;
    for (int i = 1; i < argc; i++)
    {
        if (QString(argv[i]) == "-input")
        {
            if (i+1 >= argc)
                error = true;
            else
            {
                i++;
                input = argv[i];
            }
        }
        else if (QString(argv[i]) == "-server")
        {
            if (i+1 >= argc)
                error = true;
            else
            {
                i++;
                server = argv[i];
            }
        }
        else if (QString(argv[i]) == "-port")
        {
            if (i+1 >= argc)
                error = true;
            else
            {
                i++;
                port = argv[i];
            }
        }
        else
            error = true;
    }
    if (error)
    {
        qWarning(qPrintable(usage));
        return -1;
    }
    Client c(!input.isEmpty());
    Dialog *dialog = NULL;
    if (!input.isEmpty())
    {
        c.requestNewAnswer(server, port.toUInt(), input);
    }
    else
    {
        dialog = new Dialog(&c);
        dialog->show();
    }
    int retval = app.exec();
    if (dialog)
        delete dialog;
    return retval;
}
