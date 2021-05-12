package timelimit.main

import org.springframework.data.repository.CrudRepository

interface NotificationRepository : CrudRepository<Notification, Int> {}