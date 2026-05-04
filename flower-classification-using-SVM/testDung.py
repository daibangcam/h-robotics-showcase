import cv2 as cv
import numpy as np
import os
import time
#------------------------------------#
start_time=time.time()
for i in range (10000):
    pass
SZ=100
bin_n = 16 # Number of bins
affine_flags = cv.WARP_INVERSE_MAP|cv.INTER_LINEAR
def deskew(img):
    m = cv.moments(img)
    if abs(m['mu02']) < 1e-2:
        return img.copy()
    skew = m['mu11']/m['mu02']
    M = np.float32([[1, skew, -0.5*SZ*skew], [0, 1, 0]])
    img = cv.warpAffine(img,M,(SZ, SZ),flags=affine_flags)
    #print img
    return img
def hog(img):
    gx = cv.Sobel(img, cv.CV_32F, 1, 0)
    gy = cv.Sobel(img, cv.CV_32F, 0, 1)
    mag, ang = cv.cartToPolar(gx, gy)
    bins = np.int32(bin_n*ang/(2*np.pi))    # quantizing binvalues in (0...16)
    bin_cells = bins[:10,:10], bins[10:,:10], bins[:10,10:], bins[10:,10:]
    mag_cells = mag[:10,:10], mag[10:,:10], mag[:10,10:], mag[10:,10:]
    hists = [np.bincount(b.ravel(), m.ravel(), bin_n) for b, m in zip(bin_cells, mag_cells)]
    hist = np.hstack(hists)     # hist is a 64 bit vector
    #print hist
    return hist

#--------------------------------------#




cells = []
rows1 = []
base_path = './HOTEST1/'
for dir in os.listdir(base_path):
	if dir.endswith('.jpg'):
		anh = cv.imread(base_path+dir,0)
		#anh = cv.Canny(anh, 100, 200)
		rows1.append(anh)
		#print rows1
		
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
		
		
			
cells = []
cells2 = []
cells.append(rows1)			
cells.append(rows2)		
cells.append(rows3)			
cells2 = cells
#print cells

	

#data training ok 			
train_cells = [ i[:100] for i in cells ]
#test_Data = [ i[:200] for i in cells2 ]

deskewed = [map(deskew,row) for row in train_cells]
hogdata = [map(hog,row) for row in deskewed]
trainData = np.float32(hogdata).reshape(-1,64)
responses = np.repeat(np.arange(3),100)[:,np.newaxis]
#print hogdata
#A=np.array(hogdata)
#print len(A)
#print A
#print responses
#print trainData
#train
svm = cv.ml.SVM_create()
svm.setKernel(cv.ml.SVM_LINEAR)
svm.setType(cv.ml.SVM_C_SVC)
svm.setC(2.67)
svm.setGamma(3)
svm.train(trainData, cv.ml.ROW_SAMPLE, responses)
svm.save('svm_data.txt')
#train xong

#test

test_Data = cv.imread('3.2.jpg', 0)
#test_Data = cv.Canny(anh, 100, 200)
deskewed = deskew(test_Data)

#print deskewed
#thaydoi = cv.resize(deskewed,(500,500))
#cv.imshow('aa',thaydoi)
#cv.waitKey(0)
#cv.destroyAllWindows();
hogdata = hog(deskewed)
#print hogdata
#thaydoi = cv.resize(hogdata,(1000,2000))
#cv.imshow('bb',thaydoi)
#cv.waitKey(0)
#cv.destroyAllWindows()
testData = np.float32(hogdata).reshape(-1,bin_n*4)
#print testData
#thaydoi = cv.resize(testData,(1000,2000))
#cv.imshow('cc',thaydoi)
#cv.waitKey(0)
#cv.destroyAllWindows()
result = svm.predict(testData)[1]
print result


if result == 2:
	print 'day la hoa DAI'
elif result == 1:
	print 'day la hoa VAN THO'
else:
	print 'day la hoa CUC HUAN CHUONG'

end_time=time.time()-start_time
print end_time

"""

cells = []
row = []
img = cv2.imread('A_1.jpg',0)
print img
abc  = img.tolist()
row.append(abc)
print
print 

print cells

"""
