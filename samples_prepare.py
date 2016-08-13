#!/usr/bin/env python
# encoding: utf-8
import sys
import argparse
import shutil
import os


def do_instrument_blanchet(din, dout):
    # create mapping for Blanchet 1720 harpsichord from
    # http://sonimusicae.free.fr/blanchet1-en.html
    # mapping is: midinote + 256 * n_stop + 128 * is_release()
    def iterate_through_dir():
        dirs = ['Blanchet-Jeu 1 Samples', 'Blanchet-Jeu 2 Samples', 'Blanchet-Luth Samples']
        for stop, stopdir in enumerate(dirs):
            n = stop * 256 + 20
            for f in sorted(os.listdir(os.path.join(din, stopdir))):
                if f.endswith('r.wav'):
                    yield n + 128, os.path.join(din, stopdir, f)
                    n += 1
                elif f.endswith('.wav'):
                    yield n, os.path.join(din, stopdir, f)
    if not os.path.exists(dout):
        os.makedirs(dout)
    with open(os.path.join(dout, 'blanchet.txt'), 'w') as out:
        for n, ffin in iterate_through_dir():
            ffout = os.path.join(dout, '%d.wav' % n)
            shutil.move(ffin, ffout)
            print >> out, 'addsample %d 0.2' % (n, ffout)



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    ig = parser.add_mutually_exclusive_group()
    ig.add_argument('--blanchet', action='store_true')
    parser.add_argument('input_directory')
    parser.add_argument('output_directory')
    args = parser.parse_args()
    if args.blanchet:
        do_instrument_blanchet(args.input_directory, args.output_directory)
