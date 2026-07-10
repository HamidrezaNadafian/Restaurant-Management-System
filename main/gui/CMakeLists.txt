cmake_minimum_required(VERSION 3.16)

project(gui VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
    roleselectionwindow.cpp
    roleselectionwindow.h
    roleselectionwindow.ui

    loginwindow.h
    loginwindow.cpp
    loginwindow.ui

    customerwindow.h
    customerwindow.cpp
    customerwindow.ui

    restaurantowner.h
    restaurantowner.cpp
    restaurantowner.ui

    adminwindow.h
    adminwindow.cpp
    adminwindow.ui

    main_window.cpp



    ../Menudb.cpp
    ../Restaurantdb.cpp
    ../Menu.cpp
    ../Restaurant.cpp
    ../Orderdb.cpp
    ../Login_and_SignUp.cpp
    ../SecretUserForAdmin.cpp
    ../customer.cpp
    ../sqlite3.c
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        customerwindow.h customerwindow.cpp customerwindow.ui
        restaurantowner.h restaurantowner.cpp restaurantowner.ui
        adminwindow.h adminwindow.cpp adminwindow.ui


    )
else()
    if(ANDROID)
        add_library(gui SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(gui
            ${PROJECT_SOURCES}
        )
    endif()
endif()


target_include_directories(gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.gui)
endif()
set_target_properties(gui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gui)
endif()