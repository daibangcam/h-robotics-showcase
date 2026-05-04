from PIL import Image
import numpy as np
import cv2
import os
# khai báo nơi lưu ảnh face để training
path = 'datasets'
# tạo bộ training
recognizer = cv2.face.LBPHFaceRecognizer_create()
# hàm con lấy ảnh và id name
def getImagesAndLabels(path):
    image_paths = [os.path.join(path,f) for f in os.listdir(path)]
    face_samples=[]
    face_ids = []
    for imagePath in image_paths:
        face_img = Image.open(imagePath).convert('L')
        face_numpy = np.array(face_img,'uint8')
        id = int(os.path.split(imagePath)[-1].split(".")[1])
        face_samples.append(face_numpy)
        face_ids.append(id)
        cv2.imshow('Training', face_numpy)
        cv2.waitKey(10)
    return face_samples, np.array(face_ids)
# lấy ảnh và id name
print ("Training faces. It will take a few seconds. Wait ...")
faces,ids = getImagesAndLabels(path)
recognizer.train(faces, ids)
# tạo folder lưu trữ model
# if not os.path.exists('trainer'):
#     os.makedirs('trainer')
# lưu trữ model
recognizer.write('trainer.yml')
print("{0} faces trained. Exiting Program".format(len(np.unique(ids))))