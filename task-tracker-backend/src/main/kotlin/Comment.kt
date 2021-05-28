package timelimit.main

import com.fasterxml.jackson.annotation.JsonIgnore
import java.util.*
import javax.persistence.*

@Entity
@Table(name = "comments")
class Comment {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @ManyToOne
    @JoinColumn(name = "author_id", nullable = true)
    var author: User? = null

    @Temporal(TemporalType.TIMESTAMP)
    @Column(nullable = false)
    var createdAt: Date = Date()

    @Column(nullable = false)
    var text: String = ""

    @JsonIgnore
    @ManyToOne
    @JoinColumn(name = "task_id", nullable = false)
    var task: Task = Task()
}