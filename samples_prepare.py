#!/usr/bin/env python
# encoding: utf-8
import argparse
import shutil
import os


def do_instrument_blanchet(din, dout):
    # create mapping for Blanchet 1720 harpsichord from
    # http://sonimusicae.free.fr/blanchet1-en.html
    # mapping is: midinote + 256 * n_stop + 128 * is_release()
    def iterate_through_dir():
        dirs = [
            "Blanchet-Jeu 1 Samples",
            "Blanchet-Jeu 2 Samples",
            "Blanchet-Luth Samples",
        ]
        for stop, stopdir in enumerate(dirs):
            n = stop * 256 + 20
            for f in sorted(os.listdir(os.path.join(din, stopdir))):
                if f.endswith("r.wav"):
                    yield n + 128, os.path.join(din, stopdir, f)
                    n += 1
                elif f.endswith(".wav"):
                    yield n, os.path.join(din, stopdir, f)

    with open(os.path.join(dout, "blanchet.txt"), "w") as out:
        for n, ffin in iterate_through_dir():
            ffout = os.path.join(dout, "%d.wav" % n)
            shutil.move(ffin, ffout)
            # print >> out, "addsample %d 0.2" % (n, ffout)
            print("addsample %d %s" % (n, ffout), file=out)


def do_instrument_green_positiv(din, dout):
    # create mapping for Green Positiv organ from
    # https://piotrgrabowski.pl/green-positiv
    # (use the grandorgue file and unrar it)
    def iterate_through_dir():
        for stop, stopdir in enumerate(
            [
                "Data - Green Positiv/M Pryncypal 2/A0",
                "Data - Green Positiv/M Flet kryty 4",
                "Data - Green Positiv/M Flet kryty 8",
            ]
        ):
            n = stop * 256 + 36
            for f in sorted(os.listdir(os.path.join(din, stopdir))):
                if f.endswith(".wav"):
                    yield n, os.path.join(din, stopdir, f)
                    n += 1

    with open(os.path.join(dout, '../', "greenpositiv.txt"), "w") as out:
        for n, ffin in iterate_through_dir():
            ffout = os.path.join(dout, "%d.wav" % n)
            shutil.copy(ffin, ffout)
            print("addsample %d %s" % (n, ffout), file=out)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    ig = parser.add_mutually_exclusive_group()
    ig.add_argument("--blanchet", action="store_true")
    ig.add_argument("--green-positiv", action="store_true")
    parser.add_argument("input_directory")
    parser.add_argument("output_directory")
    args = parser.parse_args()
    if not os.path.exists(args.output_directory):
        os.makedirs(args.output_directory)
    if args.blanchet:
        do_instrument_blanchet(args.input_directory, args.output_directory)
    if args.green_positiv:
        do_instrument_green_positiv(args.input_directory, args.output_directory)
