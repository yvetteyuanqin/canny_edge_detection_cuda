import numpy as np
import matplotlib.image as img

def normalize(arr,w,h):
	for i in range(w):
		for j in range(h):
			arr[i][j] = arr[i][j]*255
	return arr

image = img.imread('001.png')
print (image.shape)
s = image.shape
#a = np.array(img)
b = np.fromstring(normalize(image,s[0],s[1]),dtype=int).reshape(512, 512)

out = open('001_py.txt', 'w')
out.write(b)