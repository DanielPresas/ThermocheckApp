import numpy as np
import cv2

img = cv2.imread(r"C:\Users\100592176\Desktop\Exercise Files\Ch04\04_05 Begin\faces.jpeg",1)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
path = r"C:\Users\100592176\Desktop\Exercise Files\Ch04\04_05 Begin\haarcascade_frontalface_default.xml"

face_cascade = cv2.CascadeClassifier(path)

faces = face_cascade.detectMultiScale(gray,scaleFactor=1.10,minNeighbors=5,minSize=(40,40))
print(len(faces))

for (x,y,w,h) in faces:
	cv2.rectangle(img, (x,y),(x+w,y+h),(0,255,0),2)

cv2.imshow("Image",img)

cv2.waitKey(0)
cv2.destroyAllWindows()