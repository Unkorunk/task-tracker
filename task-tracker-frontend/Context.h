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

    QList<QPair<UserInfo, RoleInfo>> GetProjectTeam() const;
    void SetProjectTeam(const QList<QPair<UserInfo, RoleInfo>>& team);

    QList<TagInfo> GetTags() const;
    void SetTags(const QList<TagInfo>& tags);

    TaskInfo GetTask() const;
    void SetTask(const TaskInfo& task);

private:
    UserInfo myUser;
    RoleInfo myRole;

    ProjectInfo myProject;
    QList<QPair<UserInfo, RoleInfo>> myProjectTeam;
    QList<TagInfo> myProjectTags;

    TaskInfo myTask;
};

#endif // APPLICATIONSTATE_H
