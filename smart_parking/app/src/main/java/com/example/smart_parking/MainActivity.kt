package com.example.smart_parking

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.navigation.compose.rememberNavController
import com.example.smart_parking.ui.theme.Smart_parkingTheme
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
            //MainApp()
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
fun HomeScreen(modifier: Modifier = Modifier) {
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

@Preview(showBackground = true)
@Composable
fun MainApp() {
    Smart_parkingTheme {
        // A surface container using the 'background' color from the theme
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = MaterialTheme.colorScheme.background
        ) {
            //TODO: state hoisting: define userInput and score state here
            var userName = rememberSaveable { mutableStateOf("") }
            var bookingLocation = rememberSaveable { mutableStateOf("") }
            var navController = rememberNavController() //TODO: state hoisting: define userInput and score state here
            NavGraph(navController, userName, bookingLocation) //TODO: pass the navController, userInput, score to NavGraph

        }
    }
}