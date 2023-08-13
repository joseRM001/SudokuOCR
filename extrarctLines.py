import numpy as np
import cv2 as cv

def show_img(str, img):
    cv.imshow(str, img)
    cv.waitKey(0)
    cv.destroyAllWindows()



img = cv.imread('sudoku.png', cv.IMREAD_COLOR)
assert img is not None, "file could not be read"



########## TRANSFORM TO GRAY SCALE 
if len(img.shape) != 2:
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
else:
    gray = img


########## BINARIZE
gray = cv.bitwise_not(gray)
bw = cv.adaptiveThreshold(gray, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY, 15, -2)
#show_img("bw", bw)


horizontal = np.copy(bw)
vertical = np.copy(bw)

cols = horizontal.shape[1]
horizontal_size = cols // 10

horizontal_SR = cv.getStructuringElement(cv.MORPH_RECT, (horizontal_size, 1))

horizontal = cv.erode(horizontal, horizontal_SR)
horizontal = cv.dilate(horizontal, horizontal_SR)


rows = horizontal.shape[0]
vertical_size = rows // 10

vertical_SR = cv.getStructuringElement(cv.MORPH_RECT, (1, vertical_size))

vertical = cv.erode(vertical, vertical_SR)
vertical = cv.dilate(vertical, vertical_SR)


grid = cv.add(vertical, horizontal)
contours, hier = cv.findContours(grid, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
#cv.drawContours(img, contours, -1, (36, 255, 12), 3)
x, y, w, h = cv.boundingRect(contours[0])
cv.rectangle(img, (x, y), (x + w, y + h), (36, 255, 12), 2)

###### CROP
bw = bw[y:y+h, x:x+w]
img = img[y:y+h, x:x+w]
grid = grid[y:y+h, x:x+w]

lines = cv.HoughLinesP(grid, 1, np.pi/180, 50, None, 50, 10)

for i in range(0, len(lines)):
    l = lines[i][0]
    cv.line(img, (l[0], l[1]), (l[2], l[3]), (255, 0, 0), 3, cv.LINE_AA)
    cv.circle(img, (l[0], l[1]), 2, (255, 0, 0), 2)
    cv.circle(img, (l[2], l[3]), 2, (255, 0, 0), 2)

######

show_img("img", img)


#result = cv.subtract(bw, result2)
#show_img("morph", result)
#show_img("morph", result2)
