cmake_minimum_required(VERSION 3.5)

project(task-tracker-frontend LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES_FRONTEND task-tracker-frontend/task-tracker-frontend_ru_RU.ts)
set(TS_FILES_FRONTEND task-tracker-backend/task-tracker-backend_ru_RU.ts)

set(PROJECT_SOURCES_FRONTEND
  task-tracker-frontend/main.cpp
  task-tracker-frontend/MainWindow.cpp
  task-tracker-frontend/MainWindow.h
  task-tracker-frontend/MainWindow.ui
  ${TS_FILES_FRONTEND}
)

set(PROJECT_SOURCES_BACKEND
  task-tracker-backend/main.cpp
  ${TS_FILES_BACKEND}
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(task-tracker-frontend
    ${PROJECT_SOURCES_FRONTEND}
  )

  qt_add_executable(task-tracker-backend
    ${PROJECT_SOURCES_BACKEND}
  )

  qt_create_translation(QM_FILES
    ${CMAKE_SOURCE_DIR}/task-tracker-frontend
    ${TS_FILES_FRONTEND}
    ${CMAKE_SOURCE_DIR}/task-tracker-backend
    ${TS_FILES_BACKEND}
  )
else()
  add_executable(task-tracker-frontend
    ${PROJECT_SOURCES_FRONTEND}
  )

  add_executable(task-tracker-backend
    ${PROJECT_SOURCES_BACKEND}
  )

  qt5_create_translation(QM_FILES
    ${CMAKE_SOURCE_DIR}/task-tracker-frontend
    ${TS_FILES_FRONTEND}
    ${CMAKE_SOURCE_DIR}/task-tracker-backend
    ${TS_FILES_BACKEND}
  )
endif()

target_link_libraries(task-tracker-frontend PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(task-tracker-backend PRIVATE Qt${QT_VERSION_MAJOR}::Core)
