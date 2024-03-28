package com.example.smart_parking

import com.google.gson.Gson
import io.ktor.client.HttpClient
import io.ktor.client.engine.cio.CIO
import io.ktor.client.request.get
import io.ktor.client.statement.HttpResponse
import io.ktor.client.statement.bodyAsText
import io.ktor.http.isSuccess

private suspend fun getLocationData(locationUrl: String): Data? {
    val client = HttpClient(CIO)
    var responseData: Data? = null
    try {
        val response: HttpResponse = client.get(locationUrl)
        if (response.status.isSuccess()) {
            val jsonStringList = response.bodyAsText().lines()
            if (jsonStringList.isNotEmpty()) {
                val lastJsonString = jsonStringList.last()
                responseData = Gson().fromJson(lastJsonString, Data::class.java)
            }
        }
    } catch (e: Exception) {
        println("Client Error: ${e.message}")
    } finally {
        client.close()
    }
    return responseData
}

suspend fun getBedokData(): Data? {
    return getLocationData(locationLink.bedok)
}

suspend fun getOtherData(): Data? {
    return getLocationData(locationLink.other)
}

suspend fun getPasirRisData(): Data? {
    return getLocationData(locationLink.pasirRis)
}
