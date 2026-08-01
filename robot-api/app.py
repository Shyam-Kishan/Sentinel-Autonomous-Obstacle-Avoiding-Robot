from flask import Flask, request, jsonify
from robot.controller import move_robot
from robot.sensor import get_distance

app = Flask(__name__)

@app.route("/health", methods=["GET"])
def health():
    return jsonify({"status": "running"})

@app.route("/move", methods=["POST"])
def move():
    data = request.json
    command = data.get("command")

    result = move_robot(command)
    return jsonify({"result": result})

@app.route("/distance", methods=["GET"])
def distance():
    dist = get_distance()
    return jsonify({"distance": dist})

if __name__ == "__main__":
    app.run(debug=True)