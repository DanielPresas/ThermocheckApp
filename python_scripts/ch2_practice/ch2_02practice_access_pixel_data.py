import numpy as np
import cv2

img = cv2.imread(r"C:\Users\100592176\Desktop\Exercise Files\Ch02\02_02 Begin\opencv-logo.png",1)

print(img)
print(type(img))
print(len(img))
print(len(img[0]))
print(len(img[0][0]))
print(img.shape)
print(img.dtype)
print(img [10,5])
img2 = img[:,:,0]

cv2.namedWindow("Image",cv2.WINDOW_NORMAL)
cv2.imshow("Image",img2)
cv2.waitKey(0)