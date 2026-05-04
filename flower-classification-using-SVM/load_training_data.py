#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2 as cv
import numpy as np
import os

import Config 

so_anh = Config.so_anh_training

#hàm này load dữ liệu training và đổ vào list
def load_anh_training():
	rows1 = [] #tạo một mảng rỗng
	base_path = './HOTEST1/' #đường dẫn thư mục đến tập ảnh training số 1
	for dir in os.listdir(base_path):  #duyệt tất cả file trong đó
		if dir.endswith('.jpg'): #nếu nó là ảnh có đuôi .jpg thì 
			anh = cv.imread(base_path+dir,0)  #đọc ảnh đó
			#anh = cv.Canny(anh, 100, 200)
			rows1.append(anh)	#thêm vào cái mảng rỗng ở trên
	
	rows2 = []			
	base_path = './HOTEST2/'
	for dir in os.listdir(base_path):
		if dir.endswith('.jpg'):
			anh = cv.imread(base_path+dir,0)
			#anh = cv.Canny(anh, 100, 200)
			rows2.append(anh)
			
	rows3 = []
	base_path = './HOTEST3/'
	for dir in os.listdir(base_path):
		if dir.endswith('.jpg'):
			anh = cv.imread(base_path+dir,0)
			#anh = cv.Canny(anh, 100, 200)
			rows3.append(anh)
			
			
				
	cells = [] #thêm 4 tập dữ liệu training vào thành 1 danh sách
	cells.append(rows1)			
	cells.append(rows2)		
	cells.append(rows3)		
	
	return cells

	
def get_training():  
	cells = load_anh_training()
	train_cells = [ i[:so_anh] for i in cells ] #cái hàm này chỉ để lấy dữ liệu train ra thôi
	return train_cells #hàm này không cần thiết lắm
