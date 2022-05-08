from time import time, sleep
from PIL import Image, ImageDraw, ImageFont
import requests
import shutil
import psutil

''' 
Download an image from a uri. This function is necessary because we are streaming a raw jpeg and therefore need to copy
raw bytes from the uri into a local image file. Otherwise Google Vision will not function. 
'''

def download_image(uri):
    ## Set up the image URL and filename
    filename = uri.split("/")[-1]

    # Open the url image, set stream to True, this will return the stream content.
    r = requests.get(uri, stream = True)

    # Check if the image was retrieved successfully
    if r.status_code == 200:
        # Set decode_content value to True, otherwise the downloaded image file's size will be zero.
        r.raw.decode_content = True
        
        # Open a local file with wb ( write binary ) permission.
        with open(filename,'wb') as f:
            shutil.copyfileobj(r.raw, f)
            
        print('Image sucessfully Downloaded: ', filename)
    else:
        print('Image Couldn\'t be retreived')
    return filename

def hide_images():
    for proc in psutil.process_iter():
        print(proc.name())
        if proc.name() == "display":
            proc.kill()


# Takes in an image path and provides a list of objects. Objects have very general names such as 'food' but have bounding boxes
def detect_objects(filename):
    from google.cloud import vision
    client = vision.ImageAnnotatorClient()

    # Load image into memory
    with open(filename, 'rb') as image_file:
        content = image_file.read()

    image = vision.Image(content=content)

    # Perform object detection on the image file
    objects = client.object_localization(image=image).localized_object_annotations

    # Lines below print object info and should be removed later
    # print('Number of objects found: {}'.format(len(objects)))
    # for object_ in objects:
    #     print('\n{} (confidence: {})'.format(object_.name, object_.score))
    #     print('Normalized bounding polygon vertices: ')
    #     for vertex in object_.bounding_poly.normalized_vertices:
    #         print(' - ({}, {})'.format(vertex.x, vertex.y))
    return objects

# Takes in an image path and provides a list of labels. Labels are more specific than object names but have no bounding boxes.
def detect_labels(filename):
    from google.cloud import vision
    client = vision.ImageAnnotatorClient()

    # Loads the image into memory
    with open(filename, 'rb') as image_file:
        content = image_file.read()

    image = vision.Image(content=content)

    # Performs label detection on the image file
    response = client.label_detection(image=image)
    labels = response.label_annotations

    # Lines below print label info and should be removed later
    # print('Labels:')
    # for label in labels:
    #     print(label.description)
    return labels

# Takes in an image path and a list of objects in that image and draws a bounding box around each object.
def draw_bounding_boxes(filename, objects):
    img = Image.open(filename)
    width, height = img.size
    img_draw = ImageDraw.Draw(img)
    for object in objects:
        vertices = object.bounding_poly.normalized_vertices
        # Vertex coordinates are normalized so we need to multiply by height and width in practice.
        shape = [(vertices[0].x * width, vertices[0].y * height),(vertices[2].x * width, vertices[2].y * height)] # Top left and bottom right vertices
        img_draw.rectangle(shape, fill=None, width = 10, outline="yellow")
        myFont = ImageFont.truetype(font='COMIC.TTF', size=65)
        img_draw.text((vertices[0].x * width, vertices[0].y * height), object.name, font = myFont, fill=(0, 0, 0)) # Label each box
    img.show()

def main():
    # Print objects every 5 seconds
    uri = 'http://10.147.16.136/cam-hi.jpg'
    start = time()
    interval = 5
    while True:
        filename = download_image(uri)
        objects = detect_objects(filename)
        # hide_images()
        draw_bounding_boxes(filename, objects)
        sleep(interval - ((time() - start) % interval))



if __name__ == '__main__':
    main()