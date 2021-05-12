package timelimit.main

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
    val author: User? = null

    @Temporal(TemporalType.TIMESTAMP)
    @Column(nullable = false)
    val createdAt: Date = Date()

    @Column(nullable = false)
    val text: String = ""

    @ManyToOne
    @JoinColumn(name = "task_id", nullable = false)
    val task: Task = Task()
}