import cv2 
import os
import random 

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../../2019-10-26T03-26-55_M51_Clear_280s_big.jpg'
if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

o_img = cv2.imread(s_path_file_name)

def f_set_pixel_value(a_pixel, n_value):
    a_pixel[0] = n_value
    a_pixel[1] = n_value
    a_pixel[2] = n_value

a_img = o_img
n_img_width = o_img.shape[0]
n_img_height = o_img.shape[1]

n_below_blacklevel = 255/18.3
n_pixel_count_below_blacklevel = 0

# loop over the image, pixel by pixel
for n_y in range(0,n_img_height):
    for n_x in range(0,n_img_width):
        # threshold the pixel 
        a_pixel = a_img[n_y][n_x]
        n_value = a_pixel[0]

        n_value = int(n_value)
        n_value_normalized = n_value / 32
        
        f_set_pixel_value(
                a_pixel,
                min(n_value_normalized * n_value * 0.3, 255)
            )
       
        if n_value < n_below_blacklevel:
            f_set_pixel_value(a_pixel,0)
            n_pixel_count_below_blacklevel += 1

        if n_value > n_below_blacklevel:
            f_set_pixel_value(
                a_pixel,
                min(n_value_normalized * 255, 255)
            )

b = cv2.imwrite(__file__.split(".")[0] +".jpg", o_img)
b = cv2.imwrite("illuminate.jpg", o_img)
print(b)