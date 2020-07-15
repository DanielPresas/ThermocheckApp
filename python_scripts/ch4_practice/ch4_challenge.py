import numpy as np
import cv2

img = cv2.imread(r"C:\Users\100592176\Desktop\Exercise Files\Ch04\04_06 Begin\faces.jpeg",1)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
path = r"C:\Users\100592176\Desktop\Exercise Files\Ch04\04_06 Begin\haarcascade_eye.xml"

eyes_cascade = cv2.CascadeClassifier(path)

eyes = eyes_cascade.detectMultiScale(gray,scaleFactor=1.02,minNeighbors=20,minSize=(10,10))
print(len(eyes))

for (x,y,w,h) in eyes:
	center = (x+w//2, y+h//2)
	radius = (w+h)//4
	cv2.circle(img,center,radius,(255,0,0),2)

cv2.imshow("Eyes",img)

cv2.waitKey(0)
cv2.destroyAllWindows()