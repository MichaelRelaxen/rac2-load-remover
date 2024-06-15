mastercode = "9011CD48 0C0472FA"
code_lines = [mastercode] + [line.upper().strip() for line in open("bin/38996035.raw") if line != "\n"]

def swap(code):
    """AABBCCDD -> DDCCBBAA"""
    return code[6] + code[7] + code[4] + code[5] + code[2] + code[3] + code[0] + code[1]

hexes = []
for code in code_lines:
    halves = code.split(" ")
    hexes.append(halves[1])
    hexes.append(halves[0])

swapped = [swap(code) for code in hexes]
joined = "".join(swapped)

result = joined

print("u32 rac2hack[1846] = {")
for i in range(0, len(result), 8):
    print(f"\t0x{result[i:i+8]},")
print("};")