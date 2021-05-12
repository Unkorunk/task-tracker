package timelimit.main

import javax.persistence.*

@Entity
@Table(name = "roles")
class Role {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @Column(nullable = false)
    var value: String = ""

    @Column(nullable = false)
    val permissions: Int = 0
}