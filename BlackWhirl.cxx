/*
 * BlackWhirl.cxx
 *  
 *  Authors: Martin Obel Thomsen, Daniel Blyme Gruntwig, Andreas Rømer Hjorth
 *
 */

#include "BlackWhirl.h"

// global variables
pqPVApplicationCore* core;
pqServer* server;
pqView* view;
pqObjectBuilder* objectBuilder;
QList<pqPipelineSource*> pipelinesWells;
QList<pqPipelineSource*> pipelinesFields;
QList<QStringList> wellList;
pqActiveObjects* activeObjects;
pqPipelineSource* activeWell = NULL;

BlackWhirl::BlackWhirl () { // s134835
    
    // get paraview objects
    core = pqPVApplicationCore::instance();
    server = core->getActiveServer();
    objectBuilder = core->getObjectBuilder();
    activeObjects = &( pqActiveObjects::instance());
    view = activeObjects->activeView();
    
}

void BlackWhirl::resetView(){ // s134862
    view->resetDisplay();
    view->render(); 
}

void BlackWhirl::addOrShowRepresentations(pqPipelineSource* source){ // s132106
    /*
     *    if the source is not represented in the view, add a representation to it,
     *    else show the representations
     */
    if(source->getRepresentations(view).size() == 0){
        addRepresentation(source);
    }else{
        showRepresentations(source);
    }
    // sets the added or shown object to active in paraview
    activeObjects->setActivePort(source->getOutputPort(0));
    
    resetView();
}

void BlackWhirl::addOrShowField(int index){ // s134835
    addOrShowRepresentations(pipelinesFields[index]);
}

void BlackWhirl::addOrShowActiveWell() { // s134862
    if(hasActiveWell()){
        addOrShowRepresentations(activeWell);
    }
}

void BlackWhirl::addRepresentation(pqPipelineSource* source){ // s132106
    pqOutputPort* port = source->getOutputPort(0);
    // create new representations if none exist
    pqDataRepresentation *representation = objectBuilder->
        createDataRepresentation(port, view);
    colorAndScaleRepresentation(representation);
    resetView();
}

void BlackWhirl::showRepresentations(pqPipelineSource* source){ // s134835
    QList<pqDataRepresentation*> representations = source->getRepresentations(view);
    foreach(pqDataRepresentation* representation, representations){
        representation->setVisible(true);
    } 
    activeObjects->setActivePort(source->getOutputPort(0)); 
    resetView();
}

void BlackWhirl::showAllWells(QList<QStringList> activeWellList) { // s134862
    
    foreach(QStringList well, activeWellList) {
        int index = wellList.indexOf(well);
        addOrShowRepresentations(pipelinesWells[index]);
    }
    activeObjects->setActivePort(pipelinesWells[0]->getOutputPort(0));
    
    // render the view
    resetView();
}

void BlackWhirl::hideAllWells(){ // s132106
    hideAll(pipelinesWells);
}

void BlackWhirl::hideAllFields(){ // s134835
    hideAll(pipelinesFields);
}

void BlackWhirl::hideAll(QList<pqPipelineSource*> sources){ // s132106
    foreach(pqPipelineSource* source, sources){
        hideRepresentations(source);
    }
    // render the view
    resetView();	
}

void BlackWhirl::colorAndScaleRepresentation(pqDataRepresentation *rep){ // s134862
    // get the scalarName for coloring purposes
    pqOutputPort* port = rep->getOutputPortFromInput();
    vtkPVDataInformation *dataInfo = port->getDataInformation();
    vtkPVDataSetAttributesInformation* arrayAttInfo = dataInfo->GetCellDataInformation();
    vtkPVArrayInformation* arrayInfo = arrayAttInfo->GetArrayInformation(0);
    char* scalarName = arrayInfo->GetName();
    // get the proxy
    vtkSMProxy* repProxy = rep->getProxy();
    // set coloring and rescale view to data
    vtkSMPVRepresentationProxy::SetScalarColoring(repProxy, scalarName,  
                                                  vtkDataObject::CELL);
    vtkSMPVRepresentationProxy::RescaleTransferFunctionToDataRange(repProxy); 
}

void BlackWhirl::hideRepresentations(pqPipelineSource* source){ // s134835
    QList<pqDataRepresentation*> representations = source->getRepresentations(view);
    foreach(pqDataRepresentation* representation, representations){
        representation->setVisible(false);
    } 
    resetView();
}

void BlackWhirl::loadFieldSerie(QStringList series){ // s134862
    pipelinesFields << pqLoadDataReaction::loadData(series);
}

void BlackWhirl::loadWellSerie(QStringList series){ // s132106
    pipelinesWells << pqLoadDataReaction::loadData(series);
    wellList << series;
}

QList<QStringList> BlackWhirl::getWellList(){ // s132106
    return wellList;
}

void BlackWhirl::clearSources(){ // s134835
    cout << "in 1" << endl;
    objectBuilder->destroySources(server);
    cout << "in 2" << endl;
    pipelinesWells.clear();
    cout << "in 3" << endl;
    pipelinesFields.clear();
    cout << "in 4" << endl;
    activeWell = NULL; 
    cout << "in 5" << endl;
}

bool BlackWhirl::hasActiveWell() { // s134862
    if (activeWell){
        return 1;
    } else return 0;
}

void BlackWhirl::hideActiveWell() { 
    hideRepresentations(activeWell);
    resetView();
}

void BlackWhirl::setActiveWell(QStringList strlist) { // s134835
    std:: cout << " in activewell method"<< endl;
    int index = wellList.indexOf(strlist);
    std::cout << "found index" << endl;
    activeWell = pipelinesWells[index];
    std::cout<< "leaving activewell" << endl;
}

bool BlackWhirl::activeWellIsVisable() { // s132106
    bool check = 0; 
    if(hasActiveWell()){
        QList<pqDataRepresentation*> wellRep = activeWell->getRepresentations(view);
        foreach(pqDataRepresentation* rep, wellRep){
            if(rep->isVisible()){
                check = 1;
            }
        }
    }
    return check;
}
