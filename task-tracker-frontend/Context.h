#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

#include "DataClasses.h"


class Context
{
public:
    static const UserInfo DEFAULT_USER;
    static const ProjectInfo DEFAULT_PROJECT;
    static const RoleInfo DEFAULT_ROLE;
    static const TaskInfo DEFAULT_TASK;

    Context();

    UserInfo GetUser() const;
    void SetUser(const UserInfo& user = DEFAULT_USER);

    RoleInfo GetCurrentRole() const;
    void SetRole(const RoleInfo& role = DEFAULT_ROLE);

    ProjectInfo GetCurrentProject() const;
    void SetProject(const ProjectInfo& project = DEFAULT_PROJECT);

    // make set in setproject
    QList<UserInfo> GetProjectTeam() const;

    TaskInfo GetTask() const;
    void SetTask(const TaskInfo& task);

private:
    UserInfo myUser;
    RoleInfo myRole;

    ProjectInfo myProject;
    QList<UserInfo> myProjectTeam;

    TaskInfo myTask;
};

#endif // APPLICATIONSTATE_H
