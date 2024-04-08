# CSC2106-IOT-Team8-ParkEase

## Introduction

### Project Summary
A Smart Car Park Network which allows users to find available parking lots while gaining other information such as temperature of the parking lots, allowing drivers to ensure the least deterioration towards their vehicles when parking them.
### Research 
we were recommended to implement 2 LoRa Networks, LoRaWAN and LoRa Mesh to determine which of the 2 is better for the communication between Car Parks.

## LoRa Mesh
The LoRaMesh is implemented using Flooding of the packets in the network. To tackle the problem of redundant messages from the network, a TTL variable is declared in the data. Specifying the TTL value depends the distance range of the current connection to the LoRa Node which forwards the data to the cloud. For example, TTL = 3 allows the data packet to travel up to 9km from the node that sends the data as it allows 3 hops. This situation works of there are intermidiary nodes to forward the packet to the node.<br>

In the LoRaWAN file, it contains the LoRaWAN Folder <br>
&emsp;&emsp; |__ Central LoRa Mesh Code for Maker UNO which M5StickCPlus has internet connection to cloud <br>

&emsp;&emsp; |__ LoRa Mesh Node Code for Maker UNO <br>

&emsp;&emsp; |__Painless Mesh Folder configured for LoRa Mesh <br>

&emsp;&emsp;&emsp;&emsp; |__"Central" Node code for M5StickCPlus connecting to LoRA Module <br>

&emsp;&emsp;&emsp;&emsp; |__Node

## LoRaWAN
LoRAWAN is implemented using the LMIC Library.<br>

In the LoRaWAN file, it contains the LoRaWAN Folder <br>
&emsp;&emsp; |__LoRaWAN Code for Maker UNO <br>

&emsp;&emsp; |__Painless Mesh Folder configured for LoRaWAN <br>

&emsp;&emsp;&emsp;&emsp; |__"Central" Node code for M5StickCPlus connecting to LoRA Module <br>

&emsp;&emsp;&emsp;&emsp; |__Node


          
      
