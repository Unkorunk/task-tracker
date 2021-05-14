package timelimit.main

import javax.persistence.*

@Entity
@Table(name = "roles")
class Role {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @ManyToOne
    @JoinColumn(name = "project_id", nullable = false)
    var project: Project = Project()

    @Column(nullable = false)
    var value: String = ""

    @Column(nullable = false)
    val permissions: Int = 0
}