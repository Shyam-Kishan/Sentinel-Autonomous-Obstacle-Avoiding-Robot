from flask import Flask, request, jsonify, render_template
from mainRobot import Robot
import atexit

app = Flask(__name__)
robot = Robot()

@app.route('/')
def home():
    return render_template('index.html')

@app.route("/health", methods=["GET"])
def health():
    return jsonify({"status": "running"})

@app.route("/move", methods=["POST"])
def move():
    data = request.json
    command = data.get("command")

    result = robot.send_command(command)
    return jsonify({"result": result})

@app.route("/distance", methods=["GET"])
def distance():
    dist = robot.get_distance()
    return jsonify({"distance": dist})

@app.route("/telemetry", methods=["GET"])
def telemetry():
    data = robot.get_telemetry()
    return jsonify(data)

@atexit.register
def shutdown():
    print("Shutting down robot...")
    robot.close()

if __name__ == "__main__":
    app.run(debug=True)