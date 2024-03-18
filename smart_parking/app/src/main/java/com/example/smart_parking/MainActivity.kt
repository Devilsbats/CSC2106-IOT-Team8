package com.example.smart_parking

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.rememberCoroutineScope
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
import kotlinx.coroutines.launch

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            MainApp()
        }
    }
}

suspend fun getData() : Data? {
    val client = HttpClient(CIO)
    var responseData: Data? = null
    try {
        val response: HttpResponse = client.get("http://10.0.2.2:5000/data")
        if (response.status.isSuccess()) {
            val jsonString = response.bodyAsText()
            responseData = Gson().fromJson(jsonString, Data::class.java)
        }
    } catch (e: Exception) {
        println("Client Error: ${e.message}")
    } finally {
        client.close()
    }
    return responseData
}

data class Data(
    val temperature: Double,
    val distance: Double,
)

@Composable
fun HomeScreen(modifier: Modifier = Modifier) {
    var data by rememberSaveable { mutableStateOf<Data?>(null) }
    val scope = rememberCoroutineScope()

    Column {
        data?.let { dataItem ->
            Text(text = "Temperature: ${dataItem.temperature}, Distance: ${dataItem.distance}")
        }
        Button(
            onClick = {
                scope.launch {
                    data = getData()
                }
            }
        ) {
            Text(text = "Fetch Data")
        }
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