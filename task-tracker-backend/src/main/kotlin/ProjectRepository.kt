package timelimit.main

import org.springframework.data.repository.CrudRepository

interface ProjectRepository : CrudRepository<Project, Int> {}