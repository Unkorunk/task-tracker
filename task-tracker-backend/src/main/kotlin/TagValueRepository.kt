package timelimit.main

import org.springframework.data.repository.CrudRepository

interface TagValueRepository : CrudRepository<TagValue, Int> {}