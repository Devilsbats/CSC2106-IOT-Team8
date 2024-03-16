from flask import Flask, jsonify

app = Flask(__name__)

@app.route("/data")
def get_data():
    json_data = {
      "end_device_ids": {
        "device_id": "eui-70b3d57ed0065bae",
        "application_ids": {
          "application_id": "csc2106proj"
        },
        "dev_eui": "70B3D57ED0065BAE",
        "join_eui": "0000000000000000",
        "dev_addr": "260D6FA6"
      },
      "correlation_ids": [
        "gs:uplink:01HRW4TKZBZFNM4G55670B6DXB"
      ],
      "received_at": "2024-03-13T14:52:54.844872843Z",
      "uplink_message": {
        "session_key_id": "AY44RCyuMlkjHzt7IpcsPg==",
        "f_port": 1,
        "f_cnt": 57,
        "frm_payload": "AAAAAA==",
        "decoded_payload": {
          "distance": 100.50,
          "temperature": 203.634
        },
        "rx_metadata": [
          {
            "gateway_ids": {
              "gateway_id": "eui-ac1f09fffe10a703-1",
              "eui": "AC1F09FFFE10A703"
            },
            "timestamp": 662898542,
            "rssi": -38,
            "channel_rssi": -38,
            "snr": 13,
            "frequency_offset": "9509",
            "uplink_token": "CiQKIgoWZXVpLWFjMWYwOWZmZmUxMGE3MDMtMRIIrB8J//4QpwMQ7o6MvAIaDAjG+8avBhDrju2uAiCwi9S+pRM=",
            "channel_index": 3,
            "received_at": "2024-03-13T14:52:54.555013053Z"
          }
        ],
        "settings": {
          "data_rate": {
            "lora": {
              "bandwidth": 125000,
              "spreading_factor": 7,
              "coding_rate": "4/5"
            }
          },
          "frequency": "917400000",
          "timestamp": 662898542
        },
        "received_at": "2024-03-13T14:52:54.636327807Z",
        "consumed_airtime": "0.051456s",
        "network_ids": {
          "net_id": "000013",
          "ns_id": "EC656E0000000183",
          "tenant_id": "ttn",
          "cluster_id": "au1",
          "cluster_address": "au1.cloud.thethings.network"
        }
      },
      "EventProcessedUtcTime": "2024-03-13T14:52:56.6669480Z",
      "PartitionId": 1,
      "EventEnqueuedUtcTime": "2024-03-13T14:52:56.4470000Z",
      "IoTHub": {
        "MessageId": None,
        "CorrelationId": None,
        "ConnectionDeviceId": "eui-70b3d57ed0065bae",
        "ConnectionDeviceGenerationId": "638458083687343608",
        "EnqueuedTime": "2024-03-13T14:52:56.3370000Z"
      }
    }

    extracted_data = {
        "temperature": json_data["uplink_message"]["decoded_payload"]["temperature"],
        "distance": json_data["uplink_message"]["decoded_payload"]["distance"]
    }
    
    return jsonify(extracted_data)

if __name__ == "__main__":
    app.run(debug=True)
