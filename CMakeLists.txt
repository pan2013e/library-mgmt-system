cmake_minimum_required(VERSION 3.5)

project(LibraryMgmt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/Users/panzhiyuan/Qt/5.15.2/clang_64")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        login.cpp
        login.h
        login.ui
        mgmt.cpp
        mgmt.h
        mgmt.ui
        license.cpp
        license.h
        license.ui
        store.h
        store.cpp
        store.ui
        search.cpp
        search.ui
        search.h
        pch.h
        transact.cpp
        transact.h
        transact.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LibraryMgmt
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(LibraryMgmt SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(LibraryMgmt
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_precompile_headers(LibraryMgmt PRIVATE pch.h)
set_target_properties(LibraryMgmt PROPERTIES UNITY_BUILD ON)

INCLUDE_DIRECTORIES (/usr/local/Cellar/mysql++/3.2.5_1/include/)
INCLUDE_DIRECTORIES (/usr/local/include)
INCLUDE_DIRECTORIES (/usr/local/Cellar/boost/1.75.0/include/)

LINK_DIRECTORIES (/usr/local/lib)
LINK_DIRECTORIES (/usr/local/Cellar/mysql++/3.2.5_1/lib/)
LINK_DIRECTORIES (/usr/local/Cellar/boost/1.75.0/lib/)

target_link_libraries(LibraryMgmt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets /usr/local/Cellar/mysql++/3.2.5_1/lib/libmysqlpp.dylib /usr/local/Cellar/mysql-client/8.0.22/lib/libmysqlclient.a)
