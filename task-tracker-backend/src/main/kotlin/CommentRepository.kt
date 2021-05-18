package timelimit.main

import org.springframework.data.repository.CrudRepository

interface CommentRepository : CrudRepository<Comment, Int> {}