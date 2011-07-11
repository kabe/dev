#!/usr/bin/env python

import os


ALIGN=24


def main():
    """
    """
    os_props = dir(os)
    props = []
    for prop in os_props:
        if prop.startswith("O_"):
            props.append((getattr(os, prop), prop))
            #print "%s\t\t0%o" % (prop, getattr(os, prop))
    for p in sorted(props, lambda x, y: cmp(x[0], y[0])):
        padding = " " * (ALIGN - len(p[1]))
        print "%s%s0x%08X" % (p[1], padding, p[0])


if __name__ == '__main__':
    main()
