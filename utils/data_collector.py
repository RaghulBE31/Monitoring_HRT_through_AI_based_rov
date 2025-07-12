import cv2
import urllib.request
import numpy as np
from model_loader import predict_image  # Your own model inference code
from PIL import Image

url = 'http://192.168.137.122/capture'  # Change to your ESP32-CAM IP

while True:
    try:
        # Fetch JPEG image from ESP32-CAM
        img_resp = urllib.request.urlopen(url)
        img_np = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        frame = cv2.imdecode(img_np, -1)

        if frame is None:
            continue

        # Show video feed in real-time
        cv2.imshow('Live Video Feed', frame)

        # Convert BGR (OpenCV) to RGB (PIL/Image)
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        pil_img = Image.fromarray(frame_rgb)
        pil_img.save("temp.jpg")  # Save one temporary image for predict_image()

        # Run ML model (assumes your model_loader works with image paths)
        label, confidence = predict_image("temp.jpg")

        print(f"[Prediction] {label} ({confidence*100:.2f}%)")

        # Exit on 'q' key
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    except Exception as e:
        print("Error fetching or processing frame:", e)

cv2.destroyAllWindows()
