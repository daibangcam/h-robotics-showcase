import tkinter as tk
from tkinter import ttk
from tkinter.ttk import Frame, Label, Scale, Style
from PIL import Image, ImageTk

import numpy as np
import os
import pickle, sqlite3
import cv2

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

def face_recognition():
    recognizer = cv2.face_LBPHFaceRecognizer.create()
    recognizer.read("trainner.yml")
    def getProfile(Id):
        conn=sqlite3.connect("face_database.db")
        query="SELECT * FROM People WHERE ID="+str(Id)
        cursor=conn.execute(query)
        profile=None
        for row in cursor:
            profile=row
        conn.close()
        return profile
    cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    while True:
        ret,img = cap.read()
        img = cv2.flip(img, 1)
        gray =cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces =face_cascade.detectMultiScale(gray, 1.3,2)
        for (x,y,w,h) in faces:
            cv2.rectangle(img, (x,y), (x+w, y+h), (0,255,0), 2)
            id_predicted, id_confidence = recognizer.predict(gray[y:y+h, x:x+w])
            if id_confidence < 90:
                profile=getProfile(id_predicted)
                if profile != None:
                    cv2.putText(img, ""+str(profile[1]), (x+10, y), cv2.FONT_HERSHEY_COMPLEX, 1, (0,255,0), 1);
            else:
                cv2.putText(img, "Unknown", (x, y + h + 30), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 1);
        cv2.imshow('Detecting face', img)
        if(cv2.waitKey(1) == ord('q')):
            break
    cap.release()
    cv2.destroyAllWindows()

def train():
    recognizer = cv2.face_LBPHFaceRecognizer.create()
    path = 'datasets'
    def getImagesWithID(path):
        image_paths=[os.path.join(path, f) for f in os.listdir(path)]
        face_names=[]
        face_IDs=[]
        for i in image_paths:
            face_img = Image.open(i).convert('L')
            face_np = np.array(face_img, 'uint8')
            face_ID=int(os.path.split(i)[-1].split('.')[1])
            face_names.append(face_np)
            face_IDs.append(face_ID)
            cv2.imshow('Training', face_np)
            cv2.waitKey(1)
        return np.array(face_IDs), face_names
    ids, faces = getImagesWithID(path)
    recognizer.train(faces, ids)
    recognizer.save("trainner.yml")
    cv2.destroyAllWindows()

def get_datasets():
    def insertOrUpdate(id, name):
    #connecting to the db
        conn =sqlite3.connect("face_database.db")
        #check if id already exists
        query = "SELECT * FROM People WHERE ID="+str(id)
        #returning the data in rows
        cursor = conn.execute(query)
        isRecordExist=0
        for row in cursor:
            isRecordExist=1
        if isRecordExist==1:
            query="UPDATE People SET Name="+str(name)+" WHERE ID="+str(id)
        else:
            query="INSERT INTO People(ID, Name) VALUES("+str(id)+","+str(name)+")"
        conn.execute(query)
        conn.commit()
        conn.close()
    id = gui_id_face.get()
    name ="'"+gui_name_face.get()+"'"
    insertOrUpdate(id, name)
    sample_number = 0
    cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    while True:
        ret, img = cap.read()
        img = cv2.flip(img, 1)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, 1.3, 5)
        for (x,y,w,h) in faces:
            sample_number += 1
            if not os.path.exists('datasets'):
                os.makedirs('datasets')
            cv2.imwrite('datasets/User.'+str(id)+"."+str(sample_number)+".jpg",  gray[y:y+h,x:x+w])
            cv2.rectangle(img, (x,y), (x+w, y+h), (0,255,0), 2)
        cv2.imshow('Detecting face', img)
        cv2.waitKey(1);
        if(sample_number == 100):
            cap.release()
            cv2.destroyAllWindows()
            break;
    edit_id.delete(0,"end")
    edit_name.delete(0,"end")

# tạo GUI
gui = tk.Tk()
gui.title("Face Recognition ")
gui.geometry('512x349')
# chèn ảnh nền cho GUI
bg = Image.open("background.jpg")
bg_resize = bg.resize((512,349))
img_bg = ImageTk.PhotoImage(bg_resize)
img = Label(gui, image= img_bg) #ghép biến a vào giao diện
img.grid(column=0, row=0)
# màu background GUI
gui.configure(bg='#263D42') # màu nền giao diện
# thêm widget text
label = ttk.Label(gui,text="FACE RECOGNITION SYSTEM",background="grey",foreground="white",font=("Verdana",20))
label.place(x=60,y=50)
# tạo  ID
label1 = ttk.Label(gui,text="ID:",background="#263D42",foreground="white", font=("Time New Roman",14))
label1.place(x=40,y=140)
gui_id_face =tk.IntVar()
edit_id=ttk.Entry(gui,textvariable=gui_id_face, width=50)
edit_id.focus()
edit_id.place(x=110,y=140)
# tạo name
label2 = ttk.Label(gui,text="Name:",background="#263D42",foreground="white", font=("Time New Roman",14))
label2.place(x=40,y=180)
gui_name_face =tk.StringVar()
edit_name=ttk.Entry(gui,textvariable=gui_name_face,width=50)
edit_name.place(x=110,y=180)
#Tạo nút nhấn
btn_get_datasets= ttk.Button(gui, text ="Register", command=get_datasets)
btn_get_datasets.place(x=110,y=240)
btn_train_datasets= ttk.Button(gui, text ="Train", command=train)
btn_train_datasets.place(x=240,y=240)
btn_recognition= ttk.Button(gui, text ="Recognize", command=face_recognition)
btn_recognition.place(x=375,y=240)
gui.mainloop()