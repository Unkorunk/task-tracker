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

    @ManyToOne
    @JoinColumn(name = "project_id", nullable = false)
    var project: Project = Project()

    @Column(nullable = false)
    var caption: String = ""

    @OneToMany(fetch = FetchType.LAZY, cascade = [CascadeType.REMOVE], mappedBy = "tagCaption")
    val values: Set<TagValue> = emptySet()
}