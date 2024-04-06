package com.example.smart_parking

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp

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
@Composable
fun TableHeader() {
    val backgroundColor = Color(android.graphics.Color.parseColor("#73e6ff"))
    Row(
        modifier = Modifier.fillMaxWidth()
    ) {
        TableCell(text = "Location", modifier = Modifier.weight(1f).background(backgroundColor))
        TableCell(text = "Temperature (°C)", modifier = Modifier.weight(1f).background(backgroundColor))
        TableCell(text = "Available Slots", modifier = Modifier.weight(1f).background(backgroundColor))
    }
}

@Composable
fun TableRow(dataItem: Data?) {
    Row(
        modifier = Modifier.fillMaxWidth()
    ) {
        if (dataItem != null) {
            TableCell(text = dataItem.location, modifier = Modifier.weight(1f))
            TableCell(text = String.format("%.1f", dataItem.temperature), modifier = Modifier.weight(1f))
            TableCell(text = "${dataItem.available_slots}", modifier = Modifier.weight(1f))
        } else {
            TableCell(text = "Loading...", modifier = Modifier.weight(1f))
            TableCell(text = "Loading...", modifier = Modifier.weight(1f))
            TableCell(text = "Loading...", modifier = Modifier.weight(1f))
        }
    }
}