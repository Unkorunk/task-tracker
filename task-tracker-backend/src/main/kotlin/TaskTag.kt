package timelimit.main

import com.fasterxml.jackson.annotation.JsonIgnore
import javax.persistence.*

@Entity
@Table(name = "task_tags")
class TaskTag {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @JsonIgnore
    @ManyToOne
    @JoinColumn(name = "task_id", nullable = false)
    var task: Task = Task()

    @ManyToOne
    @JoinColumn(name = "tag_value_id", nullable = false)
    var tagValue: TagValue = TagValue()
}