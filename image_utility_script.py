#!/usr/bin/python
'''----------image_utility_script Version 3.1----------
contains improved naming
fixes bugs found running it on nemesis
all filters are recognized and processed correctly
gamma stretch is disabled - difficult to find good value for the moon
creates thumbnails with ending '_thumb' with size of 600x600 pixel
'''

#packages and modules
import os
import sys
import math
import numpy as np
from astropy.io import fits
from PIL import Image

#whoami
import getpass

'''----------defined constants
sys.argv[1] : given path to image
bp : percentage to set blackpoint
pw : percentage to set whitepoint
nomtf : list with elements that do not need to be stretched by mtf
tm : target median
'''

'''----------functions----------'''

'''
cast2f() - cast np.ndarray from uint16 to float64
@param np array with uint16 data

@return np array with float64 data
'''
def cast2f(image_data):
    image_data=image_data.astype(np.float64)
    return image_data

'''
rotate() - reads telescope orientation from header and rotates image if necessary
updates header and writes corrections as HISTORY keyword
hdulist[0].data and hdulist[0].header get updated

@param hdulist object

@return None
'''
def rotate(hdulist):
    image_header=hdulist[0].header
    image_data=hdulist[0].data

    try:
        if image_header['PIERSIDE']=='WEST':
            print('pierside west - no rotation has to be performed')
        elif image_header['PIERSIDE']=='EAST':
            print('perside east - rotation has to be performed')
        #rotate image
            image_data=image_data[::-1,::-1]
        #change pierside in header
            image_header['PIERSIDE']=('WEST','Changed from EAST by image_utility_script')
        #write history entry
            image_header['HISTORY']='Image orientation changed by image_utility_script'
        else:
            print('unknown pierside - proceed with script')

    except KeyError:
        print('keyword PIERSIDE not present - proceed with script')

    #update hdulist
    hdulist[0].header=image_header
    hdulist[0].data=image_data



'''
normalize() - normalizes image to values between 0 and 1

@param np.ndarray

@return np.ndarray
'''
def normalize(image_data):
    print('normalize image')
    
    maxval=np.amax(image_data)
    image_data=image_data/maxval

    return image_data

'''
mkhist() - makes histogram from an image

@param np.ndarray

@return np.ndarray
'''
def mkhist(image_data):
    print('make histogram')

    histogram=np.histogram(image_data,bins=10000, range=(0,1))

    return histogram

'''
setbp() - calls searchbp() and subtracts the blackpoint from the image

@param np.ndarray, float

@return np.ndarray
'''
def setbp(image_data,p):
    
    bp=searchbp(image_data,p) #search the blackpoint

    print('blackpoint is:'+str(bp))

    print('subtract blackpoint')

    image_data[image_data<=bp]=0
    image_data[image_data>bp]-=bp
    
    return image_data

'''
searchbp() - searches the blackpoint for a given percentage of pixels 
that will be set to value 0

@param np.ndarray, float

@return float
'''
def searchbp(image_data,p):
    histogram=mkhist(image_data)
    print('search blackpoint')

    s=image_data.shape
    n=s[0]*s[1] #number of pixels

    c,i=0,0
    while c<p*n:
        c+=histogram[0][i]
        i+=1

    return histogram[1][i]

'''
setwp() - calls searchwp() and sets all pixelvalues
beond the whitepoint to max value of 1

@param np.ndarray, float

@return np.ndarray
'''
def setwp(image_data,p):

    wp=searchwp(image_data,p)

    print('whitepoint is:'+str(wp))

    print('set white point')
    image_data[image_data>=wp]=1

    return image_data

'''
searchwp() - searches whitepoint for given percentage of pixels
that will be set to value 1

@param np.ndarray, float

@return float
'''
def searchwp(image_data,p):

    histogram=mkhist(image_data)

    print('search white point')

    s=image_data.shape
    n=s[0]*s[1] #number of pixels

    c,i=0,len(histogram[0])
    while c<p*n:
        c+=histogram[0][i-1]
        i-=1

    return histogram[1][i]

'''
findm() - calculates correct midtones balance parameter (m) to get
a targetmedian (tm) after applying midtones transfer function to image data

@param np.ndarray, float

@return float
'''
def findm(image_data,tm):

    cm=np.median(image_data) #current median

    m=cm*(tm-1)/(2*cm*tm-tm-cm)

    print('m is:'+str(m))

    return m

