package com.example.smart_parking

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
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

    if (dataList != null && dataList!!.isNotEmpty()) {
        Table(dataList!!)
    } else {
        Text("No data available", modifier = Modifier.padding(8.dp))
    }
}

@Composable
fun Table(dataList: List<Data>) {
    val backgroundColor = Color(android.graphics.Color.parseColor("#73e6ff"))
    Column(
        modifier = Modifier.padding(8.dp)
    ) {
        // Table header
        Row(
            modifier = Modifier.fillMaxWidth()
        ) {
            TableCell(text = "Location", modifier = Modifier.weight(1f).background(backgroundColor))
            TableCell(text = "Temperature (°C)", modifier = Modifier.weight(1f).background(backgroundColor))
            TableCell(text = "Available Slots", modifier = Modifier.weight(1f).background(backgroundColor))
        }
        // Table rows
        dataList.forEach { dataItem ->
            Row(
                modifier = Modifier.fillMaxWidth()
            ) {
                TableCell(text = dataItem.location, modifier = Modifier.weight(1f))
                TableCell(text = "${dataItem.avg_temperature}°C", modifier = Modifier.weight(1f))
                TableCell(text = "${dataItem.available_slots}", modifier = Modifier.weight(1f))
            }
        }
    }
}

@Composable
fun TableCell(text: String, modifier: Modifier = Modifier) {
    Box(
        modifier = modifier
            .border(BorderStroke(1.dp, Color.Black))
            .padding(8.dp)
    ) {
        Text(text = text)
    }
}
