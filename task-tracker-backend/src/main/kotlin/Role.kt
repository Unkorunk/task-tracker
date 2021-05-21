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

    fun checkPermission(permission: Permission): Boolean {
        var result = true
        val bitSet = BitSet.valueOf(permissions)
        for (bitIndex in permission.bitsIndex) {
            if (!bitSet.get(bitIndex)) {
                result = false
            }
        }
        return result
    }

    fun isSubset(role: Role): Boolean {
        val currentBitSet = BitSet.valueOf(permissions)
        val otherBitSet = BitSet.valueOf(role.permissions)

        val bitSet = currentBitSet.clone()

        currentBitSet.or(otherBitSet)

        return currentBitSet == bitSet
    }

    companion object {
        val PERMISSIONS_PROJECT_CREATOR: ByteArray = BitSet().let {
            it.set(0, 64, true)
            it
        }.toByteArray()
    }
}