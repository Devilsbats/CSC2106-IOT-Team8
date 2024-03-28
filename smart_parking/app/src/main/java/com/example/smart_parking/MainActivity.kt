package com.example.smart_parking

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.padding
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import kotlinx.coroutines.delay

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            HomeScreen()
        }
    }
}

@Composable
fun HomeScreen(modifier: Modifier = Modifier) {
    var bedokData by rememberSaveable { mutableStateOf<Data?>(null) }
    var otherData by rememberSaveable { mutableStateOf<Data?>(null) }
    val hardCode = Data("Pasir Ris", 1, 30.0)


    LaunchedEffect(Unit) {
        while (true) {
            val newBedokData = getBedokData()
            if (newBedokData != null) {
                bedokData = newBedokData
            }
            delay(5000)
        }
    }

    LaunchedEffect(Unit) {
//        while (true) {
//            val newOtherData = getPasirRisData()
//            if (newOtherData != null) {
//                otherData = newOtherData
//            }
            otherData = hardCode
//        }
    }
    Column(
        modifier = modifier.padding(8.dp)
    ) {
        TableHeader()
        TableRow(bedokData)
        TableRow(otherData)
    }
}
