import numpy as np 
import cv2

color = cv2.imread(r"C:\Users\100592176\Desktop\Exercise Files\Ch02\02_05 Begin\butterfly.jpg", 1)

gray = cv2.cvtColor(color,cv2.COLOR_RGB2GRAY)
cv2.imwrite(r"C:\Users\100592176\Desktop\Exercise Files\Ch02\02_05 Begin\graybutterfly.jpg",gray)

b = color[:,:,0]
g = color[:,:,1]
r = color[:,:,2]

rgba = cv2.merge((b,g,r,g))
cv2.imwrite(r"C:\Users\100592176\Desktop\Exercise Files\Ch02\02_05 Begin\rgba.jpg", rgba)