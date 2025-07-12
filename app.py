from flask import Flask, request, render_template, jsonify, send_from_directory
import os
from datetime import datetime
from model_loader import predict_image
import RPi.GPIO as GPIO

app = Flask(__name__)
UPLOAD_FOLDER = 'received_images'


GPIO.setmode(GPIO.BCM)
MOTOR_PINS = {
    'left': (17, 18),    # IN1, IN2
    'right': (22, 23)    # IN3, IN4
}

for pins in MOTOR_PINS.values():
    for pin in pins:
        GPIO.setup(pin, GPIO.OUT)
        GPIO.output(pin, GPIO.LOW)

def stop_motors():
    for pins in MOTOR_PINS.values():
        for pin in pins:
            GPIO.output(pin, GPIO.LOW)

def move_forward():
    GPIO.output(17, GPIO.HIGH)
    GPIO.output(18, GPIO.LOW)
    GPIO.output(22, GPIO.HIGH)
    GPIO.output(23, GPIO.LOW)

def move_backward():
    GPIO.output(17, GPIO.LOW)
    GPIO.output(18, GPIO.HIGH)
    GPIO.output(22, GPIO.LOW)
    GPIO.output(23, GPIO.HIGH)

def turn_left():
    GPIO.output(17, GPIO.LOW)
    GPIO.output(18, GPIO.HIGH)
    GPIO.output(22, GPIO.HIGH)
    GPIO.output(23, GPIO.LOW)

def turn_right():
    GPIO.output(17, GPIO.HIGH)
    GPIO.output(18, GPIO.LOW)
    GPIO.output(22, GPIO.LOW)
    GPIO.output(23, GPIO.HIGH)

COMMANDS = {
    'forward': move_forward,
    'backward': move_backward,
    'left': turn_left,
    'right': turn_right,
    'stop': stop_motors
}

if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

latest_result = {'status': 'Waiting...', 'confidence': '0%'}

@app.route('/')
def dashboard():
    return render_template('dashboard.html', result=latest_result)

@app.route('/upload_image', methods=['POST'])
def upload_image():
    global latest_result
    filename = datetime.now().strftime("frame_%Y%m%d_%H%M%S.jpg")
    filepath = os.path.join(UPLOAD_FOLDER, filename)
    with open(filepath, 'wb') as f:
        f.write(request.data)

    result, conf = predict_image(filepath)
    latest_result['status'] = result
    latest_result['confidence'] = f"{conf*100:.2f}%"
    return jsonify(latest_result)

@app.route('/latest_image')
def latest_image():
    files = sorted(os.listdir(UPLOAD_FOLDER))
    if not files:
        return "No image yet", 404
    return send_from_directory(UPLOAD_FOLDER, files[-1])

@app.route('/send_command/<cmd>')
def send_command(cmd):
    try:
        if cmd in COMMANDS:
            COMMANDS[cmd]()
            return jsonify({'status': 'executed', 'response': f'Command {cmd} executed'})
        else:
            return jsonify({'status': 'error', 'response': 'Invalid command'})
    except Exception as e:
        return jsonify({'status': 'error', 'response': str(e)})

@app.route('/cleanup')
def cleanup():
    stop_motors()
    GPIO.cleanup()
    return "GPIO cleaned up."

if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', port=5000, debug=True)
    finally:
        GPIO.cleanup()
