package timelimit.main

import com.fasterxml.jackson.annotation.JsonIgnore
import java.util.*
import javax.persistence.*

@Entity
@Table(name = "tasks")
class Task {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @ManyToOne
    @JoinColumn(name = "created_by_id", nullable = true)
    var createdBy: User? = null

    @Temporal(TemporalType.TIMESTAMP)
    @Column(nullable = false)
    var createdAt: Date = Date()

    @ManyToOne
    @JoinColumn(name = "updated_by_id", nullable = true)
    var updatedBy: User? = null

    @Temporal(TemporalType.TIMESTAMP)
    @Column(nullable = false)
    var updatedAt: Date = Date()

    @ManyToOne
    @JoinColumn(name = "project_id", nullable = false)
    var project: Project = Project()

    @Column(nullable = false)
    var storyPoints: Int = 0

    @Column(nullable = false)
    var title: String = ""

    @Column(nullable = false)
    var description: String = ""

    @ManyToOne
    @JoinColumn(name = "assigned_to_id", nullable = true)
    var assignedTo: User? = null

    @Temporal(TemporalType.TIMESTAMP)
    @Column(nullable = true)
    var deadline: Date? = null

    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.REMOVE], mappedBy = "task")
    val tags: Set<TaskTag> = emptySet()

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.REMOVE], mappedBy = "task")
    val comments: Set<Comment> = emptySet()
}