import numpy as np
from PIL import Image
from astropy.io import fits as fritz
import cv2 
import sys
import time
# call it 
# python3 convert_fts_to_jpg.py 2021-07-02T00-57-52_M17_Open_280s_Jonas-F.fts

# s_path_file_name = sys.argv[1]
# s_path_file_name = "./../2019-10-26T03-26-55_M51_Clear_280s.fts"
s_path_file_name = "./2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W_debayered.jpg__default_cv2.jpg"
a_s_parts = s_path_file_name.split(".")
a_s_parts.pop()
a_s_parts.append(str(int(time.time())))

s_path_file_name_output = ".".join(a_s_parts)

if(s_path_file_name.split(".").pop() == "fts"):

    o_hdulist = fritz.open(s_path_file_name)
    a_image_data = o_hdulist[0].data
else:
    a_image_data = cv2.imread(s_path_file_name)
    a_image_data = ((a_image_data / 255) * (pow(2,16)-1)).astype(np.uint16)

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

# black point abziehen
n_black_point_normalized = 0.050
n_white_point_normalized = 0.095
a_image_data = a_image_data - n_black_point_normalized

# a_image_data_normalized = a_image_data_normalized[x<=] / n_white_point_normalized

a_image_data = np.where(a_image_data > 0.05, a_image_data/0.095, a_image_data)
# mid tone function

n_mtf_value = (1/2)
n_mtf_value = float(sys.argv[1])

# mid tone function mtf
a_image_data_normalized = ((n_mtf_value-1)*a_image_data_normalized) / ((2*n_mtf_value-1)*a_image_data_normalized - n_mtf_value)

# a_image_data_denormalized = a_image_data_normalized * n_pixel_value_max
# a_image_data_denormalized = a_image_data_normalized * pow(2,n_bits_per_pixel)
# a_image_data_denormalized = a_image_data_denormalized.astype(np.uint16)
a_image_data_denormalized = a_image_data_normalized * (pow(2,8)-1)
a_image_data_denormalized = a_image_data_denormalized.astype(np.uint8)

# new_img = cv2.resize(a_image_data_normalized, (int(n_width*n_down_size_factor), int(n_height*n_down_size_factor)), 0.1, 0.1)
cv2.imwrite(s_path_file_name_output+"_cv2_imwrite_n_mtf_value_"+str(n_mtf_value)+".jpg", a_image_data_denormalized)
# image=Image.fromarray(a_image_data_denormalized,'L')
# image.save(s_path_file_name_output+"_PIL_save_n_mtf_value_"+str(n_mtf_value)+".jpg",format='JPEG')
# cv2.imwrite(s_path_file_name_jpg, a_image_data)
