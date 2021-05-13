package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.RequestParam
import org.springframework.web.bind.annotation.ResponseBody
import java.lang.Exception

@Controller
@RequestMapping(path = ["/tagCaption"])
class TagCaptionController {
    data class CreateTagCaptionResult(val status: Boolean, val tagCaption: TagCaption? = null)
    data class DeleteTagCaptionResult(val status: Boolean)
    data class EditTagCaptionResult(val status: Boolean)

    @Autowired
    private lateinit var tagCaptionRepository: TagCaptionRepository

    @Autowired
    private lateinit var projectRepository: ProjectRepository

    @GetMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "project_id") projectId: Int,
        @RequestParam(value = "caption") caption: String
    ): CreateTagCaptionResult {
        val projectOptional = projectRepository.findById(projectId)
        if (projectOptional.isEmpty) {
            return CreateTagCaptionResult(false)
        }
        val project = projectOptional.get()

        var tagCaption = TagCaption()
        tagCaption.project = project
        tagCaption.caption = caption

        try {
            tagCaption = tagCaptionRepository.save(tagCaption)
        } catch (ex: Exception) {
            return CreateTagCaptionResult(false)
        }

        return CreateTagCaptionResult(true, tagCaption)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(@RequestParam(value = "tag_caption_id") tagCaptionId: Int): DeleteTagCaptionResult {
        try {
            tagCaptionRepository.deleteById(tagCaptionId)
        } catch (ex: Exception) {
            return DeleteTagCaptionResult(false)
        }
        return DeleteTagCaptionResult(true)
    }

    @GetMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "tag_caption_id") tagCaptionId: Int,
        @RequestParam(value = "caption") caption: String?
    ): EditTagCaptionResult {
        val tagCaptionOptional = tagCaptionRepository.findById(tagCaptionId)
        if (tagCaptionOptional.isEmpty) {
            return EditTagCaptionResult(false)
        }

        val tagCaption = tagCaptionOptional.get()
        if (caption != null) {
            tagCaption.caption = caption
        }

        try {
            tagCaptionRepository.save(tagCaption)
        } catch (ex: Exception) {
            return EditTagCaptionResult(false)
        }

        return EditTagCaptionResult(true)
    }

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(): Iterable<TagCaption> {
        return tagCaptionRepository.findAll()
    }
}