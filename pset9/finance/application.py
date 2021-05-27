import os

from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # SELECT the data from wallet
    stocks = db.execute("SELECT * FROM wallet WHERE user_id = ?", session["user_id"])

    l = list()
    data = list()
    for row in stocks:
        l.append(row["symbol"])
    for symbol in l:
        data.append(lookup(symbol))

    total = 0
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    for row in stocks:
        total += row["value"]
    total = total + cash

    return render_template("/index.html", stocks=stocks, data=data, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # validation of purchase

        if not request.form.get("shares") or not request.form.get("shares").isdigit():
            return apology("must provide amount of stocks to purchase", 400)

        if not lookup(request.form.get("symbol")) or not request.form.get("symbol") or request.form.get("symbol").isnumeric():
            return apology("must provide stock symbol or wrong stock symbol", 400)

        amount = int(request.form.get("shares"))
        sym = request.form.get("symbol")
        ans = lookup(sym)
        price = float(ans["price"])

        # check the amount of money
        m = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])
        if (amount * price) > m:
            return apology("not enough money to purchase", 400)

        # stocks value in $
        purchase = amount * price
        new_m = m - purchase

        # check if the users has those stocks already
        stocks = list()
        for s in db.execute("SELECT symbol FROM wallet WHERE user_id = ?", session["user_id"]):
            stocks.append(s["symbol"])

        # if yes, update those stocks number, average price and value
        if sym in stocks:
            data = db.execute("SELECT shares, price, value FROM wallet WHERE symbol = ? AND user_id = ?", sym, session["user_id"])[0]
            new_price = ((float(data["price"]) * float(data["shares"])) + (amount * price))/(float(data["shares"]) + amount)
            new_value = float(data["value"]) + purchase

            # add the bought amount off stocks to the wallet
            db.execute("UPDATE wallet SET shares = ?, price = ?, value = ? WHERE symbol = ? AND user_id = ?", int(data["shares"]) + amount, new_price, new_value, sym, session["user_id"])
        # if no, then add a new stock to the wallet
        else:
            # insert the bought stocks into the db wallet
            db.execute("INSERT INTO wallet (user_id, symbol, shares, price, value) VALUES (?, ?, ?, ?, ?)", session["user_id"], sym, amount, price, purchase)

        # update the cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_m, session["user_id"])

        #################################################
        # calculate the date and time based on datetime function
        now = datetime.now()
        time = "%0.2d:%0.2d:%0.2d" % (now.hour, now.minute, now.second)
        date = "%2d-%02d-%4d" % (now.day, now.month, now.year)
        ttype = "PURCHASE"

        # add the new purchase transations to the database
        db.execute("INSERT INTO transactions (user_id, date, time, type, symbol, shares, price, total) VALUES (?, ?, ?, ?, ?, ? , ?, ?)", session["user_id"], date, time, ttype, sym, amount, price, -purchase)

        # go to the homepage
        return redirect("/")

    else:
        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # SELECT the data from transations for this user
    transactions = db.execute("SELECT date, time, type, symbol, shares, price, total FROM transactions WHERE user_id = ?", session["user_id"])

    return render_template("/history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["user"] = rows[0]["username"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        if not lookup(request.form.get("symbol")):
            return apology("Wrong stock symbol", 400)
        ans = lookup(request.form.get("symbol"))

        return render_template("/quoted.html", data=ans)

    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        usernames = set()

        data = db.execute("SELECT username FROM users")
        for d in data:
            usernames.add(d["username"])

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted and both match
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif not (request.form.get("password") == request.form.get("confirmation")):
            return apology("password and confirmation must be the same", 400)

        elif request.form.get("username") in usernames:
            return apology("user with this username already exists", 400)

        # add new user to the database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        # Redirect user to login
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        # validation of sell
        if not request.form.get("symbol"):
            return apology("Choose the sock to sell", 400)
        if not request.form.get("shares"):
            return apology("must provide the amount of stocks to sell", 400)
        if not request.form.get("shares").isdigit():
            return apology("must provide integer in amount of stocks", 400)

        amount = int(request.form.get("shares"))
        sym = request.form.get("symbol")

        # check the amount of stocks
        n = int(db.execute("SELECT shares FROM wallet WHERE symbol = ? AND user_id = ?", sym, session["user_id"])[0]["shares"])
        if amount > n:
            return apology("You don't have as much stocks, as you are trying to sell", 400)

        # Get the value of the stocks from wallet
        total = float(db.execute("SELECT value FROM wallet WHERE symbol =? AND user_id = ?", sym, session["user_id"])[0]["value"])

        # get the current price
        price = float(lookup(sym)["price"])

        # Get the current cash and calculate new, after sell
        money = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])
        sell = amount * price
        new_m = money + sell
        # new amount of shares
        a = n - amount
        # new total value of stocks
        new_total = total - sell

        if amount == n:
            # delete row with this stock from db
            db.execute("DELETE FROM wallet WHERE symbol = ? AND user_id = ?", sym, session["user_id"])
        else:
            # update row with this stock in db
            db.execute("UPDATE wallet SET shares = ?, value = ? WHERE symbol = ? AND user_id = ?", a, new_total, sym, session["user_id"])

        # update the cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_m, session["user_id"])

        #################################################
        # calculate the date and time based on datetime function
        now = datetime.now()
        time = "%0.2d:%0.2d:%0.2d" % (now.hour, now.minute, now.second)
        date = "%2d-%02d-%4d" % (now.day, now.month, now.year)
        ttype = "SALE"

        # add the new sale transations to the database
        db.execute("INSERT INTO transactions (user_id, date, time, type, symbol, shares, price, total) VALUES (?, ?, ?, ?, ?, ? , ?, ?)", session["user_id"], date, time, ttype, sym, -amount, price, sell)

        # go to the homepage
        return redirect("/")

    else:

        # load the list of symbols that user owns
        stocks = db.execute("SELECT symbol FROM wallet WHERE user_id = ?", session["user_id"])

        # show the page with the options to sell
        return render_template("/sell.html", stocks=stocks)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
