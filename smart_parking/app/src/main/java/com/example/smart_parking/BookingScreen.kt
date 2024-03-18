package com.example.smart_parking

import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Home
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import androidx.navigation.compose.rememberNavController
import com.example.smart_parking.ui.theme.Smart_parkingTheme

//Joo Wee
fun carParkList(): List<String> {
    return (1..6).map { "a$it" }
}
@Composable
fun BookingScreen(
    modifier: Modifier = Modifier,
    navController: NavController = rememberNavController(),
    userName: MutableState<String> = mutableStateOf(""),
    bookingLocation: MutableState<String> = mutableStateOf(""),
) {
    // Get from list
    val carParkList = carParkList()
    var curSelectedIndex by rememberSaveable { mutableStateOf(0) }
    var curProb by rememberSaveable { mutableFloatStateOf(0.0F) }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Column(
            verticalArrangement = Arrangement.spacedBy(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            LazyColumn {
                items(carParkList.size) { index ->
                    var isTaken by remember { mutableStateOf(true) }
                    Button(
                        onClick = {
                            navController.navigate(Screen.ConfirmScreen.route)
                            bookingLocation.value = carParkList[index]
                            isTaken = false

                        },
                        enabled = isTaken,
                        colors = ButtonDefaults.buttonColors(
                            contentColor = Color.White,
                            containerColor = Color.Blue
                        ),
                        modifier = Modifier
                            .then(if (isTaken) Modifier.fillMaxWidth() else Modifier.fillMaxWidth(0.5f))

                    ) {
                        Text(text = "Book car park slot ${carParkList[index]}")
                    }
                }
            }
        }
    }
}

@Preview
@Composable
fun BookingScreenPreview() {
    Smart_parkingTheme {
        // A surface container using the 'background' color from the theme
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = MaterialTheme.colorScheme.background
        ) {
            BookingScreen()
        }
    }
}