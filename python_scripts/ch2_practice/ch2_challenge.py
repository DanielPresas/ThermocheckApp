import numpy as np
import cv2

# Global variables
canvas = np.ones([500,500,3],'uint8')*255

colour = (0,255,0)
radius = 3
pressed = False





# click callback
def draw_circle(event, x, y, flags, param):
	global canvas, pressed

	if event == cv2.EVENT_LBUTTONDOWN:
		pressed = True
		cv2.circle(canvas,(x,y),radius,colour,-1)
		print("Pressed", x,y)
		
	
	elif event == cv2.EVENT_MOUSEMOVE and pressed == True:
		cv2.circle(canvas,(x,y),radius,colour,-1)

	elif event == cv2.EVENT_LBUTTONUP:
		pressed = False
		
		


# window initialization and callback assignment
cv2.namedWindow("canvas")
cv2.setMouseCallback("canvas", draw_circle)

# Forever draw loop
while True:

	cv2.imshow("canvas",canvas)
	

	# key capture every 1ms
	ch = cv2.waitKey(1)
	if ch & 0xFF == ord('q'):
		break

	elif ch & 0xFF == ord('b'):
		colour = (255,0,0)
	
	elif ch & 0xFF == ord('g'):
		colour = (0,0,255)
	

cv2.destroyAllWindows()