import paho.mqtt.client as mqtt
import json
from datetime import datetime

with open("drone_telemetry.json", "w") as f:
    f.write("TimeStamp, Temperature, Pressure, Altitude\n")

def on_message(client, userdata, msg):
    data = json.loads(msg.payload.decode())
    print(data)
    with open("drone_telemetry.json", "a") as f:
        f.write(f"{datetime.now()}, {data['temperature']}, {data['pressure']}, {data['altitude']}\n")

client = mqtt.Client()
client.connect("127.0.0.1", 1883)
client.subscribe("drone/telemetry")
client.on_message = on_message

client.loop_forever()
