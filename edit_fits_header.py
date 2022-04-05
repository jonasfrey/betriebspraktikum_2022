# Import the astropy fits tools
from ast import Try
from astropy.io import fits
import sys


def f_s_split_at_nth_character(str, n):
    splittedString = [str[i:i+n] for i in range(0, len(str), n)]
    return splittedString

def f_s_get_header_row(header, s_header_prop):
    a_s_rows = f_s_split_at_nth_character(header.tostring(), 80)
    for s_header_row in a_s_rows:
        try:
            if(s_header_row.index(s_header_prop) == 0):
                print(s_header_row)
        except:
            pass 


s_path_file_name = sys.argv[1]

# Open the file header for viewing and load the header
hdulist = fits.open(s_path_file_name)
header = hdulist[0].header


print(header.keys)


if len(sys.argv)<4:
    print('call script like this')
    print('python3 '+__file__+' HEADER_PROPERTY HEADER_VALUE')
    print('for example')
    print('python3 '+__file__+' AUTHOR Max Musterman')
    exit()

# Add a new key to the header
s_header_prop = sys.argv[2]
s_header_value = " ".join(sys.argv[3:])

try:
    print("--- ")
    print("old header: ")
    print("--- ")
    s_header_row = f_s_get_header_row(header, s_header_prop)
    header[s_header_prop] = s_header_value
except:
    header.set(s_header_prop, s_header_value)
    pass

print("--- ")
print("new header: ")
print("--- ")
s_header_row = f_s_get_header_row(header, s_header_prop)
print(s_header_row)

# Save the new file
hdulist.writeto(s_path_file_name, overwrite=True)

# Make sure to close the file
hdulist.close()