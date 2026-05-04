import numpy as np
import cv2
import os
# mở camera
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
# load cascade để nhận diện khuôn mặt
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
# nhập thông tin người dùng
face_id = input("Enter user id:")
face_name = input("Enter user name:")
print("Initializing face capture. Look at the camera and wait ...")
sample_number = 0
while True:
    # đọc camera
    ret, img = cap.read()
    img = cv2.flip(img, 1)
    # chuyển ảnh xám
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # tìm khuôn mặt trong ảnh
    # faces = face_cascade.detectMultiScale(gray, 1.3, 5)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=5, minSize=(100, 100),
                                          flags=cv2.CASCADE_SCALE_IMAGE)
    print("Found " + str(len(faces)) + " face(s)")
    # đóng khung khuôn mặt
    for (x, y, w, h) in faces:
        sample_number += 1  # Số ảnh lần lượt tăng dần +1
        if not os.path.exists('datasets'):
            os.makedirs('datasets')
        cv2.imwrite('datasets/User.' + str(face_id) + "." + str(sample_number) + ".jpg", img[y:y + h, x:x + w])
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
        # show video
        cv2.imshow("Frame", img)
    if cv2.waitKey(1) & 0xff == ord("q") or sample_number == 100:
        break;
