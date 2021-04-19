from flask import Flask
from pycoingecko import CoinGeckoAPI
import time
import _thread

cg = CoinGeckoAPI()
app = Flask(__name__)

coin_data = {}


def update_coin_data():
    prices = cg.get_price(ids=['bitcoin', 'ethereum', 'litecoin', 'monero'], vs_currencies='usd')
    coin_data['bitcoin'] = prices['bitcoin']['usd']
    coin_data['ethereum'] = prices['ethereum']['usd']
    coin_data['litecoin'] = prices['litecoin']['usd']
    coin_data['monero'] = prices['monero']['usd']


@app.route("/bitcoin")
def bitcoin():
    update_coin_data()
    return str(coin_data['bitcoin'])

@app.route("/ethereum")
def ethereum():
    update_coin_data()
    return str(coin_data['ethereum'])

@app.route("/litecoin")
def litecoin():
    update_coin_data()
    return str(coin_data['litecoin'])

@app.route("/monero")
def monero():
    update_coin_data()
    return str(coin_data['monero'])

if __name__ == "__main__":
    app.run('0.0.0.0', 5000)
