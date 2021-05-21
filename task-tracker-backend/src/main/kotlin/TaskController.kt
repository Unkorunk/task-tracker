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
    data class CreateResult(val status: Boolean, val task: Task? = null)
    data class DeleteResult(val status: Boolean)
    data class EditResult(val status: Boolean, val task: Task? = null)
    data class AllResult(val status: Boolean, val tasks: Set<Task> = emptySet())

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
    ): CreateResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (user.validUntil < Date()) {
            return CreateResult(false)
        }

        val role = user.projects.find { it.project.id == projectId }?.role ?: return CreateResult(false)
        if (!role.checkPermission(Permission.MANAGE_OWN_TASK)) {
            return CreateResult(false)
        }

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return CreateResult(false)
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
                return CreateResult(false)
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
            return CreateResult(false)
        }

        return CreateResult(true, task)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "task_id") taskId: Int
    ): DeleteResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (user.validUntil < Date()) {
            return DeleteResult(false)
        }

        val taskOptional = taskRepository.findById(taskId)
        if (taskOptional.isEmpty) {
            return DeleteResult(false)
        }
        val task = taskOptional.get()

        val role = user.projects.find { it.project.id == task.project.id }?.role ?: return DeleteResult(false)

        val ownTask = role.checkPermission(Permission.MANAGE_OWN_TASK) && task.createdBy?.id == user.id
        if (!ownTask && !role.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return DeleteResult(false)
        }

        try {
            taskRepository.deleteById(taskId)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }
        return DeleteResult(true)
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
    ): EditResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return EditResult(false)
        if (user.validUntil < Date()) {
            return EditResult(false)
        }

        val taskOptional = taskRepository.findById(taskId)
        if (taskOptional.isEmpty) {
            return EditResult(false)
        }
        var task = taskOptional.get()

        val role = user.projects.find { it.project.id == task.project.id }?.role ?: return EditResult(false)

        val ownTask = role.checkPermission(Permission.MANAGE_OWN_TASK) && task.createdBy?.id == user.id
        if (!ownTask && !role.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return EditResult(false)
        }

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
                return EditResult(false)
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
            val formatter = SimpleDateFormat("dd-MM-yyyy HH:mm:ss.SSS", Locale.ENGLISH)
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
            return EditResult(false)
        }

        return EditResult(true, task)
    }

    @GetMapping(path = ["/allAssignedTasks"])
    @ResponseBody
    fun allAssignedTasks(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AllResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllResult(false)
        if (user.validUntil < Date()) {
            return AllResult(false)
        }

        return AllResult(
            true,
            user.assignedTasks.filter { it.project.id == projectId }.toSet()
        )
    }

    @GetMapping(path = ["/allCreatedTasks"])
    @ResponseBody
    fun allCreatedTasks(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AllResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllResult(false)
        if (user.validUntil < Date()) {
            return AllResult(false)
        }

        return AllResult(true, user.createdTasks.filter { it.project.id == projectId }.toSet())
    }

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun all(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AllResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllResult(false)
        if (user.validUntil < Date()) {
            return AllResult(false)
        }

        val projectUserRole = user.projects.find { it.project.id == projectId } ?: return AllResult(false)

        return AllResult(true, projectUserRole.project.tasks)
    }
}