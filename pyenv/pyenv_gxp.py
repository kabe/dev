#!/usr/bin/env python

import os

if __name__ == "__main__":
    for k, v in os.environ.iteritems():
        if k.startswith("GXP_"):
            print [k, v]

