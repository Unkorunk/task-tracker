package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.ResponseBody

@Controller
@RequestMapping(path = ["/notification"])
class NotificationController {
    @Autowired
    private lateinit var notificationRepository: NotificationRepository

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(): Iterable<Notification> {
        return notificationRepository.findAll()
    }
}