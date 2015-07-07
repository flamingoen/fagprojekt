#ifndef CRAWLER_H
#define CRAWLER_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string>
#include <errno.h>
#include <dirent.h>

#include <QStringList>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QtGui>

#define FILETYPE "vtk"

QList<QStringList> getSeries(QString dirPath);
QStringList dirCrawler(QString dir_path);
QList<QStringList> sortSets(QStringList list);
int indexOfFistInt(QString str);

#endif // CRAWLER_H