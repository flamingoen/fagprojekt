/*
 * UILayer.cxx
 *  
 *  Authors: Martin Obel Thomsen, Daniel Blyme Gruntwig, Andreas Rømer Hjorth
 *
 */

#include "UILayer.h"

// UI elements
QListWidget* listFields;
QListWidget* listWells;
QList<QListWidgetItem*> itemListFields;
QHash<QString , QList<QStringList> > wellTypes;
QList<QStringList> activeWellTypeList;
QComboBox *comboBoxWell;
BlackWhirl programInstance;

// connects all the UI elements

void UILayer::constructor() { // s134862
    
    // setup Window UI
    this->setWindowTitle("Blackwhirl");
    QWidget* t_widget = new QWidget(this);
    Ui::Blackwhirl ui;
    ui.setupUi(t_widget);
    this->setWidget(t_widget);
    
    connectToUI();
}

void UILayer::connectToUI() { // s134835
    listFields = findChild<QListWidget*>("list_fields");
    listWells = findChild<QListWidget*>("list_wells");
    comboBoxWell = findChild<QComboBox*>("comboBox_wellType");
    QPushButton *loadButton = findChild<QPushButton*>("button_load");
    QPushButton *closeButton = findChild<QPushButton*>("button_close");
    QPushButton *showAll = findChild<QPushButton*>("button_showAll");
    QPushButton *hideAll = findChild<QPushButton*>("button_hideAll");
    QCheckBox *showBox = findChild<QCheckBox*>("checkBox");
    
    // add import button
    connect(loadButton,SIGNAL(clicked()),this,SLOT(onLoadAction()));
    // add close button
    connect(closeButton,SIGNAL(clicked()),this,SLOT(onCloseAction()));
    // add show all button
    connect(showAll,SIGNAL(clicked()),this,SLOT(onShowAllAction()));
    // add hide all button
    connect(hideAll,SIGNAL(clicked()),this,SLOT(onHideAllAction()));
    // add fields list
    connect(listFields, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(onListFieldAction(QListWidgetItem*)));
    // add wells list
    connect(listWells, SIGNAL(itemClicked(QListWidgetItem*)), this,
            SLOT(onListWellAction(QListWidgetItem*)));
    // add wells combobox
    connect(comboBoxWell, SIGNAL(currentIndexChanged(const QString&)), this,
            SLOT(onWellTypeChange(const QString&)));  
    // add show checkbox
    connect(showBox, SIGNAL(stateChanged(int)), this, SLOT(onWellShowSelected(int)));
    
    std::cout << "connect done" << endl;
} 

// ----------------------- Case Combo Box ------------------------

void UILayer::onLoadAction() { // s134862
    
    // opens QT-dialog for chosing folder
    QString folder = QFileDialog::getExistingDirectory(0,tr("Open File"),"/home" ,QFileDialog::ShowDirsOnly);
    std::cout << "before import" << endl;
    // get list of sources from crawler
    if(folder.length()){
        std::cout << "in import" << endl;
        import(folder);
    }
    // fix of a bug where the first item in the stringList would be loaded alone TODO: fix?
    cout << "after import" << endl;
    onWellTypeChange(comboBoxWell->itemText(0));
    
    // render the view
    programInstance.resetView();
}

void UILayer::onCloseAction() { // s132106
    // remove all sources 
    programInstance.clearSources();
    
    // Clear all application lists
    itemListFields.clear();
    wellTypes.clear();
    listFields->clear();
    listWells->clear();
    comboBoxWell->clear();
    
    // render the view
    setShowCheckbox(0);
    programInstance.resetView();
}

