/***************************************************************************
                          mymaindialog.cpp  -  description
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

#include "mymaindialog.h"
#include "aimlparser.h"
#include "ui_aimlsetdialog.h"
#include <QComboBox>
#include <QTextEdit>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QDir>
#include <QPushButton>
#include <QProgressDialog>

MyMainDialog::MyMainDialog(AIMLParser *parser, const QString& aimlSet)
        : QDialog(0, Qt::Window), parser(parser), regexp("&lt;(/*)html:(.+)&gt;")
{
    setupUi(this);
    setWindowTitle(windowTitle().replace("#VERSION", VERSION));
    regexp.setMinimal(true);
    comboBox->setFocus();
    textEdit->setFocusPolicy(Qt::NoFocus);
    pushButton1->setFocusPolicy(Qt::TabFocus);
    connect(pushButton1, SIGNAL(clicked()), SLOT(validateInput()));
    connect(AIMLSet, SIGNAL(clicked()), SLOT(setAIMLSet()));
    connect(parser, SIGNAL(aboutToLoadAIMLFile(const QString&)),
            SLOT(aboutToLoadAIMLFile(const QString&)));
    connect(comboBox, SIGNAL(editTextChanged (const QString &)),
            SLOT(determineWidgetsDirection(const QString&)));
    currentPath = qApp->applicationDirPath();
    setAIMLSet(aimlSet);
}

MyMainDialog::~MyMainDialog()
{}

void MyMainDialog::setAIMLSet(QString aimlSet)
{
    if (aimlSet.isEmpty())
    {
        //choose which aiml set to use!
        QDir dir(parser->getAIMLSetDirectory());
        QStringList subdirs = dir.entryList(QDir::Dirs);
        subdirs.removeAll(".");
        subdirs.removeAll("..");
        QDialog aimlDialog(this);
        Ui::AIMLSetDialog ui;
        ui.setupUi(&aimlDialog);
        ui.aiml_set->addItems(subdirs);
        if (aimlDialog.exec() != QDialog::Accepted)
            return;
        aimlSet = ui.aiml_set->currentText();
    }
    AIMLSet->setText("<a href=\"href\">" + aimlSet + "</a>");

    QString dirname = currentPath + "/aiml_set/" + aimlSet;
    QDir dir(dirname, "*.aiml");
    QStringList files = dir.entryList();
    pd = new QProgressDialog(this);
    pd->setSizeGripEnabled(true);
    pd->setWindowTitle(QString("\"%1\" AIML set").arg(aimlSet));
    pd->setCancelButton(NULL);
    pd->setMinimum(0);
    pd->setMaximum(files.count()-1);
    pd->show();
    fileIndex = 0;

    if (!parser->loadAIMLSet(aimlSet))
        printf("Check errors in debug.log!\n");

    delete pd;
    pd = NULL;
}

void MyMainDialog::aboutToLoadAIMLFile(const QString& aimlfile)
{
    if (!pd)
        return;
    pd->setLabelText("parsing " + QFileInfo(aimlfile).fileName() + "...");
    pd->setValue(fileIndex++);
    qApp->processEvents();
}

void MyMainDialog::determineWidgetsDirection(const QString& input)
{
    if (input.isEmpty())
        return;
    bool right2left = false;
    for (int i = 0; i < input.length(); i++)
    {
        if (input.at(i).direction() == QChar::DirAL)
        {
            right2left = true;
            break;
        }
    }
    comboBox->setLayoutDirection((Qt::LayoutDirection)right2left);
}

void MyMainDialog::validateInput()
{
    QString input = comboBox->currentText();
    if (input.trimmed().isEmpty())
        return;
    if (comboBox->findText(input) == -1)
        comboBox->addItem(input);
    //cursor operations (move to the end & change layout direction)
    QTextCursor textCursor = textEdit->textCursor();
    textCursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(textCursor);
    bool right2left = comboBox->layoutDirection() == Qt::RightToLeft;
    textEdit->setAlignment(right2left ? Qt::AlignRight : Qt::AlignLeft);
    //display input
    textEdit->setTextColor(Qt::red);
    QString output;
    if (right2left)
    {
        //Arabic text
        QChar unicode[] = {1571, 1606, 1578};
        output.setUnicode(unicode, 3);
        output += "> ";
    }
    else
        output = "You> ";
    textEdit->insertPlainText(output);
    textEdit->setTextColor(Qt::black);
    textEdit->insertPlainText(input + "\n");
    //display output
    QString result = parser->getResponse(input);
    textEdit->setTextColor(Qt::blue);
    if (right2left)
    {
        //Arabic text
        QChar unicode[] = {1607, 1608};
        output.setUnicode(unicode, 2);
        output += "> ";
    }
    else
        output = "Bot> ";
    textEdit->insertPlainText(output);
    textEdit->setTextColor(Qt::black);
    //textEdit->insertPlainText(result + "\n\n");

    QString html = Qt::convertFromPlainText(result);
    html.replace(regexp, "<\\1\\2>"),
    textEdit->insertHtml(html+"<br>");
    textEdit->ensureCursorVisible();
    //qWarning(qPrintable(html));
    comboBox->clearEditText();
}
