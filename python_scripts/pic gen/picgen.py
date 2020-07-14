print("Thermal Image Generator")
from PIL import Image, ImageDraw, ImageFont, ImageFilter
import random


#------------------------setup------------------------------------------------------------
# input array file data size: 60*80
resx=60
resy=80

# picture scale up factor
scale=10 

# size of the lable box 
lablesize=40

# display thewshold (high&low)
thresholdh=95
thresholdl=85

# alarm thresholdh
alarm=90

# data value resolution
velres=1

# if one pixel is labled then all the pixels within the range of overlap area will not be labled as a seprate spot.
overlap=5 #min of 2
#----------------------------------------------------------------------------------------


#------------------functions--------------------------------------------------------------
# function of lableing: draw a box around the target pixel 
def lable(location):
  # id the x and y location of the target pixel
  locmaxx=(int(location/resy)*+1)*scale+int(scale/2)
  locmaxy=location%resy+int(scale/2)
  
  # paint the target pixel as red
  pixels[locmaxy, locmaxx] = (252, 3, 3)
  # define the box's helf length
  halflength=int(lablesize/2)
  
  # if the box are drawed out of the image, display error
  if locmaxy>resy-halflength or locmaxy<halflength or locmaxx>resx-halflength or locmaxx<halflength:
    print("out of view")

  # draw the box
  for line in range(halflength):
    if locmaxy-line<resy-1 and locmaxx-halflength<resx-1:
      pixels[locmaxy-line, locmaxx-halflength] = (252, 3, 3)
    if locmaxy+line<resy-1 and locmaxx-halflength<resx-1:
      pixels[locmaxy+line, locmaxx-halflength] = (252, 3, 3)
    if locmaxy-line<resy-1 and locmaxx+halflength<resx-1:
      pixels[locmaxy-line, locmaxx+halflength] = (252, 3, 3)
    if locmaxy+line<resy-1 and locmaxx+halflength<resx-1:
      pixels[locmaxy+line, locmaxx+halflength] = (252, 3, 3)
    if locmaxy+halflength<resy-1 and locmaxx+line<resx-1:
      pixels[locmaxy+halflength, locmaxx+line] = (252, 3, 3)
    if locmaxy-halflength<resy-1 and locmaxx+line<resx-1:
      pixels[locmaxy-halflength, locmaxx+line] = (252, 3, 3)
    if locmaxy+halflength<resy-1 and locmaxx-line<resx-1:
      pixels[locmaxy+halflength, locmaxx-line] = (252, 3, 3)
    if locmaxy-halflength<resy-1 and locmaxx-line<resx-1:
      pixels[locmaxy-halflength, locmaxx-line] = (252, 3, 3)

  return None

# find all the pixels around the target pixeal within the range of overlap, collected in list ran
def rangeit(vel):
  ran=[]
  for i in range(overlap):
    ran.extend(range(vel-i*resy-overlap,vel-i*resy+overlap+1,1))
    ran.extend(range(vel+i*resy-overlap,vel+i*resy+overlap+1,1))

  return ran
#---------------------------------------------------------------------------------------------


#----------------------------main codes------------------------------------------------------
# crate and save a image to work with 
img = Image.new('RGB', (resy, resx), color = (255,255,255))
img.save("image.png")

# open the crated image
im = Image.open("image.png")
pixels = im.load()

# define a list that will contain all the array data from the file
allv=[]

# open the data array file
data = open("data.txt")

# read the value and convert the value to gray scale rgb number
for x, line in enumerate(data):
  ylist=line.split()
  for y in range(resy):
    val=int(ylist[y])
    # store the value in allv list
    allv.append(val)
    pixels[y, x] = (val, val, val) #grayscale
    
data.close()


# store the maximum value from the list
globhigh="Golb Maximum Temperature: "+str(max(allv))

# define a list that stores all the value within the threshold
lablelist=[]

# ran down from high threshold to low threshold and find if any value matchs the data file value
for labn in range(thresholdh, thresholdl-velres,-velres):
  if labn in allv:
    for i in range(len(allv)):
      if allv[i]==labn:
        lablelist.append(i)
        # if find the value above the alarm threshold, dislay the alarm massage
        if labn>=alarm:
          outputalarm="\n!!!HIGH VALUE ALARM!!!"

# define a list that will contain all the pixels should not be labled 
nonlable=[]

# from the highest value, include the pixels around it's overlap range into the non-lable list, then run down the lable list. if any pixel is in the non-lable list, replace it with none.
for i in range(len(lablelist)):
  if lablelist[i] not in nonlable:
    nonlable.extend(rangeit(lablelist[i]))
  else:
    lablelist[i]=None

# get rid of all the none from the lable list
lablelist=list(filter(None, lablelist))

# store the analysis infomation
localhigh="\nMaximum Temperature under threshold: "+str(allv[lablelist[1]])
thr="\nDisplay Threshold: "+str(thresholdl)+" to "+str(thresholdh)
ala="\nAlarm Level: "+str(alarm)

# scale up the image
resx=resx*scale
resy=resy*scale
im=im.resize((resy,resx), Image.ANTIALIAS)

# update the locations of pixels in the lable list for the scaled image
lablelist = [i * scale for i in lablelist]
pixels = im.load()
draw=ImageDraw.Draw(im)

# lable all the pixels within the lable list and display text
for i in range(len(lablelist)):
  lable(lablelist[i])
  display="Local Maximum of \n "+str(allv[int(lablelist[i]/scale)])
  draw.text((lablelist[i]%resy,(lablelist[i]/resy)*scale+lablesize), display, fill=(0,0,255))

# crate a backgroun image that overlays the working image
background= Image.new('RGB', (resy, resx+80), color = (255,255,255))
background.paste(im)

# make the display massage block and place it on the backgraound
output=globhigh+localhigh+thr+ala+outputalarm
drawlab=ImageDraw.Draw(background)
drawlab.text((20,resx), output, fill=(252,3,3))

# save image
background.save("image.png")

background.show()
