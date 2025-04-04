from PIL import Image

img = Image.open("Mona_Lisa_pooled.pgm")
img.save("Mona_Lisa_pooled.jpg", "JPEG")

print("Done")