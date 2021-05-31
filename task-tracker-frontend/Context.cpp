#include "Context.h"

const UserInfo Context::DEFAULT_USER = UserInfo(-1, "", "", "");
const RoleInfo Context::DEFAULT_ROLE = RoleInfo(-1, "", 0, -1);
const ProjectInfo Context::DEFAULT_PROJECT = ProjectInfo(-1, "");
const TaskInfo Context::DEFAULT_TASK = TaskInfo(-1, DEFAULT_PROJECT, "", "", std::optional<UserInfo>(), QDateTime(), 0);

Context::Context() : myUser(DEFAULT_USER), myRole(DEFAULT_ROLE),
    myProject(DEFAULT_PROJECT), myTask(-1, DEFAULT_PROJECT, "", "", std::optional<UserInfo>(), QDateTime(), 0) { }

UserInfo Context::GetUser() const {
    return myUser;
}

void Context::SetUser(const UserInfo &user) {
    myUser = user;
}

RoleInfo Context::GetCurrentRole() const {
    return myRole;
}

void Context::SetRole(const RoleInfo &role) {
    myRole = role;
}

ProjectInfo Context::GetCurrentProject() const {
    return myProject;
}

void Context::SetProject(const ProjectInfo &project) {
    myProject = project;
}

TaskInfo Context::GetTask() const {
    return myTask;
}

void Context::SetTask(const TaskInfo &task) {
    myTask = task;
}
