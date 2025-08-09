""" Small web server configuration to send the foramtted data to the MCU trough HTTP"""

from flask import Flask, Response

from Stellarium_planet_data_fetcher import get_data_from_source

app = Flask(__name__)

@app.route('/send', methods=['GET'])
def get_data():
    pos = get_data_from_source()
    ra = pos[0]
    dec = pos[1]
    data = f"{ra},{dec}"
    return Response(data, mimetype='text/plain')

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, threaded=True)