import cv2
import numpy as np
from PyQt5.QtWidgets import *
from PyQt5 import uic, QtWidgets, QtGui, QtCore
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtGui import QImage, QPixmap
import os
import sys
import math
from toucamera import *


root = os.path.dirname(__file__)
arear = 0

class open_giao_dien(QMainWindow):
    def __init__(self):
        super().__init__()
        self.timer = QTimer()
        uic.loadUi('open_image.ui',self)
        self.img = []
        self.processedImage = []
        self.mo_cam()
        self.Button_auto.clicked.connect(self.manual)
        self.save_img.clicked.connect(self.saveimg)
        self.auto()
        self.file = 'E:/hieu/mo cam1'
    def saveimg(self):
        self.close_camera()
        cv2.imwrite('cbr', self.frame)
    def close_camera(self):
        self.timer.stop()
    def auto(self):
        self.Button_auto.setStyleSheet("background-color: green;font: 12pt;bold;")
        self.Button_auto.setText('Auto')
        # self.Button_test.setEnabled(False)
        # self.Button_find_area.setEnabled(False)
    def manual(self):
        if not self.Button_auto.isChecked():
            self.Button_auto.setStyleSheet("background-color: red;font: 12pt;bold;")
            self.Button_auto.setText('Manual')
            # self.Button_test.setEnabled(True)
            # self.Button_find_area.setEnabled(True)

    def mo_cam(self):
        self.fps = 1000000
        name = ['0', '0']
        self.cam = [0, 1]
        self.cnum_vitri = get_Enum()
        self.isCapturing = True
        if self.isCapturing:
            for i in range(0, self.cnum_vitri):
                if i == 0:
                    self.cam[i] = ToupCamCamera(cid=i)
                    self.cam[i].open(1)
                    self.cam[i].set_auto_exposure(False)
                    self.cam[i].set_ExpoTime(10000)
                    self.cam[i].set_HZ(100)
                    self.cam[i].set_Speed(64)
                    self.cam[i].set_brightness(60)  # tang do sang
                    name[i] = self.cam[i].get_serial()
                    self.Starttimer_vitri()

    def Starttimer_vitri(self):
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.Displayframe_vitri)
        self.timer.start(1000. / self.fps)
    def Displayframe_vitri(self):
        for i in range (0,self.cnum_vitri):
          if i == 0:
            self.frame= np.array(self.cam[i].get_pil_image())
            frame = self.frame.copy()
            img = QtGui.QImage(frame, frame.shape[1],
                               frame.shape[0], QtGui.QImage.Format_RGB888)
            img_pix = QtGui.QImage(self.frame, self.frame.shape[1],
                               self.frame.shape[0], QtGui.QImage.Format_RGB888)
            self.pix = QtGui.QPixmap.fromImage(img_pix)
            self.img_vitri = QtGui.QPixmap.fromImage(img)
            self.label_hieu.setPixmap(self.img_vitri)
    def displayImage(self, window=1):
        qformat = QImage.Format_Indexed8
        if len(self.processedImage.shape) == 3:
            if (self.processedImage.shape[2]) == 4:
                qformat = QImage.Format_RGBA8888
            else:
                pass
        self.img = QImage(self.processedImage, self.processedImage.shape[1], self.processedImage.shape[0], self.processedImage.strides[0], qformat)
        if window == 1:
            self.label_hieu.setPixmap(QPixmap.fromImage(self.img))
            self.label_hieu.setAlignment(QtCore.Qt.AlignVCenter)
        if window == 2:
            self.label_add_anh.setPixmap(QPixmap.fromImage(self.img))
            self.label_add_anh.setAlignment(QtCore.Qt.AlignVCenter)
app = QApplication(sys.argv)
f = open_giao_dien()
f.show()
sys.exit(app.exec_())
