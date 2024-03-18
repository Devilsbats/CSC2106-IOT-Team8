package com.example.smart_parking

import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable

sealed class Screen(val route: String){
    object StartScreen: Screen(route = "start_screen")
    object BookingScreen: Screen(route = "booking_screen")
    object ConfirmScreen: Screen(route = "confirm_screen")
}

@Composable
fun NavGraph(
    navController: NavHostController,
    userName: MutableState<String>,
    bookingLocation: MutableState<String>
) {
    NavHost(
        navController = navController,
        startDestination = Screen.StartScreen.route //TODO: define the start screen
    ){
        //TODO: add composable(route)
        composable(Screen.StartScreen.route) {
            StartScreen(navController=navController, userName = userName)
        }
        composable(Screen.BookingScreen.route) {
            BookingScreen(navController=navController, userName = userName, bookingLocation = bookingLocation)
        }
        composable(Screen.ConfirmScreen.route) {
            ConfirmScreen(navController=navController, userName = userName, bookingLocation = bookingLocation)
        }
    }
}