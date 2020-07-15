import numpy as np 
import cv2

img = cv2.imread(r"C:\Users\100592176\Desktop\Exercise Files\Ch03\03_04 Begin\faces.jpeg",1)
hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
resized_img = cv2.resize(img, (0,0), fx=0.2, fy=0.2) 

h = hsv[:,:,0]
s = hsv[:,:,1]
v = hsv[:,:,2]

hsv_split = np.concatenate((h,s,v), axis=1)
resized_hsv = cv2.resize(hsv_split, (0,0), fx=0.2, fy=0.2) 
cv2.imshow("Split HSV", resized_hsv)

ret, min_sat = cv2.threshold(s,40,255,cv2.THRESH_BINARY)
resized_min_sat = cv2.resize(min_sat, (0,0), fx=0.2, fy=0.2) 
cv2.imshow("Sat Filter",resized_min_sat)

ret, max_hue = cv2.threshold(h, 15, 255, cv2.THRESH_BINARY_INV)
resized_max_hue = cv2.resize(max_hue, (0,0), fx=0.2, fy=0.2)
cv2.imshow("Hue Filter", resized_max_hue)

final = cv2.bitwise_and(resized_min_sat,resized_max_hue)
cv2.imshow("Final",final)
cv2.imshow("Original", resized_img)

cv2.waitKey(0)
cv2.destroyAllWindows()
