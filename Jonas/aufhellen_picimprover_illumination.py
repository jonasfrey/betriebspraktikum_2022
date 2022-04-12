from re import A
import numpy as np
from PIL import Image
from astropy.io import fits as fritz
import cv2 
import sys
import time
# call it 
# python3 convert_fts_to_jpg.py 2021-07-02T00-57-52_M17_Open_280s_Jonas-F.fts

# s_path_file_name = sys.argv[1]
s_path_file_name = "./../2019-10-26T03-26-55_M51_Clear_280s.fts"
a_s_parts = s_path_file_name.split(".")
a_s_parts.pop()
a_s_parts.append(str(int(time.time())))
a_s_parts.append("png")

s_path_file_name_jpg = ".".join(a_s_parts)

o_hdulist = fritz.open(s_path_file_name)
a_image_data = o_hdulist[0].data

n_height = a_image_data.shape[0]
n_width = a_image_data.shape[1]

print("n_height: " +str(n_height))
print("n_width: " +str(n_width))

n_bits_per_pixel = 16

# normalize 

n_pixel_value_max = np.amax(a_image_data)

print("n_pixel_value_max:"+str(n_pixel_value_max))
# a_image_data = a_image_data / pow(2,n_bits_per_pixel)
a_image_data_normalized = a_image_data / n_pixel_value_max

# # black point abziehen
# n_black_point_normalized = 0.050
# n_white_point_normalized = 0.095
# a_image_data = a_image_data - n_black_point_normalized
# a_image_data_normalized = a_image_data_normalized / n_white_point_normalized

# mid tone function

n_mtf_value = (1/2)

# a_image_data_normalized = ((n_mtf_value-1)*a_image_data) / ((2*n_mtf_value-1)*a_image_data - n_mtf_value)
a_image_data_normalized = a_image_data_normalized+a_image_data_normalized

# def f_mtf(n_normalized, n_mtf):
#     return ((n_mtf-1)*n_normalized) / ((2*n_mtf-1)*n_normalized - n_mtf)

# for n_y in range(0, n_height):
#     n_x = 0
#     while n_x < n_height:
#         n_pixel_value = a_image_data_normalized[n_y][n_x]
#         a_image_data_normalized[n_y][n_x] = f_mtf(
#             n_pixel_value, 
#             1/2
#         )
        # print("n_pixel_value: "+str(n_pixel_value))

a_image_data_denormalized = a_image_data_normalized * n_pixel_value_max
a_image_data_denormalized = a_image_data_denormalized.astype(np.uint16)

# new_img = cv2.resize(a_image_data_normalized, (int(n_width*n_down_size_factor), int(n_height*n_down_size_factor)), 0.1, 0.1)
cv2.imwrite(s_path_file_name_jpg, a_image_data_denormalized)
# cv2.imwrite(s_path_file_name_jpg, a_image_data)

cv2.waitKey(0)

