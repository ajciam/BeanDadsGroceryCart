from pyzbar import pyzbar # Barcode module
import cv2


def read_barcodes(frame):
    barcodes = pyzbar.decode(frame)
    for barcode in barcodes:
        x, y , w, h = barcode.rect
        
        barcode_info = barcode.data.decode('utf-8')
        cv2.rectangle(frame, (x, y),(x+w, y+h), (0, 255, 0), 2) # Draw rectangle around barcode
        
        
        font = cv2.FONT_HERSHEY_PLAIN
        cv2.putText(frame, barcode_info, (x + 6, y - 6), font, 2.0, (255, 255, 255), 1) # Add text representing barcode info
        
        with open("barcode_result.txt", mode ='w') as file:
            file.write("Recognized Barcode:" + barcode_info) # Output barcode info to text file
    return frame

def main():
    camera = cv2.VideoCapture(0) # Turn on camera
    ret, frame = camera.read()

    while ret: # Run loop until 'ESC' key is pressed
        ret, frame = camera.read()
        frame = read_barcodes(frame)
        cv2.imshow('Barcode/QR code reader', frame)
        if cv2.waitKey(1) & 0xFF == 27:
            break

    camera.release() # Close the application window
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()