package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/task"])
class TaskController {
    data class CreateTaskResult(val status: Boolean, val task: Task? = null)
    data class DeleteTaskResult(val status: Boolean)
    data class EditTaskResult(val status: Boolean)
    data class AllTaskResult(val status: Boolean, val tasks: Set<Task> = emptySet())

    @Autowired
    private lateinit var taskRepository: TaskRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @Autowired
    private lateinit var projectRepository: ProjectRepository

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "story_points") storyPoints: Int?,
        @RequestParam(value = "title") title: String,
        @RequestParam(value = "description") description: String,
        @RequestParam(value = "assigned_id") assignedId: Int?
    ): CreateTaskResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return CreateTaskResult(false)
        if (user.validUntil < Date()) {
            return CreateTaskResult(false)
        }

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return CreateTaskResult(false)
        }
        val project = projectOptional.get()

        var task = Task()
        task.createdBy = user
        task.createdAt = Date()
        task.updatedBy = user
        task.updatedAt = task.createdAt
        task.title = title
        task.description = description
        task.project = project

        if (storyPoints != null) {
            task.storyPoints = storyPoints
        }

        if (assignedId != null) {
            val assignedUserOptional = userRepository.findById(assignedId)
            if (assignedUserOptional.isEmpty) {
                return CreateTaskResult(false)
            }
            val assignedUser = assignedUserOptional.get()
            task.assignedTo = assignedUser
        }

        try {
            task = taskRepository.save(task)
        } catch (ex: Exception) {
            return CreateTaskResult(false)
        }

        return CreateTaskResult(true, task)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(@RequestParam(value = "task_id") taskId: Int): DeleteTaskResult {
        try {
            taskRepository.deleteById(taskId)
        } catch (ex: Exception) {
            return DeleteTaskResult(false)
        }
        return DeleteTaskResult(true)
    }

    @PostMapping(path = ["/edit"])
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "task_id") taskId: Int,
        @RequestParam(value = "story_points") storyPoints: Int?,
        @RequestParam(value = "title") title: String?,
        @RequestParam(value = "description") description: String?,
        @RequestParam(value = "assigned_id") assignedId: Int?
    ): EditTaskResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return EditTaskResult(false)
        if (user.validUntil < Date()) {
            return EditTaskResult(false)
        }

        val taskOptional = taskRepository.findById(taskId)
        if (taskOptional.isEmpty) {
            return EditTaskResult(false)
        }
        val task = taskOptional.get()

        task.updatedBy = user
        task.updatedAt = Date()

        if (storyPoints != null) {
            task.storyPoints = storyPoints
        }

        if (title != null) {
            task.title = title
        }

        if (description != null) {
            task.description = description
        }

        if (assignedId != null) {
            val assignedUserOptional = userRepository.findById(assignedId)
            if (assignedUserOptional.isEmpty) {
                return EditTaskResult(false)
            }
            val assignedUser = assignedUserOptional.get()
            task.assignedTo = assignedUser
        }

        try {
            taskRepository.save(task)
        } catch (ex: Exception) {
            return EditTaskResult(false)
        }

        return EditTaskResult(true)
    }

    @GetMapping(path = ["/allAssignedTasks"])
    @ResponseBody
    fun allAssignedTasks(@RequestParam(value = "access_token") accessToken: String): AllTaskResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllTaskResult(false)
        if (user.validUntil < Date()) {
            return AllTaskResult(false)
        }

        return AllTaskResult(true, user.assignedTasks)
    }

    @GetMapping(path = ["/allCreatedTasks"])
    @ResponseBody
    fun allCreatedTasks(@RequestParam(value = "access_token") accessToken: String): AllTaskResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllTaskResult(false)
        if (user.validUntil < Date()) {
            return AllTaskResult(false)
        }

        return AllTaskResult(true, user.createdTasks)
    }
}