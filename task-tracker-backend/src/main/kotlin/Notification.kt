package timelimit.main

import javax.persistence.*

@Entity
@Table(name = "notifications")
class Notification {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @ManyToOne
    @JoinColumn(name = "user_id", nullable = false)
    val user: User = User()

    @Column(nullable = false)
    val text: String = ""

    @Column(nullable = false)
    val isRead: Boolean = false
}