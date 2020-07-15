import numpy as np
import cv2
import random

img = cv2.imread(r"C:\Users\100592176\Desktop\Exercise Files\Ch03\03_10 Begin\fuzzy.png",1)
cv2.imshow("Original",img)

blur = cv2.GaussianBlur(img, (95,95),0)
cv2.imshow("Blur",blur)
gray = cv2.cvtColor(blur,cv2.COLOR_RGB2GRAY)
cv2.imshow("Gray",gray)
thresh = cv2.adaptiveThreshold(gray,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 225, 1)
cv2.imshow("Thresh",thresh)

#kernel = np.ones((15,15),'uint8')
#erode = cv2.erode(thresh,kernel, iterations=1)
#cv2.imshow("Erode",erode)


contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

img2 = img.copy()
index = -1
thickness = 4
color = (255,0,255)
canvas = np.ones([img.shape[0], img.shape[1],3],'uint8')*255


for c in contours:
	area = cv2.contourArea(c)
	perimeter = cv2.arcLength(c,True)

	if area > 1000:
			cv2.drawContours(canvas, [c], -1, (random.randrange(255),random.randrange(255),random.randrange(255)), -1)
			

	print("Area: {} | Perimeter: {}".format(area,perimeter))          




cv2.imshow("Canvas", canvas)
cv2.waitKey(0)
cv2.destroyAllWindows()