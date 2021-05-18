package timelimit.main

import org.springframework.data.jpa.repository.Query
import org.springframework.data.repository.CrudRepository

interface ProjectUserRoleRepository : CrudRepository<ProjectUserRole, Int> {
    @Query(value = "select u from ProjectUserRole u where u.project.id = ?1 and u.user.id = ?2")
    fun findByProjectIdAndUserId(projectId: Int, userId: Int): ProjectUserRole?
}