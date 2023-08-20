#!/usr/bin/env python3
print("Field, e.g. 'imm12'")
field = input("Field: ")

print("Number of segments: is the immediate split into multiple segments in the instruction? Such as in SB or BEQ")
num_segments = int(input("Num segments: "))

def num_bits(pattern):
    if type(pattern) == list:
        return sum([num_bits(p) for p in pattern])
    elif type(pattern) == str:
        pattern = pattern.split(":")
        if len(pattern) == 1:
            return 1
        elif len(pattern) == 2:
            msb, lsb = pattern
            return (int(msb) - int(lsb)) + 1
        else:
            raise ValueError("Invalid pattern")

masks_shifts = []
for seg in range(num_segments):
    seg_lsb = int(input(f"LSB of segment #{seg + 1}: "))
    print("Bit pattern, e.g. '12|10:5")
    bit_pattern = input("Bit pattern: ").split("|")
    num_total_bits = num_bits(bit_pattern)
    seg_msb = seg_lsb + num_total_bits - 1

    new_msb = seg_msb
    for chunk in bit_pattern:
        num_chunk_bits = num_bits(chunk)
        chunk = chunk.split(":")
        if len(chunk) == 1:
            old_bit = int(chunk[0])
            mask = 1 << old_bit
            shift = new_msb - old_bit
            masks_shifts.append((mask, shift))
        elif len(chunk) == 2:
            old_msb = int(chunk[0])
            old_lsb = int(chunk[1])

            new_lsb = new_msb - (num_chunk_bits - 1)
            mask = ((1 << num_chunk_bits) - 1) << old_lsb
            shift = new_lsb - old_lsb
            masks_shifts.append((mask, shift))
            pass
        else:
            raise ValueError("Invalid pattern")

        new_msb -= num_chunk_bits

expressions = []
for mask, shift in masks_shifts:
    shift_op = "<<"
    if shift < 0:
        shift_op = ">>"
    shift = abs(shift)
    expressions.append(f"((({field}) & 0x{mask:X}) {shift_op} {shift})")

print(" | \\\n".join(expressions))