// imports and splits sources 
void UILayer::import(QString folder) { // s134835
    
    // get series from crawler
    QList<QStringList> list = getSeries(folder);
    
    // splits sources in a list of wells and a list of fields
    cout << "before foreach" << endl;
    foreach(QStringList series, list){
        
        QString wellStr = "well";
        std::cout << series[0].toStdString() << endl;
        
        if (series[0].contains(wellStr)){
            // works but not as robust, as pretty as a unicorn
            QStringList list = series[0].split(QRegExp("[.]"));
            QString str = list[2];
            
            if (wellTypes.contains(str)){ // TODO: Fix this, this is redundant - need to be able to just add to the wellTypes list 
                QList<QStringList> hashList = wellTypes.value(str);
                hashList.append(series);
                wellTypes.insert(str,hashList);
            } else {
                QList<QStringList> list;
                list << series;
                wellTypes.insert(str,list);
                cout << "before add" << endl;
                comboBoxWell->addItem(str);
                cout << "after add" << endl;
            }
            
            // works but not as pretty as a unicorn
            //int index = series[0].indexOf(QRegExp("[WELL][0-9]+[.]"))+1;
            //QString str = series[0].right(series[0].size()-index-2);
            //index = str.indexOf(QRegExp("[.]"));
            //str = str.left(index);
            
            programInstance.loadWellSerie(series);
            
        } else {
            QStringList list = series[0].split(QRegExp("[.]"));
            QString str = list[1]+"."+list[2];
            QListWidgetItem* item = new QListWidgetItem;
            programInstance.loadFieldSerie(series);
            item -> setData(Qt::DisplayRole , str);
            listFields -> addItem(item);
            itemListFields.append(item);
        }
    }
    cout << "after foreach" << endl;
}

// ----------------------- Wells Combo Box -----------------------

void UILayer::setShowCheckbox(int bl){ // s134862
    QCheckBox *showBox = findChild<QCheckBox*>("checkBox");
    showBox->setChecked(false);
    if(programInstance.hasActiveWell()){
        if(bl){
            showBox->setChecked(true);
        }
    }
}

void UILayer::onWellTypeChange(const QString& str) { // s132106
    createWellTypeList(str);
    if(programInstance.hasActiveWell()){
        programInstance.hideAllWells();
    }
    if(programInstance.getWellList().length()!=0 && activeWellTypeList.length()!=0){
        programInstance.setActiveWell(activeWellTypeList[0]);
    }
}

void UILayer::createWellTypeList(const QString& str){ // s134835
    activeWellTypeList = wellTypes.value(str);
    // we only want to show the chosen type of wells
    listWells->clear();
    
    foreach(QStringList series, activeWellTypeList) {
        std::cout << "well series: " << series[0].toStdString() << endl;
        QListWidgetItem* item = new QListWidgetItem;
        QStringList list = series[0].split(QRegExp("[.]"));
        QString str = list[1]; 
        QVariant filepaths(series);
        item -> setData(Qt::DisplayRole , str);
        item ->setData(Qt::UserRole, filepaths);
        listWells -> addItem(item);
    }
}

void UILayer::onShowAllAction() { // s134862
    setShowCheckbox(1);
    programInstance.showAllWells(activeWellTypeList);
}

void UILayer::onHideAllAction() { s132106
    setShowCheckbox(0);
    programInstance.hideAllWells();
}

void UILayer::onWellShowSelected(int state){ // s134835
    if(programInstance.hasActiveWell()){
        if(state){
            programInstance.addOrShowActiveWell(); 
        }else{
            programInstance.hideActiveWell();
        }
    }
}

void UILayer::onListWellAction(QListWidgetItem* item) { // s134862
    std::cout << "in on list well action" << endl;
    programInstance.setActiveWell(item->data(Qt::UserRole).toStringList());
    std::cout << "has set active well" << endl;
    bool check = programInstance.activeWellIsVisable();
    std::cout << "bool check: " << programInstance.activeWellIsVisable() << endl;
    setShowCheckbox(check);
    std::cout << "checkbox is set" << endl;
    onWellShowSelected(check);
    std::cout << " show selected finished" << endl;
}

// ----------------------- Fields Combo Box -----------------------

// Activates the onClick listener for the items in the fields list
void UILayer::onListFieldAction(QListWidgetItem* item) { // s132106
    int listPosition = itemListFields.indexOf(item);
    programInstance.hideAllFields();
    programInstance.addOrShowField(listPosition);
}
