# Usage: bezier.py [generator_file.bez] [output_file.h]
# Take in a standard ".bez" file and emit a header containing the bezier curves encoded as an array.
# 
# Note that this program will only generate approximations for _quadratic_ bezier curves.
#
# BEZ Format:
#  sym    = [a-z_]+
#  int    = [0-9]+
#  point  = (int,int)
#  frames = int
#  line   = sym: frames point point\n
#  file   = line+
#
# You may notice that while this represents a quadratic bezier curve, there are only two points
# being given. This is because there is an implicit P[0] of (0, 0).
#
# Example BEZ File:
# curve_a: 30 (50,50) (100,0)
# curve_b: 20 (20,50) (40,0)
#
import sys
import os
import re

def b(t, p):
    #a = lambda axis: p[0][axis] * (1 - t) * (1 - t)
    b = lambda axis: p[0][axis] * (1 - t) * t * 2
    c = lambda axis: p[1][axis] * t * t
    return (b(0) + c(0), b(1) + c(1))

def bezier_array(points, frames):
    """
    Give an array of equi-distant points along the quadratic bezier curve represented by the given points.
    
    `points` is an array of two-tuples that represent the bezier curve points
    other than the first being (0,0).
    `frames` is the number of frames that the bezier curve should span across
    return an array of points of length `frames` offsets from (0,0)
    the array encodes the discrete points between 
    """
    out = []
    for i in range(frames + 1):
        t = i / frames
        out.append(b(t, points))
    return out

def c_bezier_array(points, frames):
    str = ""
    for p in bezier_array(points, frames):
        p = (round(p[0]), round(p[1]))
        str += '{'
        str += "{}, {}".format(p[0], p[1])
        str += '}, '
    return '{ ' + str[:-2] + ' }'

    
if __name__ == "__main__":
    # Handle command line arguments
    args        = sys.argv
    if(len(args) < 3):
        print("Usage: bezier.py [generator_file.bez] [output_file.h]")
        sys.exit(0)
    input_file  = args[1]
    output_file = args[2]
    base_output_file = os.path.basename(output_file)
    base_output_file = os.path.splitext(base_output_file)[0]
    # Initialize regex-based line parser
    parser = re.compile('([a-z_]+): ([0-9]+) \(([0-9]+),([0-9]+)\) \(([0-9]+),([0-9]+)\)')
    # Insert the beginning of the output header file
    out  = '#ifndef ' + base_output_file.upper() + '_H\n'
    out += '#define ' + base_output_file.upper() + '_H\n'
    out += 'typedef struct point_diff { u8 x; u8 y } point_diff;\n\n'
    # Iterate over each line, extracting each field
    for line in open(sys.argv[1]):
        match = parser.match(line)
        (sym, frames, x1, y1, x2, y2) = match.groups()
        frames = int(frames)
        points = [(int(x1), int(y1)), (int(x2), int(y2))]
        array_str = c_bezier_array(points, frames)
        out += 'const point_diff ' + sym + '[] = ' + array_str + ';\n'
        out += '#define ' + sym + '_len sizeof(' + sym + ') / sizeof(point_diff)\n\n'
    out += '#endif\n'
    out_file = open(output_file, 'w')
    out_file.write(out)
    out_file.close()
    print("Bezier curve generation complete!")
