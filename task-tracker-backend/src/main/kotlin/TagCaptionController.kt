package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.*
import java.lang.Exception
import java.util.*

@Controller
@RequestMapping(path = ["/tagCaption"])
class TagCaptionController {
    data class CreateResult(val status: Boolean, val tagCaption: TagCaption? = null)
    data class DeleteResult(val status: Boolean)
    data class EditResult(val status: Boolean)
    data class AllResult(val status: Boolean, val tagCaptions: Set<TagCaption> = emptySet())

    @Autowired
    private lateinit var tagCaptionRepository: TagCaptionRepository

    @Autowired
    private lateinit var projectRepository: ProjectRepository

    @Autowired
    private lateinit var userRepository: UserRepository

    @PostMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "caption") caption: String
    ): CreateResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return CreateResult(false)
        if (sender.validUntil < Date()) {
            return CreateResult(false)
        }

        val senderRole = sender.projects.find { it.project.id == projectId }?.role ?: return CreateResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return CreateResult(false)
        }

        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return CreateResult(false)
        }
        val project = projectOptional.get()

        var tagCaption = TagCaption()
        tagCaption.project = project
        tagCaption.caption = caption

        try {
            tagCaption = tagCaptionRepository.save(tagCaption)
        } catch (ex: Exception) {
            return CreateResult(false)
        }

        return CreateResult(true, tagCaption)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "tag_caption_id") tagCaptionId: Int
    ): DeleteResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return DeleteResult(false)
        if (sender.validUntil < Date()) {
            return DeleteResult(false)
        }

        val tagCaptionOptional = tagCaptionRepository.findById(tagCaptionId)
        if (tagCaptionOptional.isEmpty) {
            return DeleteResult(false)
        }
        val tagCaption = tagCaptionOptional.get()

        val senderRole = sender.projects.find {
            it.project.id == tagCaption.project.id
        }?.role ?: return DeleteResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return DeleteResult(false)
        }

        try {
            tagCaptionRepository.deleteById(tagCaptionId)
        } catch (ex: Exception) {
            return DeleteResult(false)
        }
        return DeleteResult(true)
    }

    @PostMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "tag_caption_id") tagCaptionId: Int,
        @RequestParam(value = "caption") caption: String?
    ): EditResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return EditResult(false)
        if (sender.validUntil < Date()) {
            return EditResult(false)
        }

        val tagCaptionOptional = tagCaptionRepository.findById(tagCaptionId)
        if (tagCaptionOptional.isEmpty) {
            return EditResult(false)
        }
        val tagCaption = tagCaptionOptional.get()

        val senderRole = sender.projects.find {
            it.project.id == tagCaption.project.id
        }?.role ?: return EditResult(false)
        if (!senderRole.checkPermission(Permission.MANAGE_ALL_TASKS)) {
            return EditResult(false)
        }

        if (caption != null) {
            tagCaption.caption = caption
        }

        try {
            tagCaptionRepository.save(tagCaption)
        } catch (ex: Exception) {
            return EditResult(false)
        }

        return EditResult(true)
    }

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun all(
        @RequestParam(value = "access_token") accessToken: String,
        @RequestParam(value = "project_id") projectId: Int
    ): AllResult {
        val sender = userRepository.findByAccessToken(accessToken) ?: return AllResult(false)
        if (sender.validUntil < Date()) {
            return AllResult(false)
        }

        sender.projects.find { it.project.id == projectId }?.role ?: return AllResult(false)

        return AllResult(true, tagCaptionRepository.findAll().filter { it.project.id == projectId }.toSet())
    }
}