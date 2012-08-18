/***************************************************************************
			mymaindialog.h  -  description
			-------------------
begin                : lun sep 26 2005
copyright            : (C) 2005 by houssem
email                : houssem@localhost
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef MYMAINDIALOG_H
#define MYMAINDIALOG_H

#include "ui_maindialog.h"

/**
*@author houssem bdioui
*/

class QProgressDialog;
class AIMLParser;

class MyMainDialog : public QDialog, Ui::Dialog
{
    Q_OBJECT
public:
    MyMainDialog(AIMLParser*, const QString&);
    virtual ~MyMainDialog();
private:
    bool server;
    QProgressDialog *pd;
    AIMLParser *parser;
    uint fileIndex;
    QString currentPath;
    QRegExp regexp;
private slots:
    void validateInput();
    void setAIMLSet(QString = "");
    void aboutToLoadAIMLFile(const QString&);
    void determineWidgetsDirection(const QString&);
};

#endif
