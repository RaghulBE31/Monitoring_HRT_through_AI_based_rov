import cv2
import os
import urllib.request
import numpy as np

url = 'http://192.168.137.122/capture'
output_folder = 'dataset'

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

counter = 0

while True:
    img_resp = urllib.request.urlopen(url)
    img_np = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    frame = cv2.imdecode(img_np, -1)

    cv2.imshow('Video', frame)

    # Save the current frame automatically
    counter += 1
    image_filename = os.path.join(output_folder, f'imGaea_{counter}.jpg')  # Updated filename format
    cv2.imwrite(image_filename, frame)
    print(f"Image {counter} saved as {image_filename}")

    # Press 'q' to quit the program
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

cv2.destroyAllWindows()

