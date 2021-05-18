package timelimit.main

import javax.persistence.*

@Entity
@Table(name = "project_user_role")
class ProjectUserRole {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @ManyToOne
    @JoinColumn(name = "project_id", nullable = false)
    var project: Project = Project()

    @ManyToOne
    @JoinColumn(name = "user_id", nullable = false)
    var user: User = User()

    @ManyToOne
    @JoinColumn(name = "role_id", nullable = false)
    var role: Role = Role()
}