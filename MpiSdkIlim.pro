QT += core gui widgets
QT += serialport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 
# static release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QXLSX_PARENTPATH=./QXlsx
QXLSX_HEADERPATH=./QXlsx/header/
QXLSX_SOURCEPATH=./QXlsx/source/
include(./QXlsx.pri)

INCLUDEPATH += $$PWD/Src/CustomChart

SOURCES += \
    Src/ReportBuilders/ReportBlocks/ResolutionBlock.cpp \
    Src/ReportBuilders/ReportBlocks/TechnicalResultsBlock.cpp \
    main.cpp \
    Src/Ui/Setup/ObjectWindow.cpp \
    Src/Ui/Setup/ValveWindow.cpp \
    Src/Ui/Setup/SelectTests.cpp \
    Src/Ui/MainWindow/MainWindow.cpp \
    Src/Ui/TestSettings/OtherTestSettings.cpp \
    Src/Ui/TestSettings/StepTestSettings.cpp \
    Src/Ui/TestSettings/MainTestSettings.cpp \
    Src/CustomChart/MyChart.cpp \
    Src/CustomChart/MySeries.cpp \
    Src/LabeledSlider/LabeledSlider.cpp \
    Src/Mpi/Mpi.cpp \
    Src/Mpi/MpiSettings.cpp \
    Src/ReportBuilders/ReportBuilder.cpp \
    Src/ReportBuilders/ReportSaver.cpp \
    Src/Runners/BaseRunner.cpp \
    Src/Runners/MainTestRunner.cpp \
    Src/Runners/OptionResolutionRunner.cpp \
    Src/Runners/OptionResponseRunner.cpp \
    Src/Runners/RunnerFactory.cpp \
    Src/Runners/StepTestRunner.cpp \
    Src/Runners/StrokeTestRunner.cpp \
    Src/Domain/Tests/MainTest.cpp \
    Src/Domain/Tests/OptionTest.cpp \
    Src/Domain/Tests/StepTest.cpp \
    Src/Domain/Tests/StrokeTest.cpp \
    Src/Domain/Tests/Test.cpp \
    Src/Uart/Uart.cpp \
    Src/Uart/UartMessage.cpp \
    Src/Uart/UartReader.cpp \
    Src/ValidatorFactory/ValidatorFactory.cpp \
    Src/Domain/Program.cpp \
    Src/Storage/Registry.cpp \
    Src/Domain/Sensor.cpp

HEADERS += \
    Src/ReportBuilders/ReportBlocks/IReportBlock.h \
    Src/ReportBuilders/ReportBlocks/MaterialsBlock.h \
    Src/ReportBuilders/ReportBlocks/ObjectInfoBlock.h \
    Src/ReportBuilders/ReportBlocks/ResolutionBlock.h \
    Src/ReportBuilders/ReportBlocks/ResponseBlock.h \
    Src/ReportBuilders/ReportBlocks/StepReactionBlock.h \
    Src/ReportBuilders/ReportBlocks/TechnicalResultsBlock.h \
    Src/ReportBuilders/ReportBlocks/ValveSpecBlock.h \
    Src/ReportBuilders/ReportWriter.h \
    Src/Ui/Setup/ObjectWindow.h \
    Src/Ui/Setup/ValveWindow.h \
    Src/Ui/Setup/SelectTests.h \
    Src/Ui/TestSettings/AbstractTestSettings.h \
    Src/Ui/TestSettings/OtherTestSettings.h \
    Src/Ui/TestSettings/StepTestSettings.h \
    Src/Ui/TestSettings/MainTestSettings.h \
    Src/Ui/MainWindow/MainWindow.h \
    Src/Domain/Program.h \
    Src/Storage/Registry.h \
    Src/Domain/Sensor.h \
    Src/CustomChart/MyChart.h \
    Src/CustomChart/MySeries.h \
    Src/LabeledSlider/LabeledSlider.h \
    Src/Mpi/Mpi.h \
    Src/Mpi/MpiSettings.h \
    Src/ReportBuilders/ReportBuilder.h \
    Src/ReportBuilders/ReportSaver.h \
    Src/Runners/AbstractTestRunner.h \
    Src/Runners/BaseRunner.h \
    Src/Runners/MainTestRunner.h \
    Src/Runners/OptionResolutionRunner.h \
    Src/Runners/OptionResponseRunner.h \
    Src/Runners/RunnerFactory.h \
    Src/Runners/StepTestRunner.h \
    Src/Runners/StrokeTestRunner.h \
    Src/Storage/Telemetry.h \
    Src/Domain/Tests/MainTest.h \
    Src/Domain/Tests/OptionTest.h \
    Src/Domain/Tests/StepTest.h \
    Src/Domain/Tests/StrokeTest.h \
    Src/Domain/Tests/Test.h \
    Src/Uart/Uart.h \
    Src/Uart/UartMessage.h \
    Src/Uart/UartReader.h \
    Src/ValidatorFactory/RegexPatterns.h \
    Src/ValidatorFactory/ValidatorFactory.h

FORMS += \
    Src/Ui/Setup/ObjectWindow.ui \
    Src/Ui/Setup/SelectTests.ui \
    Src/Ui/Setup/ValveWindow.ui \
    Src/Ui/MainWindow/MainWindow.ui \
    Src/Ui/TestSettings/MainTestSettings.ui \
    Src/Ui/TestSettings/OtherTestSettings.ui \
    Src/Ui/TestSettings/StepTestSettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    excel.qrc \
    img.qrc \
    translations.qrc
