package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.ResponseBody

@Controller
@RequestMapping(path = ["/role"])
class RoleController {
    @Autowired
    private lateinit var roleRepository: RoleRepository

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(): Iterable<Role> {
        return roleRepository.findAll()
    }
}