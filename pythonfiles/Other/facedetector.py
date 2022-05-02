import cv2

def detect(frame, cascades):
    for cascade in cascades:
        objects = cascade.detectMultiScale(frame)
        for (x, y, w, h) in objects:
            frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
    
    return frame


def main():
    cascade = cv2.CascadeClassifier()
    filename = 'haarcascade_frontalface_alt.xml'
    path = "C:\\Users\\madel\\Downloads\\haarcascade_frontalface_alt.xml"
    if not cascade.load(filename):
        print('Error loading cascade')
    
    camera = cv2.VideoCapture(0)
    
    while True:
        ret, frame = camera.read()
        cascades = [cascade]
        frame = detect(frame, cascades)
        cv2.imshow('Capture - Face detection', frame)
        if cv2.waitKey(1) == 27:
            break

if __name__ == '__main__':
    main()

