package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/tagValue"])
class TagValueController {
    data class CreateResult(val status: Boolean, val tagValue: TagValue? = null)
    data class DeleteResult(val status: Boolean)
    data class EditResult(val status: Boolean, val tagValue: TagValue? = null)

    @Autowired
    private lateinit var tagValueRepository: TagValueRepository

    @Autowired
    private lateinit var tagCaptionRepository: TagCaptionRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "tag_caption_id") tagCaptionId: Int,
        @RequestParam(value = "value") value: String
    ): CreateResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (sender.validUntil < Date()) {
            return CreateResult(false)
        }

        val tagCaptionOptional = tagCaptionRepository.findById(tagCaptionId)
        if (tagCaptionOptional.isEmpty) {
            return CreateResult(false)
        }
        val tagCaption = tagCaptionOptional.get()

        val senderRole = sender.projects.find {
            it.project.id == tagCaption.project.id
        }?.role ?: return CreateResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return CreateResult(false)
        }

        var tagValue = TagValue()
        tagValue.tagCaption = tagCaption
        tagValue.value = value

        try {
            tagValue = tagValueRepository.save(tagValue)
        } catch (ex: Exception) {
            return CreateResult(false)
        }

        return CreateResult(true, tagValue)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "tag_value_id") tagValueId: Int
    ): DeleteResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (sender.validUntil < Date()) {
            return DeleteResult(false)
        }

        val tagValueOptional = tagValueRepository.findById(tagValueId)
        if (tagValueOptional.isEmpty) {
            return DeleteResult(false)
        }
        val tagValue = tagValueOptional.get()

        val senderRole = sender.projects.find {
            it.project.id == tagValue.tagCaption.project.id
        }?.role ?: return DeleteResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return DeleteResult(false)
        }

        try {
            tagValueRepository.deleteById(tagValueId)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }
        return DeleteResult(true)
    }

    @PostMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "tag_value_id") tagValueId: Int,
        @RequestParam(value = "value") value: String?
    ): EditResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return EditResult(false)
        if (sender.validUntil < Date()) {
            return EditResult(false)
        }

        val tagValueOptional = tagValueRepository.findById(tagValueId)
        if (tagValueOptional.isEmpty) {
            return EditResult(false)
        }
        var tagValue = tagValueOptional.get()

        val senderRole = sender.projects.find {
            it.project.id == tagValue.tagCaption.project.id
        }?.role ?: return EditResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return EditResult(false)
        }

        if (value != null) {
            tagValue.value = value
        }

        try {
            tagValue = tagValueRepository.save(tagValue)
        } catch (ex: Exception) {
            return EditResult(false)
        }

        return EditResult(true, tagValue)
    }
}