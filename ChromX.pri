include(ChromX.prf)
include(ChromX_AppInfo.pri)

!isEmpty(CHROMX_PRI_INCLUDED):error("ChromX.pri already included")
CHROMX_PRI_INCLUDED = 1

CONFIG += c++14

isEmpty(PRO_LIBRARY_BASENAME) {
    PRO_LIBRARY_BASENAME = lib
}

# Project Source Path
PRO_SOURCE_TREE = $$PWD
# Project 3rdparty Path
PRO_THREEPARTY_PATH = $$PRO_SOURCR_TREE/3rdparty
# Project Shard File Path
PRO_SOURCE_SHARD = $$PRO_SOURCE_TREE/shard
# Project Scripts File Path
PRO_SOURCE_SCRIPTS = $$PRO_SOURCE_TREE/scripts

# Project Build Path
isEmpty(PRO_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    PRO_BUILD_TREE = $$clean_path($$OUT_PWD)
    PRO_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}
# Project Bin Path
PRO_APP_PATH = $$PRO_BUILD_TREE/bin
# App Target Name
PRO_TARGET   = $$APP_ID

osx{
    exists($$PRO_BUILD_TREE/Contents/MacOS/$$PRO_TARGET): PRO_APP_BUNDLE = $$PRO_BUILD_TREE
    else: PRO_APP_BUNDLE = $$PRO_APP_PATH/$${PRO_TARGET}.app
    # target output path if not set manually
    isEmpty(PRO_OUTPUT_PATH): PRO_OUTPUT_PATH = $$PRO_APP_BUNDLE/Contents

    PRO_LIBRARY_PATH = $$PRO_OUTPUT_PATH/Frameworks
    PRO_PLUGIN_PATH  = $$PRO_LIBRARY_PATH/PlugIns
    PRO_DATA_PATH    = $$PRO_OUTPUT_PATH/Resources
    PRO_DOC_PATH     = $$PRO_DATA_PATH/doc
    PRO_BIN_PATH     = $$PRO_OUTPUT_PATH/MacOS


}else{
    # target output path if not set manually
    isEmpty(PRO_OUTPUT_PATH): PRO_OUTPUT_PATH = $$PRO_BUILD_TREE

    PRO_LIBRARY_PATH = $$PRO_OUTPUT_PATH/$$PRO_LIBRARY_BASENAME/$$PRO_PRO_TARGET
    PRO_PLUGIN_PATH  = $$PRO_LIBRARY_PATH/plugins
    PRO_DATA_PATH    = $$PRO_OUTPUT_PATH/share/$$PRO_PRO_TARGET
    PRO_DOC_PATH     = $$PRO_OUTPUT_PATH/share/doc/$$PRO_PRO_TARGET
    PRO_BIN_PATH     = $$PRO_OUTPUT_PATH/bin

}

INCLUDEPATH += \
    $$PRO_BUILD_TREE/src \ # for <app/app_version.h> in case of actual build directory
    $$PRO_SOURCE_TREE/src \ # for <app/app_version.h> in case of binary package with dev package
    $$PRO_SOURCE_SHARD #for shard file
    $$PRO_SOURCE_SCRIPTS #for scripts file

win32{
    VLD_PATH= $$quote(C:\\Program Files (x86)\\Visual Leak Detector\\include)
    exists($$VLD_PATH){
        INCLUDEPATH += $$VLD_PATH
        contains(QT_ARCH, i386) {
                LIBS += $$quote(C:\Program Files (x86)\Visual Leak Detector\lib\Win32\vld.lib)
            } else {
                LIBS += $$quote(C:\Program Files (x86)\Visual Leak Detector\lib\Win64\vld.lib)
            }

    }
}

# For ChromX Control Engine Inclue File Path
CHROMX_CONTROL_ENGINE_INCLUDE= $$PRO_THREEPARTY_PATH/include/ChromXControlEngine
INCLUDEPATH += $$CHROMX_CONTROL_ENGINE_INCLUDE

# ChromX Dependencies Library Path
exists($$PRO_LIBRARY_PATH): LIBS *= -L$$PRO_LIBRARY_PATH


######################################### <<Checking For Dependencies>> #########################################
# Search Assembly Dependencies of ChromX
PRO_DEPLIB_DIRS += $$PRO_SOURCE_TREE/src

# Search Assembly Dependencies of ChormX Control Engine
PRO_DEPLIB_DIRS += $$PRO_THREEPARTY_PATH/src/ChromXControlEngine

# recursively resolve library deps
done_libs =
for(ever) {
    isEmpty(PRO_LIB_DEPENDS): \
        break()
    done_libs += $$PRO_LIB_DEPENDS
    for(dep, PRO_LIB_DEPENDS) {
        dependencies_file =
        for(dir, PRO_DEPLIB_DIRS) {
            exists($$dir/$$dep/$${dep}_dependencies.pri) {
                dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
                break()
            }
        }
        isEmpty(dependencies_file): \
            error("Library dependency $$dep not found")
        include($$dependencies_file)
        LIBS += -l$$qtLibraryTargetName($$PRO_LIB_NAME)
        osx:QMAKE_LFLAGS += -l$$qtLibraryTargetName($$PRO_LIB_NAME)
    }
    PRO_LIB_DEPENDS = $$unique(PRO_LIB_DEPENDS)
    PRO_LIB_DEPENDS -= $$unique(done_libs)
}

win32:{
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}
