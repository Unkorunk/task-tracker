package timelimit.main

import org.springframework.data.repository.CrudRepository

interface ProjectUserRoleRepository : CrudRepository<ProjectUserRole, Int> {}