include(ECMAddAppIcon)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

if (WIN32)
    find_package(Qt5 REQUIRED COMPONENTS WinExtras)
endif()

add_subdirectory(updater)

set(client_UI_SRCS
    aboutdialog.ui
    accountsettings.ui
    folderwizardsourcepage.ui
    folderwizardtargetpage.ui
    generalsettings.ui
    ignorelisteditor.ui
    networksettings.ui
    protocolwidget.ui
    issueswidget.ui
    activitywidget.ui
    settingsdialog.ui
    sharedialog.ui
    sharelinkwidget.ui
    shareusergroupwidget.ui
    shareuserline.ui
    sslerrordialog.ui
    proxyauthdialog.ui
    notificationwidget.ui
    logbrowser.ui
    wizard/owncloudadvancedsetuppage.ui
    wizard/owncloudhttpcredspage.ui
    wizard/owncloudoauthcredspage.ui
    wizard/owncloudsetuppage.ui
)

set(client_SRCS
    aboutdialog.cpp
    accountmanager.cpp
    accountsettings.cpp
    application.cpp
    clientproxy.cpp
    connectionvalidator.cpp
    folder.cpp
    folderman.cpp
    folderstatusmodel.cpp
    folderstatusdelegate.cpp
    folderwatcher.cpp
    folderwizard.cpp
    generalsettings.cpp
    ignorelisteditor.cpp
    lockwatcher.cpp
    logbrowser.cpp
    networksettings.cpp
    ocsjob.cpp
    ocssharejob.cpp
    ocsshareejob.cpp
    openfilemanager.cpp
    owncloudgui.cpp
    owncloudsetupwizard.cpp
    protocolwidget.cpp
    protocolitem.cpp
    issueswidget.cpp
    activitydata.cpp
    activitywidget.cpp
    selectivesyncdialog.cpp
    settingsdialog.cpp
    sharedialog.cpp
    sharelinkwidget.cpp
    sharemanager.cpp
    shareusergroupwidget.cpp
    sharee.cpp
    sslbutton.cpp
    sslerrordialog.cpp
    syncrunfilelog.cpp
    systray.cpp
    thumbnailjob.cpp
    quotainfo.cpp
    accountstate.cpp
    authenticationdialog.cpp
    platform.cpp
    proxyauthhandler.cpp
    proxyauthdialog.cpp
    tooltipupdater.cpp
    notificationwidget.cpp
    notificationconfirmjob.cpp
    servernotificationhandler.cpp
    guiutility.cpp
    elidedlabel.cpp
    translations.cpp
    creds/httpcredentialsgui.cpp

    models/activitylistmodel.cpp
    models/expandingheaderview.cpp
    models/models.cpp
    models/protocolitemmodel.cpp

    wizard/postfixlineedit.cpp
    wizard/abstractcredswizardpage.cpp
    wizard/owncloudadvancedsetuppage.cpp
    wizard/owncloudhttpcredspage.cpp
    wizard/owncloudoauthcredspage.cpp
    wizard/owncloudsetuppage.cpp
    wizard/owncloudwizardcommon.cpp
    wizard/owncloudwizard.cpp
)

set(3rdparty_SRC
    ../3rdparty/QProgressIndicator/QProgressIndicator.cpp
    ../3rdparty/qtlockedfile/qtlockedfile.cpp
    ../3rdparty/qtsingleapplication/qtlocalpeer.cpp
    ../3rdparty/qtsingleapplication/qtsingleapplication.cpp
    ../3rdparty/qtsingleapplication/qtsinglecoreapplication.cpp
   )

if(NOT WIN32)
   list(APPEND 3rdparty_SRC ../3rdparty/qtlockedfile/qtlockedfile_unix.cpp)
else()
   list(APPEND 3rdparty_SRC ../3rdparty/qtlockedfile/qtlockedfile_win.cpp )
endif()

set( final_src
    ${MIRALL_RC_SRC}
    ${client_SRCS}
    ${client_UI_SRCS}
    ${3rdparty_SRC}
)

if(TARGET qt5keychain)
    list(APPEND libsync_LINK_TARGETS qt5keychain)
endif()

add_library(owncloudCore STATIC ${final_src})
set_target_properties(owncloudCore PROPERTIES AUTOUIC ON AUTORCC ON)
target_link_libraries(owncloudCore PUBLIC Qt5::Widgets Qt5::Network Qt5::Xml
                                          libsync)
target_compile_definitions(owncloudCore
    PUBLIC QT_USE_QSTRINGBUILDER
           QT_MESSAGELOGCONTEXT  # enable function name and line number in debug output
           QT_DEPRECATED_WARNINGS
           QT_NO_FOREACH
)

add_subdirectory(socketapi)

target_include_directories(owncloudCore PUBLIC
    ${CMAKE_SOURCE_DIR}/src/3rdparty/QProgressIndicator
    ${CMAKE_SOURCE_DIR}/src/3rdparty/qtlockedfile
    ${CMAKE_SOURCE_DIR}/src/3rdparty/qtsingleapplication
    ${CMAKE_CURRENT_BINARY_DIR}
)


target_sources(owncloudCore PRIVATE
    updater/ocupdater.cpp
    updater/updateinfo.cpp
    updater/updater.cpp
)

IF( APPLE )
    find_package(Qt5 COMPONENTS MacExtras)
    target_link_libraries(owncloudCore PUBLIC Qt5::MacExtras)

    target_sources(owncloudCore PRIVATE
            platform_mac.mm
            systray.mm
            settingsdialog_mac.mm
            guiutility_mac.mm
            folderwatcher_mac.cpp)

    if(SPARKLE_FOUND)
        # Define this, we need to check in updater.cpp
        target_compile_definitions(owncloudCore PUBLIC HAVE_SPARKLE)
        target_sources(owncloudCore PRIVATE
            updater/sparkleupdater_mac.mm
            updater/sparkleupdater.h)
        target_link_libraries(owncloudCore PRIVATE ${SPARKLE_LIBRARY})
   endif()
elseif( WIN32 )
    target_sources(owncloudCore PRIVATE
            platform_win.cpp
            guiutility_win.cpp
            folderwatcher_win.cpp
            navigationpanehelper.cpp)
    target_link_libraries(owncloudCore PUBLIC Qt5::WinExtras)
elseif(UNIX AND NOT APPLE )
    ## handle DBUS for Fdo notifications
    find_package(Qt5 COMPONENTS DBus)
    if (TARGET Qt5::DBus)
        target_link_libraries(owncloudCore PUBLIC Qt5::DBus)
        target_compile_definitions(owncloudCore PUBLIC "USE_FDO_NOTIFICATIONS")
    endif()
    target_sources(owncloudCore PRIVATE
            folderwatcher_linux.cpp
            platform_unix.cpp
            guiutility_unix.cpp)
endif()


if(WITH_CRASHREPORTER)
    target_link_libraries(owncloudCore PUBLIC crashreporter-handler)

    if(UNIX AND NOT MAC)
        find_package(Threads REQUIRED)
        target_link_libraries(owncloudCore PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    endif()
endif()

add_executable(owncloud main.cpp)
set_target_properties(owncloud PROPERTIES
    OUTPUT_NAME "${APPLICATION_EXECUTABLE}"
    AUTOUIC ON
    AUTORCC ON
)
target_link_libraries(owncloud owncloudCore)


find_package(Qt5LinguistTools)
if(Qt5LinguistTools_FOUND)
    # Handle Translations, pick all client_* files from trans directory.
    file(GLOB client_translations ${CMAKE_SOURCE_DIR}/translations/client_*.ts)
    qt5_add_translation(client_compiled_translations ${client_translations})
    target_sources(owncloud PRIVATE ${client_compiled_translations})
    add_resources_to_target(
        TARGET owncloud
        PREFIX translations
        FILES "${client_compiled_translations}"
    )
endif()

#TODO Move resources files
target_sources(owncloud PRIVATE
    ${PROJECT_SOURCE_DIR}/client.qrc
    ${PROJECT_SOURCE_DIR}/core_theme.qrc)

generate_theme(owncloud OWNCLOUD_SIDEBAR_ICONS)
MESSAGE(STATUS "OWNCLOUD_SIDEBAR_ICONS: ${APPLICATION_ICON_NAME}: ${OWNCLOUD_SIDEBAR_ICONS}")

ecm_add_app_icon(appIcons ICONS "${OWNCLOUD_ICONS}" SIDEBAR_ICONS "${OWNCLOUD_SIDEBAR_ICONS}" OUTFILE_BASENAME "${APPLICATION_ICON_NAME}")
target_sources(owncloud PRIVATE ${appIcons})

if(NOT APPLE)
    if(WIN32)
        configure_file(
          ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
          ${CMAKE_CURRENT_BINARY_DIR}/version.rc
          @ONLY)
        target_sources(owncloud PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
        if(NOT MSVC)
            target_sources(owncloud PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/manifest-mingw.rc)
        endif()
    else()
        file(GLOB _icons "${OEM_THEME_DIR}/theme/colored/*-${APPLICATION_ICON_NAME}-icon.png")
        foreach(_file ${_icons})
            string(REPLACE "${OEM_THEME_DIR}/theme/colored/" "" _res ${_file})
            string(REPLACE "-${APPLICATION_ICON_NAME}-icon.png" "" _res ${_res})
            install(FILES ${_file} RENAME ${APPLICATION_ICON_NAME}.png DESTINATION ${DATA_INSTALL_DIR}/icons/hicolor/${_res}x${_res}/apps)
        endforeach(_file)
    endif()

    # we may not add MACOSX_BUNDLE here, if not building one
else()
    target_sources(owncloud PRIVATE ${OWNCLOUD_BUNDLED_RESOURCES})

    set_source_files_properties(
      ${OWNCLOUD_BUNDLED_RESOURCES}
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
      )

  set_target_properties(owncloud PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist)
endif()

install(TARGETS owncloud ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

if(UNIX AND NOT APPLE)
    include(libcloudproviders/libcloudproviders.cmake)

    configure_file(${CMAKE_SOURCE_DIR}/mirall.desktop.in
                   ${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_EXECUTABLE}.desktop)
    install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_EXECUTABLE}.desktop DESTINATION ${DATA_INSTALL_DIR}/applications )

    configure_file(owncloud.xml.in ${APPLICATION_EXECUTABLE}.xml)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_EXECUTABLE}.xml DESTINATION ${DATA_INSTALL_DIR}/mime/packages )

    find_package(SharedMimeInfo)
    if(SharedMimeInfo_FOUND)
        update_xdg_mimetypes( ${DATA_INSTALL_DIR}/mime/packages )
    endif(SharedMimeInfo_FOUND)
endif()
