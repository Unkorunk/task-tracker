package timelimit.main

import org.slf4j.LoggerFactory
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.text.SimpleDateFormat
import java.util.*


@Controller
@RequestMapping(path = ["/task"])
class TaskController {
    data class CreateTaskResult(val status: Boolean, val task: Task? = null)
    data class DeleteTaskResult(val status: Boolean)
    data class EditTaskResult(val status: Boolean)
    data class AllTaskResult(val status: Boolean, val tasks: Set<Task> = emptySet())

    private val logger = LoggerFactory.getLogger(ProjectUserRoleController::class.java)

    @Autowired
    private lateinit var taskRepository: TaskRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @Autowired
    private lateinit var projectRepository: ProjectRepository

    @Autowired
    private lateinit var notificationRepository: NotificationRepository

    private fun notify(user: User, text: String) {
        val notification = Notification()
        notification.isRead = false
        notification.text = text
        notification.user = user
        try {
            notificationRepository.save(notification)
        } catch (ex: java.lang.Exception) {
            logger.error("error saving notification")
        }
    }

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "story_points") storyPoints: Int?,
        @RequestParam(value = "title") title: String,
        @RequestParam(value = "description") description: String,
        @RequestParam(value = "assigned_id") assignedId: Int?,
        @RequestParam(value = "deadline") deadline: String?
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
            if (task.assignedTo != null) {
                notify(task.assignedTo!!, "You have been assigned to the '${task.title}' task.")
            }
        }

        if (deadline != null) {
            val formatter = SimpleDateFormat("dd-M-yyyy hh:mm:ss a", Locale.ENGLISH)
            task.deadline = formatter.parse(deadline)
            if (task.assignedTo != null) {
                notify(
                    task.assignedTo!!,
                    "The deadline for the '${task.title}' task has been moved to '${task.deadline}'"
                )
            }
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
    @ResponseBody
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "task_id") taskId: Int,
        @RequestParam(value = "story_points") storyPoints: Int?,
        @RequestParam(value = "title") title: String?,
        @RequestParam(value = "description") description: String?,
        @RequestParam(value = "assigned_id") assignedId: Int?,
        @RequestParam(value = "deadline") deadline: String?
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
            if (task.assignedTo != null) {
                notify(task.assignedTo!!, "You have been removed from the '${task.title}' task.")
            }
            task.assignedTo = assignedUser
            if (task.assignedTo != null) {
                notify(task.assignedTo!!, "You have been assigned to the '${task.title}' task.")
            }
        }

        if (deadline != null) {
            val formatter = SimpleDateFormat("dd-M-yyyy hh:mm:ss a", Locale.ENGLISH)
            task.deadline = formatter.parse(deadline)
            if (task.assignedTo != null) {
                notify(
                    task.assignedTo!!,
                    "The deadline for the '${task.title}' task has been moved to '${task.deadline}'"
                )
            }
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
    fun allAssignedTasks(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AllTaskResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllTaskResult(false)
        if (user.validUntil < Date()) {
            return AllTaskResult(false)
        }

        return AllTaskResult(
            true,
            user.assignedTasks.filter { it.project.id == projectId }.toSet()
        ) // todo in one query
    }

    @GetMapping(path = ["/allCreatedTasks"])
    @ResponseBody
    fun allCreatedTasks(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AllTaskResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllTaskResult(false)
        if (user.validUntil < Date()) {
            return AllTaskResult(false)
        }

        return AllTaskResult(true, user.createdTasks.filter { it.project.id == projectId }.toSet()) // todo in one query
    }

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun all(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AllTaskResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllTaskResult(false)
        if (user.validUntil < Date()) {
            return AllTaskResult(false)
        }

        val projectUserRole = user.projects.find { it.project.id == projectId } ?: return AllTaskResult(false)

        return AllTaskResult(true, projectUserRole.project.tasks)
    }
}