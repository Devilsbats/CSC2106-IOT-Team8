package com.example.smart_parking

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import androidx.navigation.compose.rememberNavController
import com.example.smart_parking.ui.theme.Smart_parkingTheme


@Composable
fun StartScreen(
    modifier: Modifier = Modifier,
    navController: NavController = rememberNavController(),
    userName: MutableState<String> = mutableStateOf("")
) {
    var buttonEnabled by remember { mutableStateOf(false) }
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(text = "Smart Parking Application", style = MaterialTheme.typography.titleLarge)
        Spacer(modifier = Modifier.height(20.dp))
        Text(text = "Username: ")
        TextField(
            value = userName.value,
            onValueChange = {
                userName.value = it
                buttonEnabled = it.isNotBlank()
            },
            modifier = Modifier,
            label = { Text(text = "name", style = MaterialTheme.typography.labelSmall) },
        )
        Button(
            onClick = {
                navController.navigate(Screen.BookingScreen.route)
            },
            enabled = buttonEnabled,
            colors = ButtonDefaults.buttonColors(
                contentColor = Color.White,
                containerColor = Color.Blue
            ),
            modifier = Modifier
                .align(Alignment.End)
                .fillMaxWidth()

        ) {
            Text(text = "Enter")
        }
    }
}


@Preview
@Composable
fun StartScreenPreview() {
    Smart_parkingTheme {
        // A surface container using the 'background' color from the theme
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = MaterialTheme.colorScheme.background
        ) {
            StartScreen()
        }
    }
}