import rawpy
import imageio
import cv2 

raw = rawpy.imread('IMG_7710.CR2')
arr = raw.postprocess()

print(arr)