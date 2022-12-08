import pytesseract as tess
from PIL import Image, ImageEnhance, ImageFilter

# Path to tesseract executable

img = Image.open('text.png')
text = tess.image_to_string(img)
print('<HTML><BODY>')
print('<H1>Text from image</H1>')
print('<P>')
print(text)
print('</P>')
print('</BODY></HTML>')
