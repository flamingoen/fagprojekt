#ifndef __UILayer_h
#define __UILayer_h

#include "crawler.h"
#include "BlackWhirl.h"

#include <QtUiTools>
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QStyle>
#include <QHash>
#include <QObject>

class UILayer : public QDockWidget {
    
    Q_OBJECT
    typedef QDockWidget Superclass;
    
public:
    UILayer(const QString &t, QWidget* p = 0, Qt::WindowFlags f=0):
    Superclass(t, p, f) { this->constructor(); }
    
    UILayer(QWidget *p=0, Qt::WindowFlags f=0):
    Superclass(p, f) { this->constructor(); }
    
    void connectToUI();
    void import(QString folder);
    void createWellTypeList(const QString& str);
    void setShowCheckbox(int);
    
    
private:
    void constructor();
    
    
public slots:
    void onLoadAction();
    void onCloseAction();
    void onListFieldAction(QListWidgetItem* item);
    void onListWellAction(QListWidgetItem* item);
    void onWellTypeChange(const QString& str);
    void onWellShowSelected(int state);
    void onShowAllAction();
    void onHideAllAction();
};

#endif
