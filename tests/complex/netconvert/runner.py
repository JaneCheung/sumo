#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2008-2018 German Aerospace Center (DLR) and others.
# This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html

# @file    runner.py
# @author  Jakob Erdmann
# @author  Michael Behrisch
# @date    2013-08-16
# @version $Id$

"""
import sumo network then export to the given format and import again
and check for idempotency with the original sumo network
"""
from __future__ import absolute_import


import sys
import os
import subprocess
sys.path.append(
    os.path.join(os.path.dirname(__file__), '..', '..', '..', "tools"))
import sumolib  # noqa

format = sys.argv[1]
netconvert = sumolib.checkBinary('netconvert')

args1 = [netconvert,
         '--sumo-net-file', 'input_net.net.xml',
         '--%s-output' % format, format]

args2 = [netconvert, '--%s' % format, format] + sys.argv[2:]

subprocess.call(args1)
subprocess.call(args2)
