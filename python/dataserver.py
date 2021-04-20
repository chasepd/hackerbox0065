from flask import Flask
from pycoingecko import CoinGeckoAPI
from time import sleep
from threading import Timer

cg = CoinGeckoAPI()
app = Flask(__name__)

coin_data = {}
coins_to_fetch = ["bitcoin", "ethereum", "litecoin", "monero"]

#Credit for RepeatedTimer class goes to MestreLion from https://stackoverflow.com/questions/474528/what-is-the-best-way-to-repeatedly-execute-a-function-every-x-seconds
class RepeatedTimer(object):
    def __init__(self, interval, function, *args, **kwargs):
        self._timer     = None
        self.interval   = interval
        self.function   = function
        self.args       = args
        self.kwargs     = kwargs
        self.is_running = False
        self.start()

    def _run(self):
        self.is_running = False
        self.start()
        self.function(*self.args, **self.kwargs)

    def start(self):
        if not self.is_running:
            self._timer = Timer(self.interval, self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        self._timer.cancel()
        self.is_running = False

def update_coin_data():
    prices = cg.get_price(ids=coins_to_fetch, vs_currencies='usd')
    for coin in coins_to_fetch:
        coin_data[coin] = prices[coin]['usd']

@app.route("/<coin>")
def getPrice(coin):
    try:
        return str(coin_data[coin])
    except KeyError:
        return "NULL"


if __name__ == "__main__":
    update_coin_data() #get initial values
    rt = RepeatedTimer(10, update_coin_data) 
    app.run('0.0.0.0', 5000)
