QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../3DViewer_v1.0.c \
    main.cpp \
    mainwindow.cpp\
    widget.cpp\
    giflib/dgif_lib.c\
    giflib/egif_lib.c\
    giflib/gif_err.c\
    giflib/gif_font.c\
    giflib/gif_hash.c\
    giflib/gifalloc.c\
    giflib/qgifimage.cpp\
    giflib/quantize.c

HEADERS += \
    ../3DViewer_v1.0.h \
    mainwindow.h\
    widget.h\
    giflib/gif_hash.h\
    giflib/gif_lib_private.h\
    giflib/gif_lib.h\
    giflib/qgifglobal.h\
    giflib/qgifimage_p.h\
    giflib/qgifimage.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    cat.obj \
    fshader.fsh \
    vshader.vsh

RESOURCES += \
    shaders.qrc
