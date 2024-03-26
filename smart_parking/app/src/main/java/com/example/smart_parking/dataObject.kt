package com.example.smart_parking

data class Data(
    val location: String,
    val available_slots: Int,
    val avg_temperature: Double
)

object locationLink {
    val bedok = "https://fnstora5jkqhp2eayio.blob.core.windows.net/azure-webjobs-hosts/location/Bedok/0_23a8a13336514a70b54215f63992abd9_1.json?sp=r&st=2024-03-26T04:04:19Z&se=2024-03-26T12:04:19Z&spr=https&sv=2022-11-02&sr=b&sig=6Y6rdP4z8wnvnk2ENQCA7p61R%2B%2B%2FegQxUls3Xj7S5Rw%3D"
    val other = "https://fnstora5jkqhp2eayio.blob.core.windows.net/azure-webjobs-hosts/location/Other/0_e767acfd71974996b722df9a3ae5a280_1.json?sp=r&st=2024-03-26T04:06:40Z&se=2024-03-26T12:06:40Z&spr=https&sv=2022-11-02&sr=b&sig=HfyFG4VwenrGKgcpb7qgdME4eJbYQXUrx8JKhpgD2Ak%3D"
}