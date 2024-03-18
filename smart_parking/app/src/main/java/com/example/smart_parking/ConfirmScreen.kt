package com.example.smart_parking


import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.offset
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Favorite
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.Info
import androidx.compose.material3.Button
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import androidx.navigation.compose.rememberNavController
import com.example.smart_parking.ui.theme.Smart_parkingTheme


@Composable
fun ConfirmScreen(
    modifier: Modifier = Modifier,
    navController: NavController = rememberNavController(),
    userName: MutableState<String> = mutableStateOf(""),
    bookingLocation: MutableState<String> = mutableStateOf(""),
) {
    val location = bookingLocation.value
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(50.dp),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(text ="You have successfully booked car park slot ${location}.", style = MaterialTheme.typography.titleLarge)
        Spacer(modifier = Modifier.height(20.dp))

        Spacer(modifier = Modifier.height(30.dp))
        Button(
            onClick = {
                userName.value = ""
                bookingLocation.value = ""
                navController.navigate(Screen.StartScreen.route)
            }
        ) {
            Text(text = "Exit Application")
        }
    }
}

@Preview
@Composable
fun ConfirmScreenPreview() {
    Smart_parkingTheme {
        // A surface container using the 'background' color from the theme
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = MaterialTheme.colorScheme.background
        ) {
            ConfirmScreen()
        }
    }
}