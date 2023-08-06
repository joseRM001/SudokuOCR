import numpy as np
import cv2 as cv

img = cv.imread('sudoku2.png')
assert img is not None, "file coud not be read"

gray_img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
binary = cv.adaptiveThreshold(gray_img, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 11, 2)
#morph_img = cv.erode(binary, cv.getStructuringElement(cv.MORPH_RECT, (3,3)), iterations=1)

#### converto to cv.CV_8U
#abs_edges = np.absolute(edges)
#edges = np.uint8(abs_edges)
####

#### CONTOURS
cnts, hier = cv.findContours(binary, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
x, y, w, h = cv.boundingRect(cnts[1])
####
#cv.drawContours(img, cnts, 2, (36, 255, 12), 2)

# CROP RGB IMG AND BINARY
img = img[y:y+h, x:x+w]
binary = binary[y:y+h, x:x+w]

#### EDGES
edges = cv.Laplacian(binary, cv.CV_8UC1)
####

#### HOUGH 
lines = cv.HoughLines(edges, 1, np.pi/180, 200)

for line in lines:
    rho, theta = line[0]
    a = np.cos(theta)
    b = np.sin(theta)
    x0 = a*rho
    y0 = b*rho
    x1 = int(x0 + 1000*(-b))
    y1 = int(y0 + 1000*(a))
    x2 = int(x0 - 1000*(-b))
    y2 = int(y0 - 1000*(a))
    if theta == 0:
        cv.line(img, (x1,y1), (x2,y2),(0,0,255),2)
    else:
        cv.line(img, (x1,y1), (x2,y2),(0,255,0),2)

####

red = np.array([0, 0, 255])
green = np.array([0, 255, 0])
gx0, gy0, w = 0,0, 0

for i, j in np.ndindex(img.shape[:-1]):
    if np.array_equal(green, img[i,j]):
        gx0, gy0 = i, j
        break

for i in range(gx0, img.shape[0]):
    for j in range(gy0, img.shape[1]):
        w += 1
        if np.array_equal(red, img[i,j]):        
            break
    break

print(w)

cv.line(img, (gx0, gy0), (gx0 + w, gy0), (255, 0, 0), 4)
cv.line(img, (gx0+w, gy0), (gx0 + w, gy0 + w), (255, 0, 0), 4)
cv.line(img, (gx0, gy0), (gx0, gy0 + w), (255, 0, 0), 4)
cv.line(img, (gx0, gy0 + w), (gx0 + w, gy0 + w), (255, 0, 0), 4)


# min_area = 1400
# max_area = 2000
# n = 0
# for c in cnts:
#     area = cv.contourArea(c)
#     if area > min_area and area < max_area:
#         x, y, w, h = cv.boundingRect(c)
#         cv.rectangle(img, (x,y), (x + w, y + h), (36, 255, 12), 2)
#         cell = img[y:y+h, x:x+w]
#         cv.imwrite('imgs/Cell_{}.png'.format(n), cell)
#         n += 1



#imshow
cv.imshow('img', img)
cv.waitKey(0)
cv.destroyAllWindows()