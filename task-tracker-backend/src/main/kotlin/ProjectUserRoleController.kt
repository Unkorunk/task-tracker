package timelimit.main

import org.slf4j.LoggerFactory
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/projectUserRole"])
class ProjectUserRoleController {
    data class CreateResult(val status: Boolean)
    data class DeleteResult(val status: Boolean)
    data class EditResult(val status: Boolean)

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

    @PostMapping(path = ["/createById"])
    @ResponseBody
    fun createById(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "user_id") userId: Int,
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "role_id") roleId: Int
    ): CreateResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (sender.validUntil < Date()) {
            return CreateResult(false)
        }

        val senderRole = sender.projects.find { it.project.id == projectId }?.role ?: return CreateResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_TEAM)) {
            return CreateResult(false)
        }

        val userOptional = userRepository.findById(userId)
        if (userOptional.isEmpty) {
            return CreateResult(false)
        }
        val user = userOptional.get()

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return CreateResult(false)
        }
        val project = projectOptional.get()

        val projectUserRole = ProjectUserRole()
        projectUserRole.user = user
        projectUserRole.project = project
        val roleOptional = roleRepository.findById(roleId)
        if (roleOptional.isEmpty) {
            return CreateResult(false)
        }
        projectUserRole.role = roleOptional.get()

        if (!projectUserRole.role.isSubset(senderRole)) {
            return CreateResult(false)
        }

        try {
            projectUserRoleRepository.save(projectUserRole)
        } catch (ex: Exception) {
            return CreateResult(false)
        }

        notifyAdd(project, user)

        return CreateResult(true)
    }

    @PostMapping(path = ["/createByEmail"])
    @ResponseBody
    fun createByEmail(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "email") email: String,
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "role_id") roleId: Int
    ): CreateResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (sender.validUntil < Date()) {
            return CreateResult(false)
        }

        val senderRole = sender.projects.find { it.project.id == projectId }?.role ?: return CreateResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_TEAM)) {
            return CreateResult(false)
        }

        val user = userRepository.findByEmail(email) ?: return CreateResult(false)

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return CreateResult(false)
        }
        val project = projectOptional.get()

        val projectUserRole = ProjectUserRole()
        projectUserRole.user = user
        projectUserRole.project = project
        val roleOptional = roleRepository.findById(roleId)
        if (roleOptional.isEmpty) {
            return CreateResult(false)
        }
        projectUserRole.role = roleOptional.get()

        if (!projectUserRole.role.isSubset(senderRole)) {
            return CreateResult(false)
        }

        try {
            projectUserRoleRepository.save(projectUserRole)
        } catch (ex: Exception) {
            return CreateResult(false)
        }

        notifyAdd(project, user)

        return CreateResult(true)
    }

    @PostMapping(path = ["/createByScreenName"])
    @ResponseBody
    fun createByScreenName(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "screen_name") screenName: String,
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "role_id") roleId: Int
    ): CreateResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (sender.validUntil < Date()) {
            return CreateResult(false)
        }

        val senderRole = sender.projects.find { it.project.id == projectId }?.role ?: return CreateResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_TEAM)) {
            return CreateResult(false)
        }

        val user = userRepository.findByScreenName(screenName) ?: return CreateResult(false)

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return CreateResult(false)
        }
        val project = projectOptional.get()

        val projectUserRole = ProjectUserRole()
        projectUserRole.user = user
        projectUserRole.project = project
        val roleOptional = roleRepository.findById(roleId)
        if (roleOptional.isEmpty) {
            return CreateResult(false)
        }
        projectUserRole.role = roleOptional.get()

        if (!projectUserRole.role.isSubset(senderRole)) {
            return CreateResult(false)
        }

        try {
            projectUserRoleRepository.save(projectUserRole)
        } catch (ex: Exception) {
            return CreateResult(false)
        }

        notifyAdd(project, user)

        return CreateResult(true)
    }

    @GetMapping(path = ["/deleteById"])
    @ResponseBody
    fun deleteById(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "user_id") userId: Int,
        @RequestParam(value = "project_id") projectId: Int
    ): DeleteResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (sender.validUntil < Date()) {
            return DeleteResult(false)
        }

        val senderRole = sender.projects.find { it.project.id == projectId }?.role ?: return DeleteResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_TEAM)) {
            return DeleteResult(false)
        }

        val projectUserRole = projectUserRoleRepository.findByProjectIdAndUserId(projectId, userId)
            ?: return DeleteResult(false)

        if (!projectUserRole.role.isSubset(senderRole)) {
            return DeleteResult(false)
        }

        try {
            projectUserRoleRepository.deleteById(projectUserRole.id)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }

        notifyRemove(projectUserRole.project, projectUserRole.user)

        return DeleteResult(true)
    }

    @GetMapping(path = ["/deleteByEmail"])
    @ResponseBody
    fun deleteByEmail(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "email") email: String,
        @RequestParam(value = "project_id") projectId: Int
    ): DeleteResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (sender.validUntil < Date()) {
            return DeleteResult(false)
        }

        val senderRole = sender.projects.find { it.project.id == projectId }?.role ?: return DeleteResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_TEAM)) {
            return DeleteResult(false)
        }

        val user = userRepository.findByEmail(email) ?: return DeleteResult(false)

        val projectUserRole = projectUserRoleRepository.findByProjectIdAndUserId(projectId, user.id)
            ?: return DeleteResult(false)

        if (!projectUserRole.role.isSubset(senderRole)) {
            return DeleteResult(false)
        }

        try {
            projectUserRoleRepository.deleteById(projectUserRole.id)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }

        notifyRemove(projectUserRole.project, projectUserRole.user)

        return DeleteResult(true)
    }

    @GetMapping(path = ["/deleteByScreenName"])
    @ResponseBody
    fun deleteByScreenName(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "screen_name") screenName: String,
        @RequestParam(value = "project_id") projectId: Int
    ): DeleteResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (sender.validUntil < Date()) {
            return DeleteResult(false)
        }

        val senderRole = sender.projects.find { it.project.id == projectId }?.role ?: return DeleteResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_TEAM)) {
            return DeleteResult(false)
        }

        val user = userRepository.findByScreenName(screenName) ?: return DeleteResult(false)

        val projectUserRole = projectUserRoleRepository.findByProjectIdAndUserId(projectId, user.id)
            ?: return DeleteResult(false)

        if (!projectUserRole.role.isSubset(senderRole)) {
            return DeleteResult(false)
        }

        try {
            projectUserRoleRepository.deleteById(projectUserRole.id)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }

        notifyRemove(projectUserRole.project, projectUserRole.user)

        return DeleteResult(true)
    }

    @PostMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "user_id") userId: Int,
        @RequestParam(value = "role_id") roleId: Int
    ): EditResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return EditResult(false)
        if (sender.validUntil < Date()) {
            return EditResult(false)
        }

        val roleOptional = roleRepository.findById(roleId)
        if (roleOptional.isEmpty) {
            return EditResult(false)
        }
        val role = roleOptional.get()

        val senderRole = sender.projects.find { it.project.id == role.project.id }?.role ?: return EditResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_TEAM)) {
            return EditResult(false)
        }

        val userOptional = userRepository.findById(userId)
        if (userOptional.isEmpty) {
            return EditResult(false)
        }
        val user = userOptional.get()

        val projectUserRole = user.projects.find { it.project.id == role.project.id } ?: return EditResult(false)
        projectUserRole.role = role

        try {
            projectUserRoleRepository.save(projectUserRole)
        } catch (ex: Exception) {
            return EditResult(false)
        }

        return EditResult(true)
    }
}