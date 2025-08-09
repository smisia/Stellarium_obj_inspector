"""
You can poll Stellarium to get an objects data, to use it for further calculations
"""

import json
import threading
import time

import requests
from bs4 import BeautifulSoup

#---------- User parameters ---------------------

URL = "http://localhost:8090/api/main/status"       # The URL of the web server
SELECTED_PARAMETER = 5                              # The number of the parameters starting from the top
REFRESH_RATE = 2                                    # Define the refresh time in seconds
PRINT_WHOLE_DATASET = False                         # If False it prints only the chosen parameters if True it prints the whole dataset
USED_LOCALLY = False                                # If True you can use the script as a standalone

#---------- User parameters ---------------------

data_dict = {}

def concert_to_raw(ra_dec: str) -> list[float]:
    """ Convert extracted data to a form which can be processed by the MCU """

    hours,minutes,seconds  = 0, 0, 0
    deg, deg_min, deg_sec = 0, 0, 0
    # parsing the right angle
    for parser in range(0, len(ra_dec)):
        if ra_dec[parser] == "h":
            hour_cursor = parser + 1
            hours = float(ra_dec[:parser])
        if ra_dec[parser] == "m":
            minutes_cursor = parser + 1
            minutes = float(ra_dec[hour_cursor:parser])
        if ra_dec[parser] == "s":
            first_endpoint = parser +1
            seconds = float(ra_dec[minutes_cursor:parser])

    ra = hours + (minutes/60.0) + (seconds/3600.0)

    ra_dec = ra_dec[first_endpoint + 1 :]
    # parsing the dec
    for parser in range(0, len(ra_dec)):
        if ra_dec[parser] == "°":
            degree_cursor = parser + 1
            deg = float(ra_dec[:parser])
        if ra_dec[parser] == "'":
            deg_min_cursor = parser + 1
            deg_min = float(ra_dec[degree_cursor:parser])
        if ra_dec[parser] == "\"":
            deg_sec = float(ra_dec[deg_min_cursor:parser])

    dec = deg + (deg_min/60.0) + (deg_sec/3600.0)

    return [ra, dec]

def get_data_from_source() -> list[float] | int:
    """ Extract the data from the selected object """

    try:
        response = requests.get(URL)
        data = response.json()
        read_data = data.get("selectioninfo", None)

        soup = BeautifulSoup(read_data, "html.parser")

        text = soup.get_text(separator="\n")

        for line in text.splitlines():
            line = line.strip()
            if ":" in line:
                key, value = line.split(":", 1)
                data_dict[key.strip()] = value.strip()

        keys = list(data_dict.keys())
        values = list(data_dict.values())

        if PRINT_WHOLE_DATASET:
            print(json.dumps(data_dict, indent=2, ensure_ascii=False))
        else:
            if data_dict != {}:
                print(f"{keys[SELECTED_PARAMETER]}: {values[SELECTED_PARAMETER]}")
            else:
                print("No object selected!")

        time.sleep(2)

        pos_num_data = concert_to_raw(values[SELECTED_PARAMETER])

        return pos_num_data

    except KeyboardInterrupt:
        print("Exiting....")

    return 0


if USED_LOCALLY:
    while True:
        get_data_from_source()
else:
    threading.Thread(target=get_data_from_source, daemon=True).start()
