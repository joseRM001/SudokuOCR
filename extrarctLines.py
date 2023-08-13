import numpy as np
import cv2 as cv
import math

def show_img(str, img):
    cv.imshow(str, img)
    cv.waitKey(0)
    cv.destroyAllWindows()

# return the centroid of a contour
def get_centeroid(cnt):
    length = len(cnt)
    sum_x = np.sum(cnt[..., 0])
    sum_y = np.sum(cnt[..., 1])
    return int(sum_x / length), int(sum_y / length)


img = cv.imread('sudoku2.png', cv.IMREAD_COLOR)
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
######


contours, hier = cv.findContours(grid, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

count = 0
cnts = []
for i in range(1, len(contours)): # contour[0] is the external contour, we are interested in the internal contours
    cnt = contours[i]
    x, y = get_centeroid(cnt)
    cv.circle(img, (x, y), 10, (0, 0, 255), -1)
    count += 1
    cv.putText(img, str(count), (x-10, y+5), cv.FONT_HERSHEY_COMPLEX, 0.5, (0, 255, 255), 1)
    cnts.append(cv.boundingRect(cnt))
    
#cnts = sorted(cnts, key=lambda data:math.sqrt(data[0]**2 + data[1]**2))
cnts = sorted(cnts, key=lambda data:data[1])
cnts = sorted(cnts, key=lambda data:data[0])

for i in range(0, len(cnts)):
    x, y, w, h = cnts[i]
    cv.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
    show_img("img", img)


# centers = list(get_centers(grid))
# h, w = grid.shape
# count = 0
# for row in get_rows(img, centers, 4, h):
#     cv.polylines(img, [row], False, (255, 0, 255), 2)
#     for x, y in row:
#         count += 1
#         cv.circle(img, (x, y), 10, (0, 0, 255), -1)  
#         cv.putText(img, str(count), (x - 10, y + 5), 1, cv.FONT_HERSHEY_PLAIN, (0, 255, 255), 2)
# 




#result = cv.subtract(bw, result2)
#show_img("morph", result)
#show_img("morph", result2)
