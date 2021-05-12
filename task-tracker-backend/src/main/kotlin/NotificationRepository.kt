package timelimit.main

import org.springframework.data.jpa.repository.Modifying
import org.springframework.data.jpa.repository.Query
import org.springframework.data.repository.CrudRepository
import org.springframework.transaction.annotation.Transactional

interface NotificationRepository : CrudRepository<Notification, Int> {
    @Query(value = "select u from Notification u where u.user = ?1 and isRead = false")
    fun getUnread(user: User) : Set<Notification>

    @Query(value = "select u from Notification u where u.user = ?1")
    fun getAll(user: User) : Set<Notification>

    @Transactional
    @Modifying
    @Query(value = "delete from Notification u where u.user = ?1 and isRead = true")
    fun clearRead(user: User)

    @Transactional
    @Modifying
    @Query(value = "delete from Notification u where u.user = ?1")
    fun clearAll(user: User)
}