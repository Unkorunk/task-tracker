cmake_minimum_required(VERSION 3.5)

project(task-tracker LANGUAGES CXX)

add_subdirectory(qthttpserver)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

set(TS_FILES_FRONTEND task-tracker-frontend/task-tracker-frontend_ru_RU.ts)

set(PROJECT_SOURCES_FRONTEND
  task-tracker-frontend/main.cpp
  task-tracker-frontend/MainWindow.cpp
  task-tracker-frontend/MainWindow.h
  task-tracker-frontend/MainWindow.ui

  task-tracker-frontend/NavBar.cpp
  task-tracker-frontend/NavBar.h
  task-tracker-frontend/NavBar.ui

  task-tracker-frontend/GreetingsWidget.cpp
  task-tracker-frontend/GreetingsWidget.h
  task-tracker-frontend/GreetingsWidget.ui

  task-tracker-frontend/ProfileWidget.cpp
  task-tracker-frontend/ProfileWidget.h
  task-tracker-frontend/ProfileWidget.ui

  task-tracker-frontend/NotificationsWidget.cpp
  task-tracker-frontend/NotificationsWidget.h
  task-tracker-frontend/NotificationsWidget.ui

  task-tracker-frontend/ProjectWidget.cpp
  task-tracker-frontend/ProjectWidget.h
  task-tracker-frontend/ProjectWidget.ui

  task-tracker-frontend/ProjectSettingsWidget.cpp
  task-tracker-frontend/ProjectSettingsWidget.h
  task-tracker-frontend/ProjectSettingsWidget.ui

  task-tracker-frontend/IssueWidget.cpp
  task-tracker-frontend/IssueWidget.h
  task-tracker-frontend/IssueWidget.ui

  task-tracker-frontend/StatisticsWidget.cpp
  task-tracker-frontend/StatisticsWidget.h
  task-tracker-frontend/StatisticsWidget.ui

  task-tracker-frontend/AuthorizationWindow.cpp
  task-tracker-frontend/AuthorizationWindow.h
  task-tracker-frontend/AuthorizationWindow.ui

  task-tracker-frontend/AuthorizationWidget.cpp
  task-tracker-frontend/AuthorizationWidget.h
  task-tracker-frontend/AuthorizationWidget.ui

  task-tracker-frontend/RegistrationWidget.cpp
  task-tracker-frontend/RegistrationWidget.h
  task-tracker-frontend/RegistrationWidget.ui

  task-tracker-frontend/ProjectsList.cpp
  task-tracker-frontend/ProjectsList.h
  task-tracker-frontend/ProjectsList.ui

  task-tracker-frontend/Backend.cpp
  task-tracker-frontend/Backend.h

  task-tracker-frontend/ProjectItemWidget.cpp
  task-tracker-frontend/ProjectItemWidget.h
  task-tracker-frontend/ProjectItemWidget.ui

  task-tracker-frontend/TaskItemWidget.cpp
  task-tracker-frontend/TaskItemWidget.h
  task-tracker-frontend/TaskItemWidget.ui

  ${TS_FILES_FRONTEND}
)

set(PROJECT_SOURCES_BACKEND
  task-tracker-backend/main.cpp
  task-tracker-backend/Project.hpp
  task-tracker-backend/Account.hpp
  task-tracker-backend/Task.hpp
  task-tracker-backend/TaskComment.hpp
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
  )
endif()

target_link_libraries(task-tracker-frontend PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(task-tracker-frontend PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(task-tracker-backend PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql Qt::HttpServer)
