package timelimit.main

import com.fasterxml.jackson.annotation.JsonIgnore
import java.math.BigInteger
import java.security.MessageDigest
import java.util.*
import javax.persistence.*

@Entity
@Table(name = "users")
class User {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @Column(nullable = false, unique = false)
    var fullName: String = ""

    @Column(nullable = false, unique = true)
    var screenName: String = ""

    @Column(nullable = false, unique = true)
    var email: String = ""

    @Column(nullable = false)
    @JsonIgnore
    var hashedPassword: String = ""

    @Column(nullable = true)
    var photo: String? = null

    @Column(unique = true, nullable = false)
    @JsonIgnore
    var accessToken: String = ""

    @Temporal(TemporalType.TIMESTAMP)
    @Column(nullable = false)
    @JsonIgnore
    var validUntil: Date = Date()

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.REMOVE], mappedBy = "user")
    val projects: Set<ProjectUserRole> = emptySet()

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.REMOVE], mappedBy = "user")
    val notifications: Set<Notification> = emptySet()

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.DETACH], mappedBy = "author")
    val comments: Set<Comment> = emptySet()

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.DETACH], mappedBy = "assignedTo")
    val assignedTasks: Set<Task> = emptySet()

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.DETACH], mappedBy = "updatedBy")
    val updatedTasks: Set<Task> = emptySet()

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.DETACH], mappedBy = "createdBy")
    val createdTasks: Set<Task> = emptySet()

    private fun getRandomString(length: Int): String {
        val allowedChars = ('A'..'Z') + ('a'..'z') + ('0'..'9')
        return (1..length)
            .map { allowedChars.random() }
            .joinToString("")
    }

    fun updateToken() {
        accessToken = getRandomString(64)
        val calendar = Calendar.getInstance()
        calendar.time = Date()
        calendar.add(Calendar.DATE, 1)
        validUntil = calendar.time
    }

    fun updatePassword(password: String) {
        this.hashedPassword = md5(password)
    }

    fun comparePassword(password: String): Boolean {
        return this.hashedPassword == md5(password)
    }

    private fun md5(input: String): String {
        val md = MessageDigest.getInstance("MD5")
        return BigInteger(1, md.digest(input.toByteArray())).toString(16).padStart(32, '0')
    }
}