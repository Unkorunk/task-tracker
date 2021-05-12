package timelimit.main

import org.springframework.data.jpa.repository.Query
import org.springframework.data.repository.CrudRepository

interface UserRepository : CrudRepository<User, Int> {
    @Query(value = "select u from User u where u.screenName = ?1")
    fun findByScreenName(screenName: String): User?

    @Query(value = "select u from User u where u.accessToken = ?1")
    fun findByAccessToken(accessToken: String): User?
}