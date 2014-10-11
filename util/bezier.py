def b(t, p):
    a = lambda axis: p[0][axis] * (1 - t) * (1 - t)
    b = lambda axis: p[1][axis] * (1 - t) * t * 2
    c = lambda axis: p[2][axis] * t * t
    return (a(0) + b(0) + c(0), a(1) + b(1) + c(1))

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
    return '{ ' str[:-2] ' }'

    
if __name__ == "__main__":
    # Usage: bezier.py [generator_file.bez] [output_file.h]
    # Take in a standard ".bez" file and emit a header containing the bezier curves encoded as an array.
    # 
    # Note that this program will only generate approximations for _quadratic_ bezier curves.
    #
    # BEZ Format:
    #  sym   = [a-z_]+
    #  int   = [0-9]+
    #  point = (int, int)
    #  line  = sym : point point \n
    #  file  = line+
    #
    # You may notice that while this represents a quadratic bezier curve, there are only two points
    # being given. This is because there is an implicit P[0] of (0, 0).
    #
    # Example BEZ File:
    # curve_a: 
    # curve_b
    
