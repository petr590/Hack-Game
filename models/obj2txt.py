#!/usr/bin/python3
import sys
from argparse import ArgumentParser
from plyfile import PlyData

parser = ArgumentParser()
parser.add_argument('file', type=str, help='Файл модели с расширением .ply')

args = parser.parse_args()

plydata = PlyData.read(args.file)

print(f"Model {args.file.removesuffix('.ply')}(")
print("    {")

for i, vertex in enumerate(plydata['vertex']):
    print(f"        Vertex {{ {vertex['x']: .4f}, {vertex['y']: .4f}, {vertex['z']: .4f}, 0x000000 }}, // {i}")

print("    },")
print("    {")

for face in plydata['face'].data:
	indices = face[0]
    
	if len(indices) != 3:
		print(f"Error: found face with {len(indices)} indices")
		sys.exit(1)

	print(f"        {indices[0]:2}, {indices[1]:2}, {indices[2]:2},")

print("    }")
print(");")	