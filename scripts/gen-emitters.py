#!/usr/bin/env python3

from argparse import ArgumentParser
parser = ArgumentParser()
parser.add_argument("--output", "-o", required=True, help="Output path of generated header")
parser.add_argument("--opcodes", "-i", required=True, help="Input path: riscv-opcodes repository - https://github.com/riscv/riscv-opcodes")
args = parser.parse_args()

argument_aliases = {
     "bimm12hi": "",
     "bimm12lo": "bimm12",
     "imm12hi": "",
     "imm12lo": "imm12_split",
}

label_arguments = ["bimm12", "jimm20"]

def process_constant(constant):
    bits, value = constant.split("=")
    value = int(value, 0)
    bits = bits.split("..")
    if len(bits) == 1:
        bit = int(bits[0])
        if value > 1 or value < 0:
            raise ValueError(f"Trying to set a single bit to {value}!")
        if value == 1:
            if bit == 0:
                return "1"
            else:
                return f"(1 << {bit})"
    elif len(bits) == 2:
        msb = int(bits[0])
        lsb = int(bits[1])

        num_bits = (msb - lsb) + 1
        max_value = (1 << num_bits) - 1
        if value > max_value:
            raise ValueError("Tried to set bits {msb} through {lsb} to {value}, which is over the maximum value for {num_bits} of {max_value}")
        if lsb == 0:
            return f"{value}"
        else:
            return f"({value} << {lsb})"
    else:
        raise ValueError("Invalid constant definition '{constant}'")

def process_instruction(line):
    # Skip empty lines, comments, and directives (for now)
    if len(line) == 0 or line[0] in ("#", "$"):
            return []

    result = [f"// {line}"]

    line = line.split()
    line[0] = line[0].capitalize()

    instruction_name = line[0]
    assembler_name = f"Assemble{instruction_name.capitalize()}"
    emitter_name = f"Emit{instruction_name.capitalize()}"

    arguments = []
    constants = []
    for token in line[1:]:
        if "=" in token:
            constants.append(token)
        else:
            if token in argument_aliases:
                 token = argument_aliases[token]
            if token not in arguments and len(token) > 0:
                arguments.append(token)
    
    processed_constants = " | ".join([process_constant(const) for const in constants if len(const) > 0])
    processed_arguments = " | ".join([f"CRV_RV_ARG_{arg}({arg})" for arg in arguments])
    terms = [processed_constants, processed_arguments]
    result.append(f"#define CRV_{assembler_name}({', '.join(arguments)}) ({' | '.join([term for term in terms if len(term) > 0])})")
    used_label_arguments = list(set(arguments).intersection(label_arguments))
    if len(used_label_arguments) == 0:
        result.append(f"#define CRV_{emitter_name}({', '.join(['ctx'] + arguments)}) _CRV_Emit_32b(ctx, CRV_{assembler_name}({', '.join(arguments)}))")
    elif len(used_label_arguments) == 1:
        label_arg = used_label_arguments[0]
        result.append(f"#define CRV_{emitter_name}({', '.join(['ctx'] + arguments)}) _Generic(({label_arg}), CRV_LABEL*: _CRV_Emit_32b_Label, default: _CRV_Emit_32b_Label_Ignore) \\")
        result.append(f"    (ctx, CRV_{assembler_name}({', '.join(arguments)}), {label_arg}, _CRV_LABEL_FIELD_{label_arg})")

        pass
    else:
        raise ValueError(f"Only up to one label argument is supported, but got: {used_label_arguments}")

    return result

def process_file(file):
    result = []
    for line in open(file):
        result += process_instruction(line.strip())
    return result
    

result = "\n".join(process_file(f"{args.opcodes}/rv_i"))
f = open(args.output, "w")
f.write(
"""/*
!!! WARNING !!!
This file was automatically generated!
Do not edit it, your changes will be lost!
*/


""")
f.write(result)
f.close()
