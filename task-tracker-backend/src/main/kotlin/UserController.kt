package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/user"])
class UserController {
    data class CreateUserResult(val status: Boolean, val user: User? = null, val accessToken: String? = null)
    data class DeleteUserResult(val status: Boolean)
    data class GetUserResult(val status: Boolean, val user: User? = null)
    data class EditUserResult(val status: Boolean, val user: User? = null)
    data class EnterResult(val status: Boolean, val user: User? = null, val accessToken: String? = null)

    @Autowired
    private lateinit var userRepository: UserRepository

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "full_name") fullName: String,
        @RequestParam(value = "screen_name") screenName: String,
        @RequestParam(value = "email") email: String,
        @RequestParam(value = "password") password: String
    ): CreateUserResult {
        var user = User()
        user.fullName = fullName
        user.screenName = screenName
        user.email = email
        user.updatePassword(password)
        user.updateToken()

        try {
            user = userRepository.save(user)
        } catch (ex: Exception) {
            return CreateUserResult(false)
        }

        return CreateUserResult(true, user, user.accessToken)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(
        @RequestParam(value = "access_token") accessToken: String
    ): DeleteUserResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return DeleteUserResult(false)

        if (user.validUntil < Date()) {
            return DeleteUserResult(false)
        }

        try {
            userRepository.deleteById(user.id)
        } catch (ex: Exception) {
            return DeleteUserResult(false)
        }

        return DeleteUserResult(true)
    }

    @GetMapping(path = ["/getById"])
    @ResponseBody
    fun getById(
        @RequestParam(value = "id") id: Int
    ): GetUserResult {
        val userOptional = userRepository.findById(id)

        if (userOptional.isEmpty) {
            return GetUserResult(false)
        }

        val user = userOptional.get()

        return GetUserResult(true, user)
    }

    @GetMapping(path = ["/getByAccessToken"])
    @ResponseBody
    fun getByAccessToken(
        @RequestParam(value = "access_token") accessToken: String
    ): GetUserResult {
        val user = userRepository.findByAccessToken(accessToken) ?: return GetUserResult(false)

        if (user.validUntil < Date()) {
            return GetUserResult(false)
        }

        return GetUserResult(true, user)
    }

    @GetMapping(path = ["/getByScreenName"])
    @ResponseBody
    fun getByScreenName(
        @RequestParam(value = "screen_name") screenName: String
    ): GetUserResult {
        val user = userRepository.findByScreenName(screenName) ?: return GetUserResult(false)

        return GetUserResult(true, user)
    }

    @PostMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "full_name") fullName: String?,
        @RequestParam(value = "screen_name") screenName: String?,
        @RequestParam(value = "email") email: String?,
        @RequestParam(value = "photo") photo: String?,
        @RequestParam(value = "password") password: String?
    ): EditUserResult {
        var user = userRepository.findByAccessToken(accessToken) ?: return EditUserResult(false)

        if (user.validUntil < Date()) {
            return EditUserResult(false)
        }

        if (fullName != null) {
            user.fullName = fullName
        }

        if (screenName != null) {
            user.screenName = screenName
        }

        if (email != null) {
            user.email = email
        }

        if (photo != null) {
            user.photo = photo
        }

        if (password != null) {
            user.updatePassword(password)
        }

        try {
            user = userRepository.save(user)
        } catch (ex: Exception) {
            return EditUserResult(false)
        }

        return EditUserResult(true, user)
    }

    @GetMapping(path = ["/enter"])
    @ResponseBody
    fun enter(
        @RequestParam(value = "screen_name") screenName: String,
        @RequestParam(value = "password") password: String
    ): EnterResult {
        val user = userRepository.findByScreenName(screenName) ?: return EnterResult(false)

        if (!user.comparePassword(password)) {
            return EnterResult(false)
        }

        user.updateToken()

        try {
            userRepository.save(user)
        } catch (ex: Exception) {
            return EnterResult(false)
        }

        return EnterResult(true, user, user.accessToken)
    }
}