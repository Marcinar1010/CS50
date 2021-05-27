import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        data = db.execute("SELECT name FROM birthdays")
        s = set()
        for d in data:
            s.add(d["name"].lower())
        birthdays = db.execute("SELECT * FROM birthdays")
        # validation

        if not name:
            message = "Missing name"
            return render_template("index.html", birthdays=birthdays, message=message)
        if not month:
            message = "Missing month"
            return render_template("index.html", birthdays=birthdays, message=message)
        if not day:
            message = "Missing day"
            return render_template("index.html", birthdays=birthdays, message=message)
        if name.lower() in s:
            message = "Name already exists"
            return render_template("index.html", birthdays=birthdays, message=message)


        if name and month and day:
            db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)

        return redirect("/")

    else:
        message = ""
        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays, message=message)


@app.route("/delete", methods=["POST"])
def delete():

    id = request.form.get("id")
    if id:
        db.execute("DELETE FROM birthdays WHERE id = ?", id)
    return redirect("/")