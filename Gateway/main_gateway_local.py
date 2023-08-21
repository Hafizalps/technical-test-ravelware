import paho.mqtt.client as mqtt
import pymysql
import json

# Konfigurasi MQTT
mqtt_broker = "192.168.1.7"
mqtt_port = 1883
mqtt_username = ""
mqtt_password = ""
mqtt_topic = "DATA/LOCAL/SENSOR/PANEL_1"

# Konfigurasi MySQL
mysql_host = "localhost"
mysql_user = "root"
mysql_password = ""
mysql_db = "mosquitto_db"

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT Broker with result code " + str(rc))
    client.subscribe(mqtt_topic)
    print("Subscribed to topic: " + mqtt_topic)

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode("utf-8")
        save_to_mysql(payload)
    except Exception as e:
        print("Error:", str(e))

def save_to_mysql(payload):
    try:
        db = pymysql.connect(host=mysql_host, user=mysql_user, password=mysql_password, db=mysql_db)
        cursor = db.cursor()
        
        data = json.loads(payload)  # Parsing JSON
        status = data.get("status")
        device_id = data.get("deviceID")
        volt = data.get("data", {}).get("v")
        current = data.get("data", {}).get("i")
        power = data.get("data", {}).get("pa")
        temperature = data.get("data", {}).get("temp")
        fan_status = data.get("data", {}).get("fan")
        
        query = "INSERT INTO mqtt_data (status, device_id, volt, current, power, temperature, fan_status) VALUES (%s, %s, %s, %s, %s, %s, %s)"
        cursor.execute(query, (status, device_id, volt, current, power, temperature, fan_status))
        
        db.commit()
        db.close()
        print("Data saved to MySQL: Status =", status)
    except Exception as e:
        print("Error saving to MySQL:", str(e))

def main():
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(username=mqtt_username, password=mqtt_password)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    try:
        mqtt_client.connect(mqtt_broker, mqtt_port, 60)
        mqtt_client.loop_forever()
    except KeyboardInterrupt:
        print("KeyboardInterrupt detected. Exiting...")
        mqtt_client.disconnect()

if __name__ == "__main__":
    main()
