package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/role"])
class RoleController {
    data class CreateResult(val status: Boolean, val role: Role? = null)
    data class EditResult(val status: Boolean, val role: Role? = null)
    data class DeleteResult(val status: Boolean)
    data class AllResult(val status: Boolean, val roles: Set<Role> = emptySet())

    @Autowired
    private lateinit var roleRepository: RoleRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "value") value: String,
        @RequestParam(value = "permissions") permissions: ByteArray
    ): CreateResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (user.validUntil < Date()) {
            return CreateResult(false)
        }

        val projectUserRole = user.projects.find { it.project.id == projectId } ?: return CreateResult(false)

        if (!projectUserRole.role.checkPermission(Permission.MANAGE_ROLES)) {
            return CreateResult(false)
        }

        var role = Role()
        role.project = projectUserRole.project
        role.value = value
        role.permissions = permissions

        try {
            role = roleRepository.save(role)
        } catch (ex: Exception) {
            return CreateResult(false)
        }

        return CreateResult(true, role)
    }

    @PostMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "role_id") roleId: Int,
        @RequestParam(value = "value") value: String?,
        @RequestParam(value = "permissions") permissions: ByteArray?
    ): EditResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return EditResult(false)
        if (user.validUntil < Date()) {
            return EditResult(false)
        }

        val roleOptional = roleRepository.findById(roleId)
        if (roleOptional.isEmpty) {
            return EditResult(false)
        }
        var role = roleOptional.get()

        val projectUserRole = user.projects.find { it.project.id == role.project.id } ?: return EditResult(false)

        if (!projectUserRole.role.checkPermission(Permission.MANAGE_ROLES)) {
            return EditResult(false)
        }

        if (value != null) {
            role.value = value
        }

        if (permissions != null) {
            role.permissions = permissions
        }

        try {
            role = roleRepository.save(role)
        } catch (ex: Exception) {
            return EditResult(false)
        }

        return EditResult(true, role)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "role_id") roleId: Int
    ): DeleteResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (user.validUntil < Date()) {
            return DeleteResult(false)
        }

        val roleOptional = roleRepository.findById(roleId)
        if (roleOptional.isEmpty) {
            return DeleteResult(false)
        }
        val role = roleOptional.get()

        val projectUserRole = user.projects.find { it.project.id == role.project.id } ?: return DeleteResult(false)

        if (!projectUserRole.role.checkPermission(Permission.MANAGE_ROLES)) {
            return DeleteResult(false)
        }

        try {
            roleRepository.deleteById(roleId)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }

        return DeleteResult(true)
    }

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AllResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllResult(false)
        if (user.validUntil < Date()) {
            return AllResult(false)
        }

        user.projects.find { it.project.id == projectId } ?: return AllResult(false)

        return AllResult(true, roleRepository.findAll().filter { it.project.id == projectId }.toSet())
    }
}