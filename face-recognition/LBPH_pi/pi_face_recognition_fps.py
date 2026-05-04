from picamera.array import PiRGBArray
from picamera import PiCamera
import cv2
import serial
import imutils
from imutils.video import FPS
# lấy hàm fps ra xài
from time import sleep

ser = serial.Serial('/dev/ttyACM0',9600) #cổng kết nối của arduino với pi
# initialize the camera and grab a reference to the raw camera capture
# khởi tạo máy ảnh và lấy tham chiếu đến camera thô
camera = PiCamera()
fps = FPS().start()
#khởi tạo cam đồng thời đếm tốc độ fps
camera.resolution = (320, 240)
camera.framerate = 24
rawCapture = PiRGBArray(camera, size=(320, 240)) #chuyển điểm ảnh sang dạng mảng
#use Local Binary Patterns Histograms
recognizer = cv2.face.LBPHFaceRecognizer_create()
#Load a trainer file
recognizer.read('trainer.yml') #đọc file model đã train
#Load a cascade file for detecting faces
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
font = cv2.FONT_HERSHEY_SIMPLEX
#iniciate id counter
id = 0
# names related to ids: example ==> Marcelo: id=1,  etc
names = ['none', 'Phat', 'Do'] 
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    # convert frame to array
    image = frame.array
    #Convert to grayscale
    gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
    #Look for faces in the image using the loaded cascade file
    faces = face_cascade.detectMultiScale(gray, scaleFactor = 1.2, minNeighbors = 5, minSize = (100, 100), flags = cv2.CASCADE_SCALE_IMAGE)

    print ("Found "+str(len(faces))+" face(s)")
    #Draw a rectangle around every found face
    for (x,y,w,h) in faces:
        roi_gray = gray[y:y + h, x:x + w]
        cv2.rectangle(image,(x,y),(x+w,y+h),(255,0,0),2)
        id, confidence = recognizer.predict(gray[y:y+h,x:x+w])
        # Check if confidence is less them 100 ==> "0" is perfect match 
        if (confidence < 93):
            id = names[id]
            confidence = "  {0}%".format(round(100 - confidence))
        else:
            id = "unknown"
            confidence = "  {0}%".format(round(100 - confidence))
        # vẽ khung % và tên khuôn mặt
        cv2.putText(image, str(id), (x+5,y-5), font, 1, (255,255,255), 2)
        cv2.putText(image, str(confidence), (x+5,y+h-5), font, 1, (255,255,0), 1)  
        print(x,y,w,h)
        string= str(id)
        ser.write(string.encode()) #gởi tên khuôn mặt xuống cổng arduino
        sleep(1) #ngừng gởi trong vòng 1s     
    # display a frame    
    cv2.imshow("Frame", image)
    if cv2.waitKey(1) & 0xff == ord("q"):
        exit()
    
    #cập nhật tốc dộ bn khung ảnh trong/1s
    # clear the stream in preparation for the next frame
    rawCapture.truncate(0)
    fps.update()
fps.stop()
print("[INFO] elapsed time: {:.2f}".format(fps.elapsed()))
print("[INFO] approx. FPS: {:.2f}".format(fps.fps()))