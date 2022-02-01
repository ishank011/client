add_library(newwizard OBJECT
    pages/AbstractSetupWizardPage.h
    pages/AbstractSetupWizardPage.cpp

    pages/ServerUrlSetupWizardPage.ui
    pages/ServerUrlSetupWizardPage.h
    pages/ServerUrlSetupWizardPage.cpp

    SetupWizardWindow.ui
    SetupWizardWindow.h
    SetupWizardWindow.cpp

    SetupWizardController.h
    SetupWizardController.cpp

    SetupWizardAccountBuilder.cpp
    SetupWizardAccountBuilder.h

    Pagination.cpp
    Pagination.h
)
target_link_libraries(newwizard PUBLIC Qt5::Widgets libsync)
set_target_properties(newwizard PROPERTIES AUTOUIC ON AUTORCC ON)
target_include_directories(newwizard PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
