package timelimit.main

import org.springframework.data.repository.CrudRepository

interface TaskTagRepository : CrudRepository<TaskTag, Int> {}