import serial
from datetime import datetime
import schedule
import time
import requests

arduino = serial.Serial(port='COM4 ', baudrate=9600, timeout=.1)
base_url = 'https://api.open-meteo.com/v1/forecast?'

params = {
    "latitude": 56.173645477920324,
    "longitude": 10.184723138809206,
    "current": ["temperature_2m", "apparent_temperature", "precipitation",
                "weather_code", "cloud_cover", "wind_speed_10m", "wind_direction_10m", "is_day"],
    "wind_speed_unit": "ms"
}

weather_code_transformer = (
    {k: 0 for k in [0]} |  # "Clear sky"
    {k: 1 for k in [1, 2]} |  # "Mainly clear"
    {k: 2 for k in [3]} |  # "Overcast"
    {k: 3 for k in [45, 48]} |  # "Fog"
    {k: 4 for k in [51, 53, 55, 56, 57]} |  # "Drizzle"
    {k: 5 for k in [61, 63, 65, 66, 67, 80, 81, 8]} |  # "Rain showers"
    {k: 6 for k in [71, 73, 75, 77, 85, 86]} |  # "Snow showers"
    {k: 7 for k in [95, 96, 99]}  # "Thunderstorm"
)


def send_weather_to_arduino():
    response = requests.get(
        url=base_url,
        params=params
    ).json()["current"]
    now = datetime.now()
    seconds_today = now.hour * 60 * 60 + now.minute * 60 + now.second
    out = f'{seconds_today},{response["temperature_2m"]},{weather_code_transformer[response["weather_code"]]},'
    arduino.write(bytes(out, 'utf-8'))
    arduino.flush()


schedule.every().hour.at(":00").do(send_weather_to_arduino)
while True:
    if arduino.readline().strip() == b"ready":
        send_weather_to_arduino()
        while True:
            schedule.run_pending()
            time.sleep(1)


# a = {
#     'time': '2025-04-10T20:00',
#     'interval': 900,                # s
#     'temperature_2m': 7.6,          # °C
#     'apparent_temperature': 0.8,    # °C
#     'precipitation': 0.0,           # mm
#     'weather_code': 3,              # See weather_code_transformer
#     'cloud_cover': 97,              # %
#     'wind_speed_10m': 10.4,         # m/s
#     'wind_direction_10m': 289,      # Degrees
#     'is_day': 0                     # Boolean
# }

