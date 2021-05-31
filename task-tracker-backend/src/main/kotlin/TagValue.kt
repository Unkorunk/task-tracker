package timelimit.main

import com.fasterxml.jackson.annotation.JsonIgnore
import javax.persistence.*

@Entity
@Table(name = "tag_values")
class TagValue {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @JsonIgnore
    @ManyToOne
    @JoinColumn(name = "tag_caption_id", nullable = false)
    var tagCaption: TagCaption = TagCaption()

    @Column(nullable = false)
    var value: String = ""
}