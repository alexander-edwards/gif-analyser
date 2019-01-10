# gif-analyser

Overview

This program takes a directory of GIFs and transforms each GIF into a series of time, x, y coordinates that describes how to draw each character.

This program uses a database of Chinese character GIFs that show the sequence of the strokes. 

Chinese database: http://208.111.35.22/characters/learn-to-write-chinese/

Files Contained 

listOfCharacters.rtf 
main.cpp
GifConverter.py

/////////////////////////////////////////////////////////////////////////

Instructions: How to Run the Program 

To use this program, you must install OpenCV on your computer, download the database of GIFs, run the python script on the GIFs (changing the paths to those corresponding to your computer, i.e. Users/alexedwards/Desktop/images-large -> Users/jondoe/Desktop/images-large).

Depending on where you set the output path, this will create a series of directories each containing the PNG frames of the GIFs. 

Change the path at line 61 of the cpp file to the path you set for the output of the python script (where all the png directories are kept). 
Change the path at line 121 of cpp file to the corresponding location of the listOfCharacters.rtf file on your machine.    

Instructions: Using the Program 

Running the program will automatically show the display window. On the display window, press any key to begin the first character. Press any key after the character has been written to proceed to a blank window, and press any other key to begin the next character.

Changing the value of speed (line 41) will make the characters be written slower or faster correspondingly. 

Changing the frequency (line 40) will change the number of data points extrapolated. Frequency should be in range 500-6000 Hz (but preferably around 1000 Hz). Max frequency is 6000 Hz.  

Change the sizeFactor (line 40) to change how big the image is. 

/////////////////////////////////////////////////////////////////////////

Problems

The database is slightly faulty. When converted to PNG files, some frames are missing parts of the character. This can only be fixed by getting a more reliable GIF database. This affects approximately 1 in 10 characters. 

The program fills in the spaces between data points. If the distance between two points is greater than the changeValue (line 238), the program will not fill in the datapoint.  Depending on the changeValue (line 238) the space between some points that should not be filled in get filled in anyway (for instance, the space between the end of a stroke and the beginning of a new one). There is currently a decently working fix that alters the changeValue based on the number of data points. This works because the number of data points corresponds with the complexity of the character, which corresponds with how close subsequent strokes are likely to be. However, for approximately 1 in 15 strokes there is still a small stroke that is connected to another. This may be able to be fixed with increased optimization, altering the weights of the change value. 

It was challenging to get the cpp program to work with the GIFs, as OpenCV does not currently have a framework for altering/reading the frames of a GIF. That necessitated the python script. 

If the program was to be rewritten, we would change the input mechanism for both the folder containing all PNG files and the listOfCharacters.rtf. Currently the paths are hard coded into the program, so this would not be an easy fix, but making the paths more versatile by having them be inputs would be nicer and more user-friendly. We would also use a different data-base from the start to get rid of the problems outlined above. Finally we would expand on the optimization of the changeValue to lower the amount of affected characters. 
