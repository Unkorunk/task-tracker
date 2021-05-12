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
    var user: User = User()

    @Column(nullable = false)
    var text: String = ""

    @Column(nullable = false)
    var isRead: Boolean = false
}