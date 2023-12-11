from paho.mqtt.client import Client

def mqtt_client(message):
    client = Client()
    client.connect(host="test.mosquitto.org", port=1883, keepalive=60)
    client.subscribe(topic="measurements", qos=0)
    client.publish(topic="measurements", payload=message, qos=0, retain=False)
    client.disconnect()

