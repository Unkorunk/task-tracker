package timelimit.main

import org.springframework.data.repository.CrudRepository

interface RoleRepository : CrudRepository<Role, Int> {}