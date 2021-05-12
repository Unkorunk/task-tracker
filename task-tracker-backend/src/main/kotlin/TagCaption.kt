package timelimit.main

import com.fasterxml.jackson.annotation.JsonIgnore
import javax.persistence.*

@Entity
@Table(name = "tag_captions")
class TagCaption {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(nullable = false, unique = true)
    val id: Int = 0

    @Column(nullable = false)
    var caption: String = ""

    @JsonIgnore
    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.REMOVE], mappedBy = "tagCaption")
    val values: Set<TagValue> = emptySet()
}