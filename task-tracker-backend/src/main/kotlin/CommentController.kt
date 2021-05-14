package timelimit.main

import org.hibernate.sql.Delete
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/comment"])
class CommentController {
    data class CreateResult(val status: Boolean, val comment: Comment? = null)
    data class DeleteResult(val status: Boolean)
    data class EditResult(val status: Boolean)

    @Autowired
    private lateinit var commentRepository: CommentRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @Autowired
    private lateinit var taskRepository: TaskRepository

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "task_id") taskId: Int,
        @RequestParam(value = "text") text: String
    ): CreateResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (user.validUntil < Date()) {
            return CreateResult(false)
        }

        val taskOptional = taskRepository.findById(taskId)
        if (taskOptional.isEmpty) {
            return CreateResult(false)
        }
        val task = taskOptional.get()

        var comment = Comment()
        comment.author = user
        comment.createdAt = Date()
        comment.task = task
        comment.text = text

        try {
            comment = commentRepository.save(comment)
        } catch (ex: Exception) {
            return CreateResult(false)
        }

        return CreateResult(true, comment)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "comment_id") commentId: Int
    ): DeleteResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (user.validUntil < Date()) {
            return DeleteResult(false)
        }

        try {
            userRepository.deleteById(commentId)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }

        return DeleteResult(true)
    }

    @PostMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "comment_id") commentId: Int,
        @RequestParam(value = "text") text: String?
    ): EditResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return EditResult(false)
        if (user.validUntil < Date()) {
            return EditResult(false)
        }

        val commentOptional = commentRepository.findById(commentId)
        if (commentOptional.isEmpty) {
            return EditResult(false)
        }
        val comment = commentOptional.get()

        if (text != null) {
            comment.text = text
        }

        try {
            commentRepository.save(comment)
        } catch (ex: Exception) {
            return EditResult(false)
        }

        return EditResult(true)
    }

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(): Iterable<Comment> {
        return commentRepository.findAll()
    }
}