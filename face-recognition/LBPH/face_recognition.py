from PIL import Image
import numpy as np
import cv2
import os
# load cascade để nhận diện khuôn mặt
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
# tạo bộ nhận dạng
recognizer = cv2.face_LBPHFaceRecognizer.create()
# gọi model đã train
recognizer.read('trainer.yml')
# mở camera
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
# thiết lập font chữ
font = cv2.FONT_HERSHEY_COMPLEX
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
    #print("Found " + str(len(faces)) + " face(s)")
    # quét lần lượt tất cả khuôn mặt trong ảnh
    for (x,y,w,h) in faces:
        # so sánh mặt trong model
        id_predicted, confidence = recognizer.predict(gray[y:y+h, x:x+w])
        # kiểm tra confidence < 100 : "0" là kết quả perfect
        if confidence <90:
            # id_predicted = "Known"
            # vẽ khung bao quanh khuôn mặt
            cv2.rectangle(img, (x-10, y-20), (x + w + 20, y + h + 40), (0, 255, 0), 2)
            cv2.putText(img, str(id_predicted), (x, y + h + 30), font, 0.4, (0, 255, 0), 1);
        else:
            id_predicted = "Unknown"
            # vẽ khung bao quanh khuôn mặt
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 0, 255), 2)
            cv2.putText(img, str(id_predicted), (x, y + h + 30), font, 0.4, (0, 0, 255), 1);
    #confidence = "  {0}%".format(round(100 - confidence))
    #print("\n" + str(confidence))
    #cv2.putText(img, str(confidence), (x + 5, y + h - 5), font, 1, (255, 255, 0), 1)
    cv2.imshow('Face Recognition', img)
    if cv2.waitKey(1) & 0xff == ord("q"):
        exit()