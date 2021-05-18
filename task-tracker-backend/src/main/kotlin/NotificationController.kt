package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.RequestParam
import org.springframework.web.bind.annotation.ResponseBody
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/notification"])
class NotificationController {
    data class AllResult(val status: Boolean, val notifications: Set<Notification> = emptySet())
    data class UnreadResult(val status: Boolean, val notifications: Set<Notification> = emptySet())
    data class ClearReadResult(val status: Boolean)
    data class ClearAllResult(val status: Boolean)

    @Autowired
    private lateinit var notificationRepository: NotificationRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(@RequestParam(value = "access_token") accessToken: String): AllResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return AllResult(false)
        if (user.validUntil < Date()) {
            return AllResult(false)
        }

        return AllResult(true, notificationRepository.getAll(user))
    }

    @GetMapping(path = ["/unread"])
    @ResponseBody
    fun getUnread(@RequestParam(value = "access_token") accessToken: String): UnreadResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return UnreadResult(false)
        if (user.validUntil < Date()) {
            return UnreadResult(false)
        }

        val notifications = notificationRepository.getUnread(user)
        notifications.forEach { it.isRead = true }
        try {
            notificationRepository.saveAll(notifications)
        } catch (ex: Exception) {
            return UnreadResult(false)
        }
        notifications.forEach { it.isRead = false }

        return UnreadResult(true, notifications)
    }

    @GetMapping(path = ["/clearRead"])
    @ResponseBody
    fun clearRead(@RequestParam(value = "access_token") accessToken: String): ClearReadResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return ClearReadResult(false)
        if (user.validUntil < Date()) {
            return ClearReadResult(false)
        }

        notificationRepository.clearRead(user)

        return ClearReadResult(true)
    }

    @GetMapping(path = ["/clearAll"])
    @ResponseBody
    fun clearAll(@RequestParam(value = "access_token") accessToken: String): ClearAllResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return ClearAllResult(false)
        if (user.validUntil < Date()) {
            return ClearAllResult(false)
        }

        notificationRepository.clearAll(user)

        return ClearAllResult(true)
    }
}