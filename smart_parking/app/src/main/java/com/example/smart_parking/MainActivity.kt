package com.example.smart_parking

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import com.google.gson.Gson
import io.ktor.client.HttpClient
import io.ktor.client.engine.cio.CIO
import io.ktor.client.request.get
import io.ktor.client.statement.HttpResponse
import io.ktor.client.statement.bodyAsText
import io.ktor.http.isSuccess
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.flow
import org.json.JSONObject

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            HomeScreen()
        }
    }
}

suspend fun getData(): List<Data>? {
    val client = HttpClient(CIO)
    var responseData: List<Data>? = null
    try {
        val response: HttpResponse = client.get("http://10.0.2.2:5000/data")
        if (response.status.isSuccess()) {
            val jsonString = response.bodyAsText()
            val locationsJson = JSONObject(jsonString).getJSONArray("locations")
            val gson = Gson()
            responseData = List(locationsJson.length()) { index ->
                gson.fromJson(locationsJson.getJSONObject(index).toString(), Data::class.java)
            }
        }
    } catch (e: Exception) {
        println("Client Error: ${e.message}")
    } finally {
        client.close()
    }
    return responseData
}

data class Data(
    val location: String,
    val available_slots: Int,
    val avg_temperature: Double
)

@Composable
fun HomeScreen() {
    var dataList by rememberSaveable { mutableStateOf<List<Data>?>(null) }

    LaunchedEffect(Unit) {
        val dataFlow = flow {
            while (true) {
                emit(getData())
                delay(5000)
            }
        }

        dataFlow.collect { newData ->
            dataList = newData
        }
    }

    Column {
        dataList?.let { data ->
            data.forEach { dataItem ->
                Text(
                    text = "Location: ${dataItem.location}, Temperature: ${dataItem.avg_temperature}°C, Available Slots: ${dataItem.available_slots}"
                )
            }
        } ?: Text("No data available")
    }
}