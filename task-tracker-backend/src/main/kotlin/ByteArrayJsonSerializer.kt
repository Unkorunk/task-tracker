package timelimit.main

import com.fasterxml.jackson.core.JsonGenerator
import com.fasterxml.jackson.databind.JsonSerializer
import com.fasterxml.jackson.databind.SerializerProvider
import org.springframework.boot.jackson.JsonComponent
import java.util.*

@JsonComponent
class ByteArrayJsonSerializer : JsonSerializer<ByteArray>() {
    override fun serialize(value: ByteArray, gen: JsonGenerator, serializers: SerializerProvider) {
        val bitSet = BitSet.valueOf(value)
        var result = ""
        for (i in 0 until bitSet.length()) {
            if (bitSet[i]) {
                result += "1"
            } else {
                result += "0"
            }
        }
        gen.writeString(result)
    }
}