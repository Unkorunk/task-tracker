package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.ResponseBody

@Controller
@RequestMapping(path = ["/comment"])
class CommentController {
    @Autowired
    private lateinit var commentRepository: CommentRepository

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(): Iterable<Comment> {
        return commentRepository.findAll()
    }
}