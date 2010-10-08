#!/usr/bin/env python

import os

if __name__ == "__main__":
    for k, v in os.environ.iteritems():
        print [k, v]