'''
mtf() - apply midtones transferfunction with given midtones balance parameter (m)

@param np.ndarray, float

@return np.ndarray
'''
def mtf(image_data,m):

    a=np.median(image_data)
    print('median is:'+str(a))

    print('do midtonestransfer')
    image_data=image_data=(m-1)*image_data/((2*m-1)*image_data-m)
    
    a=np.median(image_data)
    print('medain after mtf is:'+str(a))
    
    return image_data

'''
cast2uint8() - casts normalized np.ndarray to uint8 np.ndarray

@param np.ndarray

@return np.ndarray
'''
def cast2uint8(image_data):
    image_data=((2**8-1)*image_data)

    image_data =image_data.astype(np.uint8)

    return image_data

'''
savejpg() - saves given np.ndarray as a jpg under same name as fits

@param np.ndarray, Path object

@return None
'''
def savejpg(image_data, l):
    
    image_data=cast2uint8(image_data)

    print('save jpg')

    image=Image.fromarray(image_data,'L')
    image.save(l.path+l.name+l.jpgextension,format='JPEG')

'''
savethumb() - saves given image as jpg with given size of nxn pixel

@param np.ndarray, Path object, size

@return None
'''
def savethumb(image_data, l, size):

    image_data=cast2uint8(image_data)

    print('save thumb')

    image=Image.fromarray(image_data,'L')
    image.thumbnail(size)
    image.save(l.path+l.name+'_thumb'+l.jpgextension,format='JPEG')


'''
gammacorr() - makes a gammacorrection on given image

@param np.ndarray, float

@return np.ndarray
'''
def gammacorr(image_data,gamma):
    image_data=(image_data)**gamma

    return image_data

'''
searchgamma() - searches gamma for a given target median (tm)

@param np.ndarray, float

@return float
'''
def searchgamma(image_data, tm):
    cm=np.median(image_data) #current median

    gamma=math.log(tm)/math.log(cm)
    print('calculated gamma is:'+str(gamma))

    return gamma

'''
name() - creates filename from standard:
DATE-TIME_TARGET_FILTER_EXPOSURETIME_NAME
stores name in l.name

@param image_header, path object

@None
'''
def name(image_header, l):

    try:
        date=image_header['DATE-OBS']

        date=date.replace(':','-')
        date=date.replace('/','-')

    except KeyError:
        date='DATE-TIME'


    try:
        target=image_header['OBJECT']

        target=target.replace(' ','')
        target=target.replace('_','')
        target=target.replace('-','')
        target=target.replace('/','')
        target=target.replace('.','')

        #limit target to 20 char.
        target=target[0:20]

        if target=='':
            target='TARGET'

    except KeyError:
        target='TARGET'
    
    try:
        filt=image_header['FILTER']

        filt=filt.replace('H-alpha 3.5 nm','Halpha')
        filt=filt.replace(' ','')
        filt=filt.replace('_','')
        filt=filt.replace('-','')
        filt=filt.replace('.','-')
        

    except KeyError:
        filt='FILTER'

    try:
        exptime=image_header['EXPTIME']

        if exptime>1:
            exptime=str(int(exptime))+'s'
        else:
            exptime=str(int(exptime*1000))+'ms'

    except KeyError:
        exptime='EXPTIME'

    try:
        usrname=image_header['USER']
        usrname=usrname.split(' ')
        firstname=usrname[0]
        lastname=usrname[1]

        #truncate firstname to max 20 char.
        usrname=firstname[0:20]+'-'+lastname[0:1]

    except KeyError:
        usrname='USERNAME'

    #assemble filename
    name=date+'_'+target+'_'+filt+'_'+exptime+'_'+usrname
    l.name=name

    json  = '{ "exists" : '+l.exists+', "date":"'+date+'", "target": "'+target+'", "filter": "'+filt+'", "exposureTime": "'+exptime+'", "username": "'+usrname+'", "filename" : "'+name+'", "thumbnailname" : "'+name+'_'+l.thumb+'", "extensionFits": "'+l.extension+'","extensionJpg": "'+l.jpgextension+'","execuser": "'+l.execusr+'"  }'
    l.json=json

'''
exists() - checks if jpgs and thumbs of a file already exists and return the json

@param path obj, hdulist

@return None
'''
def exists(l, hdulist):
    
    image_header=hdulist[0].header
    
    name(image_header,l)
    
    fitsexists=os.path.isfile(l.path+l.name+l.extension)
    jpgexists=os.path.isfile(l.path+l.name+l.jpgextension)
    jpgthumbexists=os.path.isfile(l.path+l.name+'_'+l.thumb+l.jpgextension)
    
    if fitsexists and jpgexists and jpgthumbexists:
        l.exists='true'
        name(image_header,l)
        print('return-6cc934e7-56ec-4799-94c2-d9f656285b65:'+l.json)
        sys.exit()
    
    



'''---------Class----------'''

'''
class Path - class to store filepath as an object containing of 
path, filename and extension.
I assume that filepaths begin with '/' for absolute and '.' 
for relativ paths or direct with filenames



'''
class Path:
    def __init__(self,path):
        filepath=path.split('/')

        self.json=''

        self.jpgextension='.jpg'
        
        self.thumb='thumb'
        
        self.exists='false'

        if path[0]=='/': #working with absolute path

            self.path='/'.join(filepath[:-1])+'/'

            self.file='.'.join(filepath[len(filepath)-1].split('.')[:-1])

            self.extension='.'+filepath[len(filepath)-1].split('.')[-1]

        elif path[0]=='.': #working with relativ path

            self.path='/'.join(filepath[:-1])+'/'

            self.file='.'.join(filepath[len(filepath)-1].split('.')[:-1])

            self.extension='.'+filepath[len(filepath)-1].split('.')[-1]

        else: #working in same directory as image (no path necessary)
            self.path=''

            self.file='.'.join(filepath[len(filepath)-1].split('.')[:-1])
            
            self.extension='.'+filepath[len(filepath)-1].split('.')[-1]

    #check if disassembly was successful
        if os.path.isfile(self.path+self.file+self.extension):
            print('path disassembly successful')
            print(self.path+self.file+self.extension)
        else:
            print('path could not be disassembled')
            sys.exit()

        self.name=''

   #whoami
        self.execusr=getpass.getuser()


'''---------main program---------'''

#store filepath as object
l=Path(sys.argv[1])

#open fits
try:
    hdulist=fits.open(sys.argv[1])

except OSError:
    print('given filepath does not exist or file is not a fits')
    sys.exit()

except IndexError:
    print('no path was given, sys.argv[1] is empty')
    sys.exit()

#check if file exists
#exists(l,hdulist)

#rotate image
rotate(hdulist)

#change software owner to StellariumGornergrat
try:
    hdulist[0].header['SWOWNER']=('StellariumGornergrat','changed by image_utility_script')
except KeyError:
    print('Keyword SWOWNER not found and therefore no changes made - proceed with script')

#save header and data changes on original fits file
try:
    hdulist.writeto(sys.argv[1],overwrite=True)

except OSError:
    print('writing fits failed')
    sys.exit()

#store image and header in seperate variables and close fits
image_data=hdulist[0].data
image_header=hdulist[0].header

hdulist.close()

#bugfix: check wether image has only value zero - abort do not 
if np.amax(image_data)==0:

    print('image is completely black (all values 0) - therefore no stretch made. Continue by renaming and saving jpg')
        #create new name
    name(image_header,l)

    #save the image as jpg
    savejpg(image_data,l)
    print('jpg saved')

    #save image as thumbnail (300px jpg)
    size=600, 600 #image size
    savethumb(image_data,l,size)
    print('thumbnail saved')

    #rename original fits
    os.rename(l.path+l.file+l.extension,l.path+l.name+l.extension)
    print('fits renamed')

    print('return-6cc934e7-56ec-4799-94c2-d9f656285b65:'+l.json)

    print('done')

    sys.exit()


#cast np.ndarray image_data to float64
image_data=cast2f(image_data)

#normalize image
image_data=normalize(image_data)

#set blackpoint
pb=0.02
image_data=setbp(image_data,pb)

#set whitepoint
pw=0.000001
image_data=setwp(image_data,pw)

#normalize image
image_data=normalize(image_data)

'''following objects do not need to be stretched with midtones transferfunction'''
nomtf=['MOON','Jupiter','Saturn','Mars']

#check if object is in header, else set mtf as standard correction
try:
    a=image_header['OBJECT'] in nomtf

except KeyError:
    a=False


if a:
    #do gammacorrection

    #find gamma
    tm=0.02
    gamma=searchgamma(image_data,tm)

    #perform gammacorrection
    #image_data=gammacorr(image_data,gamma)

else:
    #do mtf

    #find midtones balance parameter
    tm=0.2
    m=findm(image_data,tm)

    #perform mtf
    image_data=mtf(image_data,m)


#create new name
name(image_header,l)

#save the image as jpg
savejpg(image_data,l)
print('jpg saved')

#save image as thumbnail (300px jpg)
size=600, 600 #image size
savethumb(image_data,l,size)
print('thumbnail saved')

#rename original fits
os.rename(l.path+l.file+l.extension,l.path+l.name+l.extension)
print('fits renamed')

print('return-6cc934e7-56ec-4799-94c2-d9f656285b65:'+l.json)

print('done')
