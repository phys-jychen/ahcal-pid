import uproot as up
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm, colors
from mpl_toolkits.mplot3d import Axes3D
import argparse
from os.path import join

CellWidthX = 40.0
CellWidthY = 40.0
LayerThick = 30.0
GapX = 0.3
GapY = 0.3
PeriodX = CellWidthX + GapX
PeriodY = CellWidthY + GapY
nCellX = 18
nCellY = 18
nLayer = 40
WidthX = CellWidthX * nCellX
WidthY = CellWidthY * nCellY
Thick = LayerThick * nLayer


def read_file(fname: str, tree: str, event_index: int):
    with up.open(fname) as f:
        tree = f[tree]

        Hit_X = tree['Hit_X_nonzero'].array(library='np')
        Hit_Y = tree['Hit_Y_nonzero'].array(library='np')
        Hit_Z = tree['Hit_Z_nonzero'].array(library='np')
        Hit_Energy = tree['Hit_Energy_nonzero'].array(library='np')

        x = np.round(Hit_X[event_index] / PeriodX + 0.5).astype(int)
        y = np.round(Hit_Y[event_index] / PeriodY + 0.5).astype(int)
        z = np.round(Hit_Z[event_index] / LayerThick).astype(int)
        energy = Hit_Energy[event_index]

        znew, ynew, xnew, enew = (np.array(a) for a in zip(*sorted(zip(z, y, x, energy), reverse = True)))

        return xnew, ynew, znew, enew


def plot(fname: str, tree: str, event_index: int, title: str):
    x, y, z, energy = read_file(fname, tree, event_index)
    energy_norm = energy / np.max(energy)

    nhits = len(x)
    assert nhits == len(y)
    assert nhits == len(z)
    assert nhits == len(energy)

    fig, ax = plt.subplots(subplot_kw={'projection': '3d'})
    plt.gca().set_box_aspect((WidthX / Thick, 1, WidthY / Thick))
    cmap = cm.OrRd

    for i in np.arange(nhits):
        xnew = np.arange(x[i] - 1, x[i] + 1)
        ynew = np.arange(y[i] - 1, y[i] + 1)
        xnew, ynew = np.meshgrid(xnew, ynew)
        znew = z[i] * np.ones(xnew.shape)
        enew = energy_norm[i] * np.ones(xnew.shape)

        ax.plot_surface(xnew, znew, ynew, cmap=cmap, facecolors=cmap(enew), edgecolor='k', alpha=0.8, lw=0.05, rstride=1, cstride=1, antialiased=False)

    ax.set_xlim(-9, 9)
    ax.set_ylim(0, nLayer)
    ax.set_zlim(-9, 9)
    ax.set_xticks(np.linspace(-9, 9, 7), 4 * np.linspace(-9, 9, 7).astype(np.int8))
    ax.set_yticks(np.linspace(0, nLayer, 5))
    ax.set_zticks(np.linspace(-9, 9, 7), 4 * np.linspace(-9, 9, 7).astype(np.int8))
    ax.set_aspect(aspect='equalxz')
    ax.grid(False)

    fig.suptitle(title, size='xx-large')
    ax.invert_xaxis()
    ax.set_xlabel("X [cm]", size='x-large')
    ax.set_ylabel("Z [layer]", size='x-large')
    ax.set_zlabel("Y [cm]", size='x-large')

    m = plt.cm.ScalarMappable(cmap=cmap)
    m.set_array(energy)
    plt.colorbar(m, pad=0.2, ax=plt.gca()).set_label(label="Hit Energy [MeV]", size='x-large')

    ax.view_init(elev=20, azim=-35, roll=0)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument("-f", "--file", type=str, default='', required=True, help="Input ROOT file")
    parser.add_argument("-t", "--tree", type=str, default='Calib_Hit', help="Input tree name (default: Calib_Hit)")
    parser.add_argument("-i", "--title", type=str, default='', help="Title of display figure")
    parser.add_argument("-e", "--event", type=int, default=0, help="The event to be displayed")
    parser.add_argument("-d", "--dir", type=str, default=None, help="Directory to save the plot")
    parser.add_argument("-o", "--output", type=str, default=None, help="Output file name")
    parser.add_argument("-s", "--show", type=int, default=1, choices=[0, 1], help="Instantly display or not")
    args = parser.parse_args()

    filename = args.file
    tree = args.tree
    title = args.title
    event_index = args.event
    save_dir = args.dir
    output = args.output
    show = args.show

    plot(filename, tree, event_index, title)

    if save_dir and output:
        plt.savefig(join(save_dir, output))
        print("Figure", join(save_dir, output), "successfully created!")

    if show:
        plt.show()
