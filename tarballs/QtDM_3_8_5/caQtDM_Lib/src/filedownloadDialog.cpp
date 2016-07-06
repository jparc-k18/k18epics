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

#include "filedownloadDialog.h"
#include "networkaccess.h"

FileDownloadDialog::FileDownloadDialog(const QString &urlString, const QString &indexFile, QWidget *parent): QDialog(parent)
{
    thisUrlString = UrlIndexFileString = urlString;
    UrlIndexFileString.append(indexFile);
    thisUrl = QUrl(UrlIndexFileString);

    cancelButton = createButton("&Cancel", SLOT(cancel()));

    QPushButton *dummyButton = new QPushButton("");
    dummyButton->setDefault(true);
    dummyButton->setVisible(false);

    deviceLabel = new QLabel("Search Pattern for " + thisUrl.toString() + ":");
    fileChoice = new QLineEdit("");
    fileChoice->setReadOnly(true);

    createfilesTable();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->addWidget(deviceLabel, 0, 0);
    mainLayout->addWidget(filesTable, 3, 0, 1, 4);
    mainLayout->addWidget(fileChoice, 4, 0, 1, 4);
    mainLayout->addWidget(dummyButton, 6, 0, 1, 4);

    QDialogButtonBox *box = new QDialogButtonBox( QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal );
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));

    mainLayout->addWidget(box, 6, 0);

    setLayout(mainLayout);

    setWindowTitle("Find Ui files");
}

int FileDownloadDialog::openUrl()
{
    NetworkAccess *wget = new NetworkAccess(filesTable);
    wget->requestUrl(thisUrl);
    return true;
}

void FileDownloadDialog::cancel()
{
    qDebug() << "cancel";
}

QPushButton *FileDownloadDialog::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void FileDownloadDialog::createfilesTable()
{
    filesTable = new QTableWidget(0, 1);
    filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStringList labels;
    labels << "devicename";
    filesTable->setHorizontalHeaderLabels(labels);
    filesTable->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    filesTable->verticalHeader()->hide();
    filesTable->setShowGrid(true);
    filesTable->setAlternatingRowColors(true);
    filesTable->verticalHeader()->setDefaultSectionSize(18);
    filesTable->setStyleSheet("QTableWidget { font: 10pt; background: cornsilk; alternate-background-color: wheat;}");

    connect(filesTable, SIGNAL(cellActivated(int,int)), this, SLOT(downloadFile(int,int)));
}


void FileDownloadDialog::downloadFile(int row, int /* column */)
{
    QTableWidgetItem *item = filesTable->item(row, 0);
    fileChoice->setText(item->text());

    fileChoosen = item->text();
    QString fileString = thisUrlString;
    fileString.append(fileChoosen);

    QUrl fileUrl(fileString);

    qDebug() << "start downloading" << fileString;

    NetworkAccess *wget = new NetworkAccess(0, fileChoosen);
    wget->requestUrl(fileUrl);
}
