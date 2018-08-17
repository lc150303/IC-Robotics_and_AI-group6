# -*- coding: utf-8 -*-
import  cv2
import numpy as np
from scipy import stats

'''eight color boll : red yellow green brown blue pink black white
   HSV value:   red: H:0-10 156-180  S:43-255 V:46-255
	 	blue:H:100-124 S:43-255 V:46-255
		green:H:35-77 S:43-255 V:46-255
		black:H:0-180 S:0:255  V:0-46
		white:H:0-180 S:0-30 V:221-255
		yellow:H:26-34 S:43-255 V:46-255
'''

#the number of color  & low and high value of each color
NUM=6
color=['red','red','blue','green','black','white','yellow']
low=np.array([[156,43,46],[0,43,46],[98,43,46],[35,43,46],[0,0,0],[0,0,180],[20,43,46]])
high=np.array([[180,255,255],[10,255,255],[124,255,255],[ 77,255,255],[180,255,72],[180,30,255],[34,255,255]])


#read the image
img=cv2.imread('ball.jpg')
cv2.imshow('original',img)

#RGB to Gray
gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

#Cycle detection use Hough Transform
circles= cv2.HoughCircles(gray,cv2.HOUGH_GRADIENT,1,100,param1=140,param2=34.4,minRadius=10,maxRadius=80)

#color detect
for circle in circles[0]:
    #the coordinate of the circle
    x=int(circle[0])
    y=int(circle[1])
    #the radius
    r=int(circle[2])
    #在原图用指定颜色标记出圆的位置
    cut=img[y-r:y+r,x-r:x+r]
    #RGB to HSV
    cut=cv2.cvtColor(cut,cv2.COLOR_BGR2HSV)
    m=np.zeros([1,3])
    for i in range(0,3):
	m[0,i]=np.median(cut[:,:,i])
    for i in range(0,NUM):
    	l=m-low[i,:]
	h=m-high[i,:]
	if np.all(l>=0) and np.all(h<=0):	
		img=cv2.putText(img,color[i],(x+20,y+r+20),cv2.FONT_HERSHEY_COMPLEX,1,(0,0,0),2)
    		img=cv2.circle(img,(x,y),r+3,(0,0,255),4)
		print(color[i])
#show the new image

cv2.imshow('res',img)
cv2.imwrite('detect.jpg',img)

while True:
	key=chr(cv2.waitKey(15)&255)
	if key=='q':
		cv2.destroyAllWindows()
		break

