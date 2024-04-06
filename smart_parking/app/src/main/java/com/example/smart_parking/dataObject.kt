package com.example.smart_parking

data class Data(
    var location: String,
    var available_slots: Int,
    var temperature: Float
)

object locationLink {
    val bedok = "\n" +
            "https://fnstora5jkqhp2eayio.blob.core.windows.net/azure-webjobs-hosts/location/Bedok/0_c94b83c2166045c088e2c18b10aebbe0_1.json?sp=r&st=2024-04-02T02:03:52Z&se=2024-04-02T10:03:52Z&spr=https&sv=2022-11-02&sr=b&sig=cmxUMhQWIo6nbiFYdNB2HYnD4%2B7HFyPuAm42h1yHfj8%3D"
    val other = ""
    val pasirRis = "https://fnstora5jkqhp2eayio.blob.core.windows.net/azure-webjobs-hosts/location/PasirRis/0_7bcfe0e95e2e450f816106e5553019e6_1.json?sp=r&st=2024-04-02T02:02:51Z&se=2024-04-02T10:02:51Z&spr=https&sv=2022-11-02&sr=b&sig=HBfbr3%2Bv4dP32nJkV411oB00oGpviSdixIg5TuGgczY%3D"
}