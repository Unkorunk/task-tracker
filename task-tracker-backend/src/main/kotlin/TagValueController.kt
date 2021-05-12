package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.RequestParam
import org.springframework.web.bind.annotation.ResponseBody
import java.lang.Exception

@Controller
@RequestMapping(path = ["/tagValue"])
class TagValueController {
    data class CreateTagValueResult(val status: Boolean, val tagValue: TagValue? = null)
    data class DeleteTagValueResult(val status: Boolean)
    data class EditTagValueResult(val status: Boolean)

    @Autowired
    private lateinit var tagValueRepository: TagValueRepository

    @Autowired
    private lateinit var tagCaptionRepository: TagCaptionRepository

    @GetMapping(path = ["/create"])
    @ResponseBody
    fun create(
        @RequestParam(value = "tag_caption_id") tagCaptionId: Int,
        @RequestParam(value = "value") value: String
    ): CreateTagValueResult {
        val tagCaptionOptional = tagCaptionRepository.findById(tagCaptionId)
        if (tagCaptionOptional.isEmpty) {
            return CreateTagValueResult(false)
        }
        val tagCaption = tagCaptionOptional.get()

        var tagValue = TagValue()
        tagValue.tagCaption = tagCaption
        tagValue.value = value

        try {
            tagValue = tagValueRepository.save(tagValue)
        } catch (ex: Exception) {
            return CreateTagValueResult(false)
        }

        return CreateTagValueResult(true, tagValue)
    }

    @GetMapping(path = ["/delete"])
    @ResponseBody
    fun delete(@RequestParam(value = "tag_value_id") tagValueId: Int): DeleteTagValueResult {
        try {
            tagValueRepository.deleteById(tagValueId)
        } catch (ex: Exception) {
            return DeleteTagValueResult(false)
        }
        return DeleteTagValueResult(true)
    }

    @GetMapping(path = ["/edit"])
    @ResponseBody
    fun edit(
        @RequestParam(value = "tag_value_id") tagValueId: Int,
        @RequestParam(value = "value") value: String?
    ): EditTagValueResult {
        val tagValueOptional = tagValueRepository.findById(tagValueId)
        if (tagValueOptional.isEmpty) {
            return EditTagValueResult(false)
        }

        val tagValue = tagValueOptional.get()
        if (value != null) {
            tagValue.value = value
        }

        try {
            tagValueRepository.save(tagValue)
        } catch (ex: Exception) {
            return EditTagValueResult(false)
        }

        return EditTagValueResult(true)
    }

    @GetMapping(path = ["/all"])
    @ResponseBody
    fun getAll(): Iterable<TagValue> {
        return tagValueRepository.findAll()
    }
}