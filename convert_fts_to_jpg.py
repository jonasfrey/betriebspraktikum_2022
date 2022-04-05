import numpy as np
from PIL import Image
from astropy.io import fits
import cv2 
import sys
import time
# call it 
# python3 convert_fts_to_jpg.py 2021-07-02T00-57-52_M17_Open_280s_Jonas-F.fts

s_path_file_name = sys.argv[1]
a_s_parts = s_path_file_name.split(".")
a_s_parts.pop()
a_s_parts.append(str(int(time.time())))
a_s_parts.append("jpg")

s_path_file_name_jpg = ".".join(a_s_parts)

# print(s_path_file_name_jpg)
# fits_image_filename = fits.util.get_testdata_filepath(s_path_file_name)
o_hdulist = fits.open(s_path_file_name)

a_image_data = o_hdulist[0].data

# converts 16 bit values to 8 bit values
# for n_y, value in enumerate(a_image_data):
#     # print(n_y)
#     # print(a_image_data[n_y])
#     for n_x, value in enumerate(a_image_data[n_y]): 
#         n_val = a_image_data[n_y][n_x]
#         print(n_val)
#         a_image_data[n_y][n_x] = (n_val / pow(2,16)) * pow(2,8)

# converts 16 bit values to 8 bit values
# a_image_data = a_image_data.astype('uint8')
# a_image_data = a_image_data * a_image_data


# a_image_data = a_image_data / pow(2,16)
a_image_data = a_image_data / pow(2,5)
# a_image_data = a_image_data / pow(2,32)


# cv2 needs float32
# a_image_data = a_image_data.astype(np.float32)
# cv2.imshow('',a_image_data)

# for cell in np.nditer(a_image_data, op_flags=['readwrite']):
#     cell =cell*10
# a_image_data = pow(a_image_data, 4)
# a_image_data = np.power(a_image_data, 10)

# rgb is from last arg to first merge(b,g,r) 
new_img = cv2.merge(
    (a_image_data, # b
    a_image_data, # g
    a_image_data) # r 
)

n_width = new_img.shape[0]
n_height = new_img.shape[1]

n_down_size_factor = 0.0015

new_img = cv2.resize(new_img, (int(n_width*n_down_size_factor), int(n_height*n_down_size_factor)), 0.1, 0.1)
cv2.imwrite(s_path_file_name_jpg, new_img)

cv2.waitKey(0)


cv2.waitKey(0)
# im.convert("RGB")
# im.save("your_file.jpeg")

# print(a_image_data)
# print(o_hdulist)

# o_image = Image.open(o_hdulist)
# print(o_image)

# o_image.save("test.jpg")