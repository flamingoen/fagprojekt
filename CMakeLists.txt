# Makelistfile for BlackWhirl
# No specific student have been working on this

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

IF(NOT ParaView_BINARY_DIR)
  set(ParaView_DIR /home/obel/DTU/fagprojekt/paraview/paraview-build/)
  FIND_PACKAGE(ParaView REQUIRED)
  INCLUDE(${PARAVIEW_USE_FILE})
ENDIF()

IF(PARAVIEW_BUILD_QT_GUI)
  IF (PARAVIEW_QT_VERSION VERSION_GREATER "4")
    QT5_WRAP_CPP(MOC_SRCS UILayer.h)
    QT5_WRAP_UI(UI_SRCS BlackWhirl.ui)
  ELSE ()
    QT4_WRAP_CPP(MOC_SRCS UILayer.h)
    QT4_WRAP_UI(UI_SRCS BlackWhirl.ui)
  ENDIF ()

  ADD_PARAVIEW_DOCK_WINDOW(
    OUTIFACES
    OUTSRCS
    CLASS_NAME UILayer
    DOCK_AREA Right)

file(GLOB_RECURSE sources "${PROJECT_SOURCE_DIR}/*.cxx")                            

  ADD_PARAVIEW_PLUGIN(Blackwhirl "1.17"
                      GUI_INTERFACES ${OUTIFACES}
                      GUI_SOURCES ${OUTSRCS} ${MOC_SRCS} ${UI_SRCS} ${sources})
ENDIF()
