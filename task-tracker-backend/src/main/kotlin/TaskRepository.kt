package timelimit.main

import org.springframework.data.repository.CrudRepository

interface TaskRepository : CrudRepository<Task, Int> {}