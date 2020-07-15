import numpy as np 
import cv2

img = cv2.imread(r"C:\Users\100592176\Desktop\Exercise Files\Ch02\02_01 Begin\opencv-logo.png")
cv2.namedWindow("Image",cv2.WINDOW_NORMAL)
cv2.imshow("Image",img)

cv2.waitKey(0)