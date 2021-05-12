package timelimit.main

import org.slf4j.LoggerFactory
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.RequestParam
import org.springframework.web.bind.annotation.ResponseBody
import java.lang.Exception

@Controller
@RequestMapping(path = ["/projectUserRole"])
class ProjectUserRoleController {
    data class AddResult(val status: Boolean)
    data class RemoveResult(val status: Boolean)

    private val logger = LoggerFactory.getLogger(ProjectUserRoleController::class.java)

    @Autowired
    private lateinit var projectUserRoleRepository: ProjectUserRoleRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @Autowired
    private lateinit var projectRepository: ProjectRepository

    @Autowired
    private lateinit var roleRepository: RoleRepository

    @Autowired
    private lateinit var notificationRepository: NotificationRepository

    private fun notifyAdd(project: Project, user: User) {
        notify(user, "You have been added to the '${project.fullName}' project.")
    }

    private fun notifyRemove(project: Project, user: User) {
        notify(user, "You have been removed from the '${project.fullName}' project.")
    }

    private fun notify(user: User, text: String) {
        val notification = Notification()
        notification.isRead = false
        notification.text = text
        notification.user = user
        try {
            notificationRepository.save(notification)
        } catch (ex: Exception) {
            logger.error("error saving notification")
        }
    }

    @GetMapping(path = ["/addById"])
    @ResponseBody
    fun addById(
        @RequestParam(value = "user_id") userId: Int,
        @RequestParam(value = "project_id") projectId: Int
    ): AddResult {
        val userOptional = userRepository.findById(userId)
        if (userOptional.isEmpty) {
            return AddResult(false)
        }
        val user = userOptional.get()

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return AddResult(false)
        }
        val project = projectOptional.get()

        val projectUserRole = ProjectUserRole()
        projectUserRole.user = user
        projectUserRole.project = project
        projectUserRole.role = roleRepository.findAll().firstOrNull() ?: return AddResult(false) // todo

        try {
            projectUserRoleRepository.save(projectUserRole)
        } catch (ex: Exception) {
            return AddResult(false)
        }

        notifyAdd(project, user)

        return AddResult(true)
    }

    @GetMapping(path = ["/addByEmail"])
    @ResponseBody
    fun addByEmail(
        @RequestParam(value = "email") email: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AddResult {
        val user = userRepository.findByEmail(email) ?: return AddResult(false)

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return AddResult(false)
        }
        val project = projectOptional.get()

        val projectUserRole = ProjectUserRole()
        projectUserRole.user = user
        projectUserRole.project = project
        projectUserRole.role = roleRepository.findAll().firstOrNull() ?: return AddResult(false) // todo

        try {
            projectUserRoleRepository.save(projectUserRole)
        } catch (ex: Exception) {
            return AddResult(false)
        }

        notifyAdd(project, user)

        return AddResult(true)
    }

    @GetMapping(path = ["/addByScreenName"])
    @ResponseBody
    fun addByScreenName(
        @RequestParam(value = "screen_name") screenName: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AddResult {
        val user = userRepository.findByScreenName(screenName) ?: return AddResult(false)

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return AddResult(false)
        }
        val project = projectOptional.get()

        val projectUserRole = ProjectUserRole()
        projectUserRole.user = user
        projectUserRole.project = project
        projectUserRole.role = roleRepository.findAll().firstOrNull() ?: return AddResult(false) // todo

        try {
            projectUserRoleRepository.save(projectUserRole)
        } catch (ex: Exception) {
            return AddResult(false)
        }

        notifyAdd(project, user)

        return AddResult(true)
    }

    @GetMapping(path = ["/removeById"])
    @ResponseBody
    fun removeById(
        @RequestParam(value = "user_id") userId: Int,
        @RequestParam(value = "project_id") projectId: Int
    ): RemoveResult {
        val projectUserRole = projectUserRoleRepository.findByProjectIdAndUserId(projectId, userId)
            ?: return RemoveResult(false)

        try {
            projectUserRoleRepository.deleteById(projectUserRole.id)
        } catch (ex: Exception) {
            return RemoveResult(false)
        }

        notifyRemove(projectUserRole.project, projectUserRole.user)

        return RemoveResult(true)
    }

    @GetMapping(path = ["/removeByEmail"])
    @ResponseBody
    fun removeByEmail(
        @RequestParam(value = "email") email: String,
        @RequestParam(value = "project_id") projectId: Int
    ): RemoveResult {
        val user = userRepository.findByEmail(email) ?: return RemoveResult(false)

        val projectUserRole = projectUserRoleRepository.findByProjectIdAndUserId(projectId, user.id)
            ?: return RemoveResult(false)

        try {
            projectUserRoleRepository.deleteById(projectUserRole.id)
        } catch (ex: Exception) {
            return RemoveResult(false)
        }

        notifyRemove(projectUserRole.project, projectUserRole.user)

        return RemoveResult(true)
    }

    @GetMapping(path = ["/removeByScreenName"])
    @ResponseBody
    fun removeByScreenName(
        @RequestParam(value = "screen_name") screenName: String,
        @RequestParam(value = "project_id") projectId: Int
    ): RemoveResult {
        val user = userRepository.findByScreenName(screenName) ?: return RemoveResult(false)

        val projectUserRole = projectUserRoleRepository.findByProjectIdAndUserId(projectId, user.id)
            ?: return RemoveResult(false)

        try {
            projectUserRoleRepository.deleteById(projectUserRole.id)
        } catch (ex: Exception) {
            return RemoveResult(false)
        }

        notifyRemove(projectUserRole.project, projectUserRole.user)

        return RemoveResult(true)
    }
}