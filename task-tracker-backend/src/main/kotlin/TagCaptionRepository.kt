package timelimit.main

import org.springframework.data.repository.CrudRepository

interface TagCaptionRepository : CrudRepository<TagCaption, Int> {}