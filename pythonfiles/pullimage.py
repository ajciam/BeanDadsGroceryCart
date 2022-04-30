from PIL import Image
import requests

# url = 'https://solidstarts.com/wp-content/uploads/Green-Bean-scaled.jpg'
url = 'http://192.168.86.33/cam-hi.jpg'
im = Image.open(requests.get(url, stream=True).raw)
im.show()