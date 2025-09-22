#!/bin/python3
import matplotlib.pyplot as plt

# Чтение данных из файла в формате:
# возвращает dict: {shader_id: [время_покадрово]}
def read_times_per_frame(filename):
	times = {}
	with open(filename, 'r') as f:
		for line in f:
			parts = line.strip().split()
			if len(parts) != 2:
				continue
			shader_id, time = int(parts[0]), int(parts[1])
			if shader_id not in times:
				times[shader_id] = []
			times[shader_id].append(time)
	return times

old_times = read_times_per_frame('shaders-time.log.old')
new_times = read_times_per_frame('shaders-time.log')

shader_ids = sorted(set(old_times.keys()) | set(new_times.keys()))

max_frames = 0
for shader_id in shader_ids:
	max_frames = max(max_frames,
					 len(old_times.get(shader_id, [])),
					 len(new_times.get(shader_id, [])))

fig, axes = plt.subplots(len(shader_ids), 1, figsize=(12, 3*len(shader_ids)), sharex=True)
if len(shader_ids) == 1:
	axes = [axes]

for axis, shader_id in zip(axes, shader_ids):
	old = old_times.get(shader_id, [])
	new = new_times.get(shader_id, [])
	
	frames = list(range(1, max_frames+1))
	
	old_padded = old + [0]*(max_frames - len(old))
	new_padded = new + [0]*(max_frames - len(new))
	
	axis.plot(frames, old_padded, color='gray', label='старое')
	
	colors = []
	for o, n in zip(old_padded, new_padded):
		d = o - n
		colors.append('orange' if abs(d) <= 1e5 else 'green' if d >= 0 else 'red')
	
	axis.scatter(frames, new_padded, color=colors, label='новое')
	
	axis.set_ylabel(f'Шейдер {shader_id}')
	axis.legend(loc='upper right')
	axis.grid(True)

axes[-1].set_xlabel('Кадр')

plt.suptitle('Время выполнения шейдеров по кадрам (старое и новое)')
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
