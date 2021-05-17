package timelimit.main

import java.util.*
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
    var permissions: ByteArray = ByteArray(4)

    companion object {
        val PERMISSIONS_PROJECT_CREATOR: ByteArray = BitSet().let {
            it.set(0, 64, true)
            it
        }.toByteArray()
    }
}