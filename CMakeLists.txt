cmake_minimum_required(VERSION 3.5)

project(task-tracker LANGUAGES CXX)

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
  QtWaitingSpinner/waitingspinnerwidget.cpp
  QtWaitingSpinner/waitingspinnerwidget.h

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

  task-tracker-frontend/DataClasses.cpp
  task-tracker-frontend/DataClasses.h

  task-tracker-frontend/Backend.cpp
  task-tracker-frontend/Backend.h

  task-tracker-frontend/ProjectItemWidget.cpp
  task-tracker-frontend/ProjectItemWidget.h
  task-tracker-frontend/ProjectItemWidget.ui

  task-tracker-frontend/TaskItemWidget.cpp
  task-tracker-frontend/TaskItemWidget.h
  task-tracker-frontend/TaskItemWidget.ui

  task-tracker-frontend/CreateProjectDialog.cpp
  task-tracker-frontend/CreateProjectDialog.h
  task-tracker-frontend/CreateProjectDialog.ui

  task-tracker-frontend/LoadingBar.cpp
  task-tracker-frontend/LoadingBar.h
  task-tracker-frontend/LoadingBar.ui

  icons.qrc

  task-tracker-frontend/LabelPropertyWidget.cpp
  task-tracker-frontend/LabelPropertyWidget.h
  task-tracker-frontend/LabelPropertyWidget.ui

  task-tracker-frontend/DateTimePropertyWidget.cpp
  task-tracker-frontend/DateTimePropertyWidget.h
  task-tracker-frontend/DateTimePropertyWidget.ui

  task-tracker-frontend/UserSelectorWidget.cpp
  task-tracker-frontend/UserSelectorWidget.h
  task-tracker-frontend/UserSelectorWidget.ui

  task-tracker-frontend/IntegerSelectorWidget.cpp
  task-tracker-frontend/IntegerSelectorWidget.h
  task-tracker-frontend/IntegerSelectorWidget.ui

  task-tracker-frontend/RoleWidgetItem.cpp
  task-tracker-frontend/RoleWidgetItem.h
  task-tracker-frontend/RoleWidgetItem.ui

  task-tracker-frontend/UserItemWidget.cpp
  task-tracker-frontend/UserItemWidget.h
  task-tracker-frontend/UserItemWidget.ui

  ${TS_FILES_FRONTEND}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(task-tracker-frontend
    ${PROJECT_SOURCES_FRONTEND}
  )

  qt_create_translation(QM_FILES
    ${CMAKE_SOURCE_DIR}/task-tracker-frontend
    ${TS_FILES_FRONTEND}
  )
else()
  add_executable(task-tracker-frontend
    ${PROJECT_SOURCES_FRONTEND}
  )

  qt5_create_translation(QM_FILES
    ${CMAKE_SOURCE_DIR}/task-tracker-frontend
    ${TS_FILES_FRONTEND}
  )
endif()

target_link_libraries(task-tracker-frontend PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(task-tracker-frontend PRIVATE Qt${QT_VERSION_MAJOR}::Network)
