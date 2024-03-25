from flask import Flask, jsonify

app = Flask(__name__)

@app.route("/data")
def get_data():
  json_data = {
    "locations": [
      {
        "location": "Singapore",
        "available_slots": 1,
        "avg_temperature": 90.0
      },
      {
        "location": "New York",
        "available_slots": 3,
        "avg_temperature": 60.5
      },
      {
        "location": "Tokyo",
        "available_slots": 2,
        "avg_temperature": 20.0
      }
    ]

      # "location": "Singapore",
      # "available_slots": 1,
      # "avg_temperature": 85.0
  }
    
  return jsonify(json_data)

if __name__ == "__main__":
    app.run(debug=True)