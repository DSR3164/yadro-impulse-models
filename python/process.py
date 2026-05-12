import matplotlib.pyplot as plt
import numpy as np

plt.rcParams.update({
    'axes.facecolor': '#1e1e1e',
    'figure.facecolor': '#1e1e1e',
    'text.color': 'white',
    'axes.labelcolor': 'white',
    'xtick.color': 'white',
    'ytick.color': 'white',
    'grid.color': "#747474",
    'font.size': 9,
    'savefig.facecolor': '#1e1e1e',
})

def apply_dark_theme(fig):
    fig.canvas.get_tk_widget().configure(bg='#1e1e1e')
    fig.canvas.manager.window.configure(bg='#1e1e1e')

_original_figure = plt.figure

def figure(*args, **kwargs):
    fig = _original_figure(*args, **kwargs)
    apply_dark_theme(fig)
    return fig

plt.figure = figure

data = np.genfromtxt("files/stats.csv", delimiter=',', skip_header=1)

time = data[:, 0] / 1e3
size_bytes = data[:, 1]

inter_packet_time = np.diff(time)

mean_ipt = inter_packet_time.mean()
mean_size = np.array(size_bytes).mean()

total_seconds = time[-1] / 1e6
total_bits = np.sum(size_bytes) * 8

bitrate = total_bits / total_seconds

print(f"Mean packet size: {mean_size:.2f} bytes")
print(f"Mean inter packet delay: {mean_ipt:.2f} us")
print(f"Mean bitrate: {bitrate / 1e6:.2f} Mbit/s")

plt.figure(figsize=(10, 5))
plt.plot(time / 1e6, size_bytes, label='Packet size over time')
plt.xlabel("Time in seconds from start")
plt.ylabel("Size (bytes)")
plt.title('Packet size over time')
plt.grid(True)
plt.tight_layout()

plt.figure(figsize=(10, 5))
plt.hist(inter_packet_time, bins=100, label='Inter packet time hist')
plt.ylabel("Count")
plt.xlabel("Time (us)")
plt.title('Inter packet time hist')
plt.grid(True)
plt.tight_layout()

plt.figure(figsize=(10, 5))
plt.hist(size_bytes, bins=100, label='Packet size hist')
plt.ylabel("Count")
plt.xlabel("Size (bytes)")
plt.title('Packet size hist')
plt.grid(True)
plt.tight_layout()

plt.show()
