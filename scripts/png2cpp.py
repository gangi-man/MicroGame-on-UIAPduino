import argparse, math, re
from PIL import Image


def genCode(name, width, height, bytes):
    code = "static const uint8_t %s[] = {%d, %d,\n" % (name, width, height)
    for y in range(height):
        x8 = int(math.floor(width/8) + (width/4) % 2)
        for x in range(x8):
            code += " 0x%02x," % bytes[y*x8 +x]
        code += "\n"

    code = re.sub(",$", "};\n", code)
    return code

def openPNG(fname):
    def pixelon(img, x, y):
        pixel = img.getpixel((x, y))
        if (pixel[0] < 128):
            return 1
        else:
            return 0

    img = Image.open(fname)
    (width, height) = img.size
    if ((width % 4) != 0):
        raise("Image width should be 4xn")

    if ((width > 256) or height > 256):
        raise("Image size should be less 256x256")

    bytes = []
    for y in range(height):
        upper = True
        byte = 0
        for x in range(math.floor(width / 4)):
            xp = x*4
            # print("#%d, %d  %d %d %d %d (%d)" % (xp, y, pixelon(img, xp, y), pixelon(img, xp+1, y), pixelon(img, xp+2, y), pixelon(img, xp+3, y), byte))
            byte += 8 * pixelon(img, xp  , y)
            byte += 4 * pixelon(img, xp+1, y)
            byte += 2 * pixelon(img, xp+2, y)
            byte += 1 * pixelon(img, xp+3, y)
            if upper:
                upper = False
                byte = byte << 4
            else:
                upper = True
                bytes.append(byte)
                byte = 0

        if upper == False:
            bytes.append(byte)

    found = re.search(r"([^/]*)\.png$", fname)
    varName = found.group(1)
    return genCode(varName, width, height, bytes)


def main():
    parser= argparse.ArgumentParser()
    parser.add_argument('filenames', nargs="+")
    args = parser.parse_args()

    code = ""
    for fname in sorted(args.filenames):
        code += openPNG(fname)
    print(code)

if __name__ == "__main__":
    main()
    


        
                
            
        
    
    
