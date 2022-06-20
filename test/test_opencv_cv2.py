import cv2
import sys
url='rtsp://192.168.1.142:554/live2'
cap=cv2.VideoCapture(url)
# cap.set(3,480)
print(cap.isOpened())
print(sys.version)
print(cv2.__version__)
while cap.isOpened():
	ret, img_camera=cap.read()
	cv2.imshow("camera",img_camera)
	k=cv2.waitKey(1)
	if k==ord('s'):
		cv2.imwrite("test.jpg",img_camera)
	if k==ord('q'):
		break
cap.release
cv2.destroyAllWindows()

