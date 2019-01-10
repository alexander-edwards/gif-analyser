import os
from PIL import Image
import glob

"""
Has access to list of most common words
for each word in lists searches dictionary 
opens first element
saves gif into gif folder, calling it nameToneCharacter
makes a new folder in imgFolder of nameToneChar
creates 1, 2, and 3 folders in that folder
loads gifs into 1, 2, 3
C++ program alters files and loads info into new folder RSV info with name nameToneChar

"""

def loadingAllGifs(gifFolder) : 
    for filename in glob.glob('%s/*.gif' % gifFolder): #assuming gif
        folderCreation(filename[40:])

def folderCreation(nameToneChar):

    if (os.path.isdir("/Users/alexedwards/CLionProjects/Characters2/cmake-build-debug/Characters/%s/1")):
        print("exists")
        return
    if (not os.path.isdir("/Users/alexedwards/CLionProjects/Characters2/cmake-build-debug/Characters/%s/1")):
        os.makedirs("/Users/alexedwards/CLionProjects/Characters2/cmake-build-debug/Characters/%s/1" % (nameToneChar))
        os.makedirs("/Users/alexedwards/CLionProjects/Characters2/cmake-build-debug/Characters/%s/2" % (nameToneChar))
        os.makedirs("/Users/alexedwards/CLionProjects/Characters2/cmake-build-debug/Characters/%s/3" % (nameToneChar))
        extractFrames("/Users/alexedwards/Desktop/images-large/%s" % (nameToneChar),
        "/Users/alexedwards/CLionProjects/Characters2/cmake-build-debug/Characters/%s" % (nameToneChar))


def extractFrames(inGif, outFolder):
    print(inGif)
    frame = Image.open(inGif)
    nframes = 0
    while frame:

        #The Glob() function from openCV reads in alphanumeric order.
        #It might read 10 as less than 1. 
        #So it is just easier to have them in two separate folders. 
        if nframes < 10:
            
            frame.save( '%s/1/%s-%s.png' % (outFolder, os.path.basename(inGif), nframes ))
            nframes += 1
            print (nframes)

        if (nframes >= 10 and nframes < 100): 
            frame.save( '%s/2/%s-%s.png' % (outFolder, os.path.basename(inGif), nframes ))
            nframes += 1
            print (nframes)

        if(nframes >= 100 and nframes < 1000):
            frame.save( '%s/3/%s-%s.png' % (outFolder, os.path.basename(inGif), nframes ))
            nframes += 1
        
        if (nframes > 1000):
            print("Too many frames")
        try:
            frame.seek( nframes )
        except EOFError:
            break
    return True

loadingAllGifs("/Users/alexedwards/Desktop/images-large")