import os
import string
import Image
import ImageOps

GenFeature = { 'Orig' : lambda file, label: GenOrig(file, label),
               'Equal' : lambda file, label: GenEqual(file, label),
              }
#set feature type
featureType = 'Orig'

def GenerateDir(dir):
    try:
        label = int(dir)
    except ValueError:
        return

    files = os.listdir(dir)
    for file in files:
            GenFeature[featureType](os.path.join(dir,file), label)


def GenOrig(file, label):
    ''' Generate original pixal feature'''

    noext, ext = os.path.splitext(file)
    if(ext == '.png'):
        im = Image.open(file)
        im = im.convert('L')
        data = im.getdata()
        for pixel in data:
            outputFile.write(str(pixel)+' ')
        outputFile.write(str(label)+ '\n')

def GenEqual(file, label):
    '''
	Generate Equalization feature
	There is a bug: when the total pixel count less than 255,ImageOps.equalize take no effect
	'''

    noext, ext = os.path.splitext(file)
    if(ext == '.png'):
        im = Image.open(file)
        im = im.convert('L')
        im = ImageOps.equalize(im)
        data = im.getdata()
        for pixel in data:
            outputFile.write(str(pixel)+' ')
        outputFile.write(str(label)+ '\n')




#call folder browser dialog to choose a folder. but too ugly
#folder = tkFileDialog.askdirectory(root = None, initialdir = os.path.dirname(__file__))

#change work directory to the directory which contain this source file
os.chdir(os.path.dirname(__file__))

#list all file in the work directory
subdirs = os.listdir(os.getcwd())

outputFile = file(os.path.join(os.getcwd(), 'total.datset'), 'w')

for subdir in subdirs:
    if os.path.isdir(subdir):
        GenerateDir(subdir)

outputFile.close()




