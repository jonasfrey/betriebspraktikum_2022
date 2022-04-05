import cv2 
import os
import random 
import numpy
import sys

numpy.set_printoptions(threshold=sys.maxsize)

b_os_is_windows = os.name == 'nt'

s_path_file_name = './../2019-10-26T03-26-55_M51_Clear_280s_tiny.jpg'

if(b_os_is_windows):
    s_path_file_name = "\\".join(s_path_file_name.split("/"))

o_img = cv2.imread(s_path_file_name)

with open(__file__.split(".")[0] +'.txt', 'w') as f:
    f.write(str(o_img))


print(o_img)