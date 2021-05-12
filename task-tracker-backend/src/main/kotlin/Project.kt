package timelimit.main

import com.fasterxml.jackson.annotation.JsonIgnore
import javax.persistence.*

@Entity
@Table(name = "projects")
class Project {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @Column(nullable = false)
    var fullName: String = ""

    @Column(nullable = true)
    var photo: String? = null

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.REMOVE], mappedBy = "project")
    val tasks: Set<Task> = emptySet()

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.REMOVE], mappedBy = "project")
    val users: Set<ProjectUserRole> = emptySet()
}