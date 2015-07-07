#ifndef __BlackWhirl_h
#define __BlackWhirl_h

#include "crawler.h"
#include "ui_BlackWhirl.h"
#include "UILayer.h"

#include <QFileDialog>
#include <QString>
#include <QStringList>

#include <pqApplicationCore.h>
#include <pqObjectBuilder.h>
#include <pqServer.h>
#include <pqServerManagerModel.h>
#include <pqActiveObjects.h>
#include <pqDataRepresentation.h>
#include <pqDefaultMainWindow.h>
#include <pqDisplayPolicy.h>
#include <pqLoadDataReaction.h>
#include <pqObjectBuilder.h>
#include <pqPVApplicationCore.h>
#include <pqPipelineSource.h>
#include <pqRenderView.h>
#include <pqView.h>
#include <pqPipelineRepresentation.h>
#include <pqCoreUtilities.h>
#include <pqPipelineModel.h>

#include <vtkSMPVRepresentationProxy.h>
#include <vtkSMPropertyHelper.h>
#include <vtkSMProxyIterator.h>
#include <vtkSMSession.h>
#include <vtkSMSessionProxyManager.h>
#include <vtkPVDataInformation.h>
#include <vtkPVDataSetAttributesInformation.h>
#include <vtkPVArrayInformation.h>
#include <vtkSMPVRepresentationProxy.h>
#include <vtkSMRepresentationProxy.h>
#include <vtkDataObject.h>

class BlackWhirl {
    
public:
    BlackWhirl();
    void import(QString folder);
    QList<pqPipelineSource*> filterListBy(std::string str);
    void resetView();
    void addOrShowRepresentations(pqPipelineSource* source);
    void addRepresentation(pqPipelineSource* source);
    void showRepresentations(pqPipelineSource* source);
    void colorAndScaleRepresentation(pqDataRepresentation *rep);
    void hideRepresentations(pqPipelineSource* source);
    void loadFieldSerie(QStringList series);
    void loadWellSerie(QStringList series);
    void clearSources();
    void hideAllWells();
    void hideAllFields();
    void hideAll(QList<pqPipelineSource*> sources);
    void showAllWells(QList<QStringList> list);
    bool hasActiveWell();
    void hideActiveWell();
    void addOrShowActiveWell();
    void setActiveWell(QStringList strlist);
    bool activeWellIsVisable();
    void addOrShowField(int index);
    QList<QStringList> getWellList();
    
};

#endif
