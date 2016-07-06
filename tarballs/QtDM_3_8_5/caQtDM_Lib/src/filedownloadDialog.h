/*
 *  This file is part of the caQtDM Framework, developed at the Paul Scherrer Institut,
 *  Villigen, Switzerland
 *
 *  The caQtDM Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The caQtDM Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the caQtDM Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2010 - 2014
 *
 *  Author:
 *    Anton Mezger
 *  Contact details:
 *    anton.mezger@psi.ch
 */

#ifndef fileDialog_H
#define fileDialog_H

#include <QtGui>
#include <QDialog>
#include <QDir>
#include <QUrl>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QTableWidget>

class FileDownloadDialog: public QDialog
{
    Q_OBJECT

public:
    FileDownloadDialog(const QString &urlString, const QString &indexFile, QWidget *parent = 0);
    int openUrl();
    void executeQuery();

    QLineEdit *fileChoice;

private slots:
    void cancel();
    void downloadFile(int row, int column);

private:

    QPushButton *createButton(const QString &text, const char *member);
    void createfilesTable();

    QLabel *deviceLabel;
    QPushButton *cancelButton;

    QTableWidget *filesTable;
    QUrl thisUrl;
    QString UrlIndexFileString;
    QString thisUrlString;
    QString fileChoosen;

};

#endif
