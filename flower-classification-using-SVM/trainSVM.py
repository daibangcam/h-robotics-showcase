#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2 as cv
import numpy as np
import os
import load_training_data
import lib
import Config
import time
#số ảnh train
start_time =time.time()
for i in range(100000):
        pass
so_anh = Config.so_anh_training
for i in range (1000000):
        pass
#hàm này dùng để phân loại
def get_class_object(url_img):
	#lấy dữ liệu training
	train_cells = load_training_data.get_training()
	#bắt đầu xử lý phục chế một chút cho ảnh train
	deskewed = [map(lib.deskew,row) for row in train_cells]
	#tiến hành trích chọn vector đặc trưng với mỗi dữ liệu training
	hogdata = [map(lib.hog,row) for row in deskewed]
	trainData = np.float32(hogdata).reshape(-1,64) #chuyển đổi dữ liệu training thành mảng 1 chiều
	#tạo danh sách các nhãn tương ứng với tập dữ liệu training
	responses = np.repeat(np.arange(3),so_anh)[:,np.newaxis]
	#print responses
	#print trainData
	#train
	#khởi tạo hàm SVM của openCV
	svm = cv.ml.SVM_create()
	svm.setKernel(cv.ml.SVM_LINEAR)
	svm.setType(cv.ml.SVM_C_SVC)
	svm.setC(2.67)
	svm.setGamma(3)
	#khởi tạo xong bắt đầu train dữ liệu dựa trên datatrain và dữ liệu đã gắn nhãn 
	#nhãn ở đây được lưu trong responses
	svm.train(trainData, cv.ml.ROW_SAMPLE, responses)
	svm.save('svm_data.txt') #lưu lại phòng khi sau này sử dụng
	
	#train xong
	#---------------------------#
	#bắt đầu nhận dạng
	#ĐỌc dữ liệu từ ảnh nhận dạng ảnh này có đường dẫn được truyên vào từ hàm
	test_Data = cv.imread(url_img, 0)
	#test_Data = cv.Canny(anh, 100, 200)
	#xử lý lọc nhiều phục chế qua một chút
	deskewed = lib.deskew(test_Data)
	#bắt đầu trích chọn đặc trưng
	hogdata = lib.hog(deskewed)
	#chuyển đổi về kiểu danh sách một chiều và ép về kiểu float 32 bit
	testData = np.float32(hogdata).reshape(-1,lib.bin_n*4)
	#bắt đầu phân lớp cho dữ liệu test
	result = svm.predict(testData)[1]
	#trả về kết quả
	return result
end_time=time.time() - start_time
print 'thoi gian train :',end_time

