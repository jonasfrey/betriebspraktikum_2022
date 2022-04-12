import cv2 
import numpy

# allright , this should simply store a image with a red pixel in the top left corner, right ?

a_img = numpy.zeros(
    [
        10, 
        10, 
        3
    ],
    dtype=numpy.uint8, 

)
a_img[0][0][0] = 255
a_img[0][0][1] = 0
a_img[0][0][2] = 0

# check out the jpg, it has other pixels with a slight blue
cv2.imwrite("png_vs_jpg.jpg", a_img)
# png does not have this
cv2.imwrite("png_vs_jpg.png", a_img)

