from flask import Flask, request, jsonify
import os

app = Flask(__name__)

# Directory to save images
IMAGE_DIR = "images"
if not os.path.exists(IMAGE_DIR):
    os.makedirs(IMAGE_DIR)

@app.route('/upload_image', methods=['POST'])
def upload_image():
    img_data = request.data
    img_name = f"image_{len(os.listdir(IMAGE_DIR)) + 1}.jpg"
    img_path = os.path.join(IMAGE_DIR, img_name)

    with open(img_path, 'wb') as f:
        f.write(img_data)

    return "Image uploaded successfully", 200

@app.route('/sensor_data', methods=['POST'])
def sensor_data():
    data = request.json
    distance = data.get('distance', None)
    
    # You can process the distance data as needed
    print(f"Received distance data: {distance}")

    return jsonify({"status": "success"}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
