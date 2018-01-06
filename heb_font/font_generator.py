import argparse
import StringIO
# import itertools
import os
import os.path

FONT_FORMAT = """/*
 *	Name       {font_name}
 *	Date       {date}
 *	Size       {size}
 *	Height     {max_height}
 *	Width      {max_width}
 *	Sequences  {sequences}
 *	Characters {characters}
 *	Created with font_generator.py written by Arad Eizen 2017, a.e.tec.inc@gmail.com
 */

static const uint8_t {font_name}[] PROGMEM = {
	{height}, // Height
	{sequences_count}, // Sequences count
	{sequences_table}
	{characters_table}
	{characters_data}
};
"""

def convert_image_to_bitmap(image_path, bitmap_path, bitmap_name, convert_function, append=False):
    font_name
    with open(bitmap_path, "w") as bitmap_file:
        bitmap_file.write(FONT_FORMAT.format())


def main(args):

    font_name
    date = time.strftime("%d/%m/%y")
    size
    max_height
    max_width
    sequences
    characters


    if os.path.isfile(args.image_path):
        pass
    elif os.path.isdir(args.image_path):
        image_path, _, image_names = os.walk(args.image_path).next()
        for image_name in image_names:
            pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Convert GLCD font file(s) to Arduino font file")
    parser.add_argument(
        "-i",
        metavar="<GLCD font file(s)>",
        dest="lcd_font_path",
        type=str,
        required=True,
        help="The GLCD font file(s) path or directory")
    parser.add_argument(
        "-s",
        metavar="<space size>",
        dest="space_size",
        type=int,
        default=5,
        help="The space character size, (default is 5)")
    parser.add_argument(
        "-n",
        metavar="<font name>",
        dest="font_name",
        type=str,
        help="The Arduino font name (default is the first GLCD font file name)")
    parser.add_argument(
        "-o",
        metavar="<Arduino font file>",
        dest="arduino_font_path",
        type=str,
        help="The Arduino font file path (default is the the first GLCD font file path)")
    args = parser.parse_args()

    args.image_path = os.path.abspath(args.image_path)

    main(args)
