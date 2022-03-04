import glob
import tecplot as tp

files = glob.glob("./*.plt")
t = 0.0
for plt_file in files:
    tp.new_layout()
    dataset = tp.data.load_tecplot(plt_file )
    for zone in dataset.zones():
        zone.solution_time = t
        zone.strand = zone.index + 1
        t += 0.1
    tp.data.save_tecplot_plt(plt_file )