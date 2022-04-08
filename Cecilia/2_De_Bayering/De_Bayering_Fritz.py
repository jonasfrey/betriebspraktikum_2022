
import cv2 
import os
import random 
import numpy
from astropy.io import fits as fritz 

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts'
# s_path_output_file = __file__.split(".")[0] 
s_path_output_file = "2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W_small_debayered.jpg" # static filename
s_path_output_file_suffix = "_default"

if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

# o_img = cv2.imread(s_path_file_name)
o_hdulist = fritz.open(s_path_file_name)

o_img = o_hdulist[0].data
print(o_img)
exit()