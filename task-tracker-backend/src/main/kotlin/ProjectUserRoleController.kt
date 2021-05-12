package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.ResponseBody

@Controller
@RequestMapping(path = ["/projectUserRole"])
class ProjectUserRoleController {
    @Autowired
    private lateinit var projectUserRoleRepository: ProjectUserRoleRepository

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(): Iterable<ProjectUserRole> {
        return projectUserRoleRepository.findAll()
    }
}