package com.example.smart_parking

data class Data(
    var location: String,
    var available_slots: Int,
    var avg_temperature: Double
)

object locationLink {
    val bedok = "\n" +
            "https://fnstora5jkqhp2eayio.blob.core.windows.net/azure-webjobs-hosts/location/Bedok/0_589aa38ea50b4731bf2b3c0e866076bd_1.json?sp=r&st=2024-03-28T13:51:11Z&se=2024-03-28T21:51:11Z&spr=https&sv=2022-11-02&sr=b&sig=0G6ZoHtZLUIz3%2F2iLOBUbL32%2BneSi4kdzckEs8%2FF2fU%3D"
    val other = ""
    val pasirRis = "https://fnstora5jkqhp2eayio.blob.core.windows.net/azure-webjobs-hosts/location/Pasir%2520Ris/0_fc4998ca9f26412d8b209dcb21ee629d_1.json?sp=r&st=2024-03-28T17:30:15Z&se=2024-03-29T01:30:15Z&spr=https&sv=2022-11-02&sr=b&sig=9eX2WTGwLLx3%2F2UrDMnvkvalfJTcHbHlW2JjmorWf4s%3D"
}