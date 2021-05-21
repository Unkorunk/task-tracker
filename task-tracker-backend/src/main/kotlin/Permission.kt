package timelimit.main

enum class Permission(val bitsIndex: Set<Int>) {
    MANAGE_OWN_TASK(setOf(0)),
    MANAGE_ALL_TASKS(setOf(0, 1)),
    MANAGE_OWN_COMMENT(setOf(2)),
    MANAGE_ALL_COMMENTS(setOf(2, 3)),
    MANAGE_TEAM(setOf(4)),
    MANAGE_ROLES(setOf(5)),
    EDIT_PROJECT(setOf(6)),
    DELETE_PROJECT(setOf(7))
}