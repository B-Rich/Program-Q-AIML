/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : mar mar 8 2005
    copyright            : (C) 2005 by houssem bdioui
    email                : houssem.bdioui@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include <QPlastiqueStyle>
#include "mymaindialog.h"
#include "aimlparser.h"
#include <stdio.h>

#define DEFAULT_PORT "32770"
char helpMessage[] = "Usage: %s [options]\nWhere options can be:\n"
                     "  -h, --help                       display this help message\n"
                     "  -d, --display-tree               display the nodes tree in debug.log\n"
                     "  -r, --run-regression             run a test suite regression & exit\n"
                     "                                   (see debug.log)\n"
                     "  -s, --server-mode                run ProgramQ in server mode\n"
                     "                                   (AIML subset must be set)\n"
                     "  -a, --aiml-subset \"subset name\"  use the specified AIML subset\n"
                     "  -p, --port \"port number\"         set port number (default: "DEFAULT_PORT")\n";

//The main entry
int main( int argc, char ** argv )
{
    QApplication a(argc, argv);
    QApplication::setStyle(new QPlastiqueStyle);
    bool displayTree = false, runRegression = false, server = false;
    QString subset(""), portStr(DEFAULT_PORT);
    for (uint i = 1; i < argc; i++)
    {
        if ((QString(argv[i]) == "-h") || (QString(argv[i]) == "--help"))
        {
            printf(helpMessage, argv[0]);
            return 0;
        }
        else if ((QString(argv[i]) == "-d") || (QString(argv[i]) == "--display-tree"))
            displayTree = true;
        else if ((QString(argv[i]) == "-r") || (QString(argv[i]) == "--run-regression"))
            runRegression = true;
        else if ((QString(argv[i]) == "-s") || (QString(argv[i]) == "--server-mode"))
            server = true;
        else if ((argc > i+1) && ((QString(argv[i]) == "-a") || (QString(argv[i]) == "--aiml-subset")))
        {
            subset = argv[i+1];
            i++;
        }
        else if ((argc > i+1) && ((QString(argv[i]) == "-p") || (QString(argv[i]) == "--port")))
        {
            portStr = argv[i+1];
            i++;
        }
        else
        {
            printf(helpMessage, argv[0]);
            return -1;
        }
    }
    //Create the parser
    AIMLParser parser(displayTree);
    if (server)
    {
        bool ok;
        uint port = portStr.toUInt(&ok);
        if (subset.isEmpty() || !ok)
        {
            printf(helpMessage, argv[0]);
            return -1;
        }
        if (!parser.loadAIMLSet(subset) || !parser.startServer(port))
        {
           printf("Check errors in debug.log!\n");
           return -1;
        }
        else
           return a.exec();
    }
    else if (runRegression)
    {
        parser.runRegression();
        return 0;
    }
    //Create and show the main window if in GUI mode
    MyMainDialog md(&parser, subset);
    md.show();
    return a.exec();
}
