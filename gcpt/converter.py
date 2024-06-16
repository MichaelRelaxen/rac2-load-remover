mastercode = "0011CD48 0C0472FA"
code_lines = [mastercode] + [line.upper().strip() for line in open("bin/38996035.raw") if line != "\n"]

def swap(code):
    """AABBCCDD -> DDCCBBAA"""
    return code[6] + code[7] + code[4] + code[5] + code[2] + code[3] + code[0] + code[1]

hexes = []
for code in code_lines:
    halves = code.split(" ")
    hexes.append(halves[0])
    hexes.append(halves[1])

swapped = [code for code in hexes]
joined = "".join(swapped)

result = joined

output = ""
for i in range(0, len(result), 8):
    output += f"\t0x{result[i:i+8]},\n"

num_lines = len(output.split("\n"))
output = f"u32 RacTimer[{num_lines - 1}] =" + " {\n" + output + "};"   
print(output)