def getx(y:int, bits:int, debug:bool=False):
    ybin = bin(y)[2:].zfill(bits)
    count1 = ybin.count('1')
    count0 = ybin.count('0')
    assert count1 + count0 == bits

    ybin = ybin[::-1]
    linesstartswith0 = ybin[:count0]
    linesstartswith1 = ybin[count0:]

    count0segments = linesstartswith0.count('1')
    count1segments = linesstartswith1.count('0')

    assert count0segments == count1segments

    segments = {}
    #segments["example"] = {"no": "A1", "1count": 6, "0count": 1, "secondlastbit": 0, "prev0count": 1, "prevsegment": "A5"}

    linesmark = {}
    #linesmark["example"] = {"inherited": False, "inheritfromline": 11, "nextline": 5, "segmentno": "A1", "inheritdepth": 0, "0count": 1, "1count": 2}

    countasegments = 0
    nextline = count0 - 1
    inheritfromline = bits - 1
    for i in range(bits-1, count0-1, -1):
        assert ybin[i] in ['0', '1']
        if ybin[i] == '0':
            # new 1-segment/A-segment
            countasegments = countasegments + 1
            segmentno = f"A{countasegments}"
            segments[segmentno] = {"no": segmentno, "line": i, "1count": 1}
            assert nextline >= 0
            linesmark[i] = {"inherited": False, "nextline": nextline, "secondlastbit": ybin[nextline], "segmentno": segmentno, "inheritdepth": 0}
            

            nextline = nextline - 1
        else:
            # inherited 1-segment/A-segment
            assert inheritfromline >= count0
            linesmark[i] = {"inherited": True, "inheritfromline": inheritfromline, "segmentno": linesmark[inheritfromline]["segmentno"], "inheritdepth": linesmark[inheritfromline]["inheritdepth"] + 1}
            segments[linesmark[i]["segmentno"]]["1count"] = linesmark[i]["inheritdepth"] + 1

            inheritfromline = inheritfromline - 1

    assert countasegments == count1segments
    one_segments_lengths_sorted = sorted([segments[f"A{segmentno}"]["1count"] for segmentno in range(1, countasegments+1)])[::-1]  

    countbsegments = 0
    inheritfromline = count0
    nextline = 0
    for i in range(0, count0):
        if ybin[i] == '1':
            # new 0-segment/B-segment, inherits from some 1-leading-line
            countbsegments = countbsegments + 1
            segmentno = f"B{countbsegments}"
            segments[segmentno] = {"no": segmentno, "line": i, "0count": 1}
            assert inheritfromline < bits
            linesmark[i] = {"inherited": True, "inheritfromline": inheritfromline, "segmentno": segmentno, "depth": 0, "secondlastbit": ybin[inheritfromline]}

            inheritfromline = inheritfromline + 1
        else:
            # inherited 0-segment (but don't set inherited flag here)
            assert nextline < i and nextline >= 0
            linesmark[i] = {"inherited": False, "nextline": nextline, "secondlastbit": ybin[nextline], "segmentno": linesmark[nextline]["segmentno"], "depth": linesmark[nextline]["depth"] + 1}
            segments[linesmark[i]["segmentno"]]["0count"] = linesmark[i]["depth"] + 1

            nextline = nextline + 1

    assert countbsegments == count0segments
    zero_segments_lengths_sorted = sorted([segments[f"B{segmentno}"]["0count"] for segmentno in range(1, countbsegments+1)])[::-1]

    # note that A1 and B1 have the max lengths, and An and Bn have the lengths of 1

    if debug:
        print(segments)

    # match A's and B's segments 
    for i in range(1, countasegments + 1):
        asegmentno = linesmark[count0 + (i-1)]["segmentno"]
        segments[asegmentno]["0count"] = segments["B" + str(i)]["0count"]
        segments["B" + str(i)]["prevsegment"] = asegmentno

    #print(segments)

    # link these segments
    for i in range(1, countasegments + 1):
        j = linesmark[segments[f"A{i}"]["line"]]["nextline"]
        while linesmark[j]["inherited"] == False:
            j = linesmark[j]["nextline"]
        assert j < count0
        j = linesmark[j]["inheritfromline"]
        assert j >= count0
        prevsegmentno = linesmark[j]["segmentno"]
        assert prevsegmentno.startswith("A")
        segments[f"A{i}"]["prevsegment"] = prevsegmentno

    for i in range(1, countasegments + 1):
        asegmentno = f"A{i}"
        assert "prevsegment" in segments[asegmentno]

    if debug:
        print(segments)

    # rebuild X
    segmentno = "A1"
    result = ""
    while True:
        if debug:
            print(segmentno)
        segmentno = segments[segmentno]["prevsegment"]
        assert segmentno[0] == 'A'
        result = "1" * segments[segmentno]["1count"] + "0" * segments[segmentno]["0count"] + result
        if segmentno == "A1":
            break
    
    if len(result) < bits: # sepcial case: x is repeated
        assert bits % len(result) == 0
        result = result * (bits // len(result))

    return result

def str_to_bits(s):
    return " ".join([bin(ord(b))[2:].zfill(8) for b in s])

def bits_to_str(b):
    return "".join([chr(int(b[i:i+8], 2)) for i in range(0, len(b), 8)])

def solve(raw_hex_output: str):
    y = "".join([bin(int(h, 16))[2:].zfill(8) for h in raw_hex_output.split(" ")])[::-1]
    bits = len(y)
    x = getx(int(y, 2), bits)

    # one possible solution
    # return x

    # generate other possible solutions (by Rotated Bit Shifting)
    x = int(x, 2)
    nums = set()
    for times in range(bits):
        xs = bin(x)[2:].zfill(bits)
        if xs in nums:
            break
        nums.add(xs)
        yield bits_to_str(xs)

        x = x * 2
        if x >= 2 ** bits:
            x = x ^ (2 ** bits) ^ 1
            
            
# these bytes are extracted from the binary
results = solve("de 75 68 ed fe b2 4d df b0 0f 22 8f fe 2f e8 9b 1d 9e f9 de ff b0 db ff f9 61 b4 f8 00 ef 7d b0 ab f8 08 1f 00 b0 dc c7 dd f8 00 23 ad 4f b5 00")

import string
for possbileresult in results: # number of possible solutions is equal to bits
    if str(possbileresult).isprintable():
        print(possbileresult)

# PCTF{clever_pun_about_burrows_wheeler_goes_here}