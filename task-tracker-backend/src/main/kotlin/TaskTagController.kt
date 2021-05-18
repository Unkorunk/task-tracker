package timelimit.main

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Controller
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RequestMapping
import org.springframework.web.bind.annotation.RequestParam
import org.springframework.web.bind.annotation.ResponseBody
import java.lang.Exception

@Controller
@RequestMapping(path = ["/taskTag"])
class TaskTagController {
    data class AddTaskTagResult(val status: Boolean, val taskTag: TaskTag? = null)
    data class RemoveTaskTagResult(val status: Boolean)

    @Autowired
    private lateinit var taskTagRepository: TaskTagRepository

    @Autowired
    private lateinit var tagValueRepository: TagValueRepository

    @Autowired
    private lateinit var taskRepository: TaskRepository

    @GetMapping(path = ["/add"])
    @ResponseBody
    fun add(
        @RequestParam(value = "task_id") taskId: Int,
        @RequestParam(value = "tag_value_id") tagValueId: Int
    ): AddTaskTagResult {
        val taskOptional = taskRepository.findById(taskId)
        if (taskOptional.isEmpty) {
            return AddTaskTagResult(false)
        }
        val task = taskOptional.get()

        val tagValueOptional = tagValueRepository.findById(tagValueId)
        if (tagValueOptional.isEmpty) {
            return AddTaskTagResult(false)
        }
        val tagValue = tagValueOptional.get()

        var taskTag = TaskTag()
        taskTag.task = task
        taskTag.tagValue = tagValue

        try {
            taskTag = taskTagRepository.save(taskTag)
        } catch (ex: Exception) {
            return AddTaskTagResult(false)
        }

        return AddTaskTagResult(true, taskTag)
    }

    @GetMapping(path = ["/remove"])
    @ResponseBody
    fun remove(@RequestParam(value = "task_tag_id") taskTagId: Int): RemoveTaskTagResult {
        try {
            taskTagRepository.deleteById(taskTagId)
        } catch (ex: Exception) {
            return RemoveTaskTagResult(false)
        }
        return RemoveTaskTagResult(true)
    }
}