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
    var permissions: String = "0".repeat(64)

    fun checkPermission(permission: Permission): Boolean {
        var result = true
        val bitSet = getBitsetPermissions(permissions)

        for (bitIndex in permission.bitsIndex) {
            if (!bitSet.get(bitIndex)) {
                result = false
            }
        }
        return result
    }

    fun isSubset(role: Role): Boolean {
        val currentBitSet = getBitsetPermissions(permissions)
        val otherBitSet = getBitsetPermissions(role.permissions)

        currentBitSet.or(otherBitSet)

        return currentBitSet == otherBitSet
    }

    companion object {
        val PERMISSIONS_PROJECT_CREATOR = "1".repeat(64)

        fun getBitsetPermissions(permissions: String): BitSet {
            val bitSet = BitSet(permissions.length)
            for (i in permissions.indices) {
                bitSet[i] = permissions[i] != '0'
            }
            return bitSet
        }
    }
}