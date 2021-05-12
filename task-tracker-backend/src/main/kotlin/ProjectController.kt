package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.util.*

@Controller
@RequestMapping(path = ["/project"])
class ProjectController {
    data class CreateProjectResult(val status: Boolean)
    data class DeleteProjectResult(val status: Boolean)
    data class EditProjectResult(val status: Boolean)
    data class ProjectRolePair(val project: Project, val role: Role)
    data class AllProjectResult(val status: Boolean, val projects: Set<ProjectRolePair> = emptySet())

    @Autowired
    private lateinit var projectRepository: ProjectRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @Autowired
    private lateinit var projectUserRoleRepository: ProjectUserRoleRepository

    @Autowired
    private lateinit var roleRepository: RoleRepository

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "full_name") fullName: String,
        @RequestParam(value = "photo") photo: String?
    ): CreateProjectResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return CreateProjectResult(false)

        if (user.validUntil < Date()) {
            return CreateProjectResult(false)
        }

        var project = Project()
        project.fullName = fullName
        project.photo = photo

        try {
            project = projectRepository.save(project)
        } catch (ex: Exception) {
            return CreateProjectResult(false)
        }

        val projectUserRole = ProjectUserRole()
        projectUserRole.project = project
        projectUserRole.user = user
        projectUserRole.role = roleRepository.findAll().firstOrNull() ?: return CreateProjectResult(false) // todo

        try {
            projectUserRoleRepository.save(projectUserRole)
        } catch (ex: Exception) {
            return CreateProjectResult(false)
        }

        return CreateProjectResult(true)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): DeleteProjectResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return DeleteProjectResult(false)

        if (user.validUntil < Date()) {
            return DeleteProjectResult(false)
        }

        // todo make custom query
        // todo check user role
        val projectUserRole = projectUserRoleRepository.findAll().find { it.project.id == projectId && it.user == user }
            ?: return DeleteProjectResult(false)

        try {
            projectRepository.deleteById(projectId)
        } catch (ex: Exception) {
            return DeleteProjectResult(false)
        }

        return DeleteProjectResult(true)
    }

    @PostMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "full_name") fullName: String?,
        @RequestParam(value = "photo") photo: String?
    ): EditProjectResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return EditProjectResult(false)

        if (user.validUntil < Date()) {
            return EditProjectResult(false)
        }

        val projectOptional = projectRepository.findById(projectId)

        if (projectOptional.isEmpty) {
            return EditProjectResult(false)
        }

        val project = projectOptional.get()

        // todo make custom query
        // todo check user role
        val projectUserRole = projectUserRoleRepository.findAll().find { it.project == project && it.user == user }
            ?: return EditProjectResult(false)

        if (fullName != null) {
            project.fullName = fullName
        }

        if (photo != null) {
            project.photo = photo
        }

        try {
            projectRepository.save(project)
        } catch (ex: Exception) {
            return EditProjectResult(false)
        }

        return EditProjectResult(true)
    }

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun all(@RequestParam(value = "access_token") accessToken: String): AllProjectResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllProjectResult(false)

        if (user.validUntil < Date()) {
            return AllProjectResult(false)
        }

        return AllProjectResult(true, user.projects.map { ProjectRolePair(it.project, it.role) }.toSet())
    }
}