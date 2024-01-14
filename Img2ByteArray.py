import os
from PIL import Image
import numpy as np

def vertical1bit(image_path, threshold=128, screenWidth=96, screenHeight=65):
    # Open the image file
    img = Image.open(image_path).convert('L')  # Convert image to grayscale

    # Resize the image to match the screen size
    img = img.resize((screenWidth, screenHeight))

    # Convert the image data to a numpy array and apply threshold
    img_data = np.array(img)
    bit_image = np.where(img_data > threshold, 1, 0)

    output_string = ""
    output_index = 0
    byteArraySize = 0

    for p in range(int(np.ceil(screenHeight / 8))):
        for x in range(screenWidth):
            byteIndex = 7
            number = 0
            for y in range(7, -1, -1):
                row = (p * 8) + y
                if row < screenHeight:
                    number += bit_image[row][x] * 2**byteIndex
                byteIndex -= 1

            byteSet = hex(number)[2:]
            if len(byteSet) == 1:
                byteSet = "0" + byteSet

            b = "0x" + byteSet
            output_string += b + ", "
            output_index += 1

            if output_index >= 16:
                output_string += "\n"
                output_index = 0

            byteArraySize += 1

    return output_string

# Usage:
directory = 'images'
output_file = 'output.h'
num_files = len([name for name in os.listdir(directory) if os.path.isfile(os.path.join(directory, name))])
print('Found ' + str(num_files) + ' files in ' + directory)
with open(output_file, 'w') as f:
    f.write('unsigned char bytearr[][864] = {\n')

    # Get all .png files
    filenames = [name for name in os.listdir(directory) if name.endswith('.png')]

    # Sort filenames by the numeric part
    filenames.sort(key=lambda name: int(name.split('_')[1].split('.')[0]))

    for filename in filenames:
        result = vertical1bit(os.path.join(directory, filename))
        f.write('\t{' + result + '},\n')

    f.write('};\n')