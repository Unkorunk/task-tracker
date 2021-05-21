package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/taskTag"])
class TaskTagController {
    data class CreateResult(val status: Boolean, val taskTag: TaskTag? = null)
    data class DeleteResult(val status: Boolean)

    @Autowired
    private lateinit var taskTagRepository: TaskTagRepository

    @Autowired
    private lateinit var tagValueRepository: TagValueRepository

    @Autowired
    private lateinit var taskRepository: TaskRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "task_id") taskId: Int,
        @RequestParam(value = "tag_value_id") tagValueId: Int
    ): CreateResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (sender.validUntil < Date()) {
            return CreateResult(false)
        }

        val taskOptional = taskRepository.findById(taskId)
        if (taskOptional.isEmpty) {
            return CreateResult(false)
        }
        val task = taskOptional.get()

        val tagValueOptional = tagValueRepository.findById(tagValueId)
        if (tagValueOptional.isEmpty) {
            return CreateResult(false)
        }
        val tagValue = tagValueOptional.get()

        val senderRole = sender.projects.find {
            it.project.id == tagValue.tagCaption.project.id
        }?.role ?: return CreateResult(false)
        val ownTask = senderRole.checkPermission(Permission.MANAGE_OWN_TASK) && task.createdBy?.id == sender.id
        if (!ownTask && !senderRole.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return CreateResult(false)
        }

        var taskTag = TaskTag()
        taskTag.task = task
        taskTag.tagValue = tagValue

        try {
            taskTag = taskTagRepository.save(taskTag)
        } catch (ex: Exception) {
            return CreateResult(false)
        }

        return CreateResult(true, taskTag)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "task_tag_id") taskTagId: Int
    ): DeleteResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (sender.validUntil < Date()) {
            return DeleteResult(false)
        }

        val taskTagOptional = taskTagRepository.findById(taskTagId)
        if (taskTagOptional.isEmpty) {
            return DeleteResult(false)
        }
        val taskTag = taskTagOptional.get()

        val senderRole = sender.projects.find {
            it.project.id == taskTag.tagValue.tagCaption.project.id
        }?.role ?: return DeleteResult(false)
        val ownTask = senderRole.checkPermission(Permission.MANAGE_OWN_TASK) && taskTag.task.createdBy?.id == sender.id
        if (!ownTask && !senderRole.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return DeleteResult(false)
        }

        try {
            taskTagRepository.deleteById(taskTagId)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }
        return DeleteResult(true)
    }
}