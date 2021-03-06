#!/usr/bin/env python
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2009-2018 German Aerospace Center (DLR) and others.
# This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html
# SPDX-License-Identifier: EPL-2.0

# @file    test.py
# @author  Pablo Alvarez Lopez
# @date    2016-11-25
# @version $Id$

# import common functions for netedit tests
import os
import sys

testRoot = os.path.join(os.environ.get('SUMO_HOME', '.'), 'tests')
neteditTestRoot = os.path.join(
    os.environ.get('TEXTTEST_HOME', testRoot), 'netedit')
sys.path.append(neteditTestRoot)
import neteditTestFunctions as netedit  # noqa

# Open netedit
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot, ['--gui-testing-debug-gl'])

# go to select mode
netedit.selectMode()

# select all using invert
netedit.selectionInvert()

# go to inspect mode
netedit.inspectMode()

# inspect chargingStations
netedit.leftClick(referencePosition, 150, 250)

# Set name
netedit.modifyAttribute(0, "commonName")

# Set friendlyPos
netedit.modifyBoolAttribute(1)

# Set invalid Power
netedit.modifyAttribute(2, "-1000")

# Set Power
netedit.modifyAttribute(2, "11000")

# Try to set invalid efficiency
netedit.modifyAttribute(3, "2")

# set efficiency
netedit.modifyAttribute(3, "0.5")

# Set charge in transit
netedit.modifyBoolAttribute(4)

# Set invalid charge delay
netedit.modifyAttribute(5, "-2")

# Set valid charge delay
netedit.modifyAttribute(5, "6.3")

# Set charge in transit
netedit.modifyBoolAttribute(9)

# Check undo redo
netedit.undo(referencePosition, 9)
netedit.redo(referencePosition, 9)

# save additionals
netedit.saveAdditionals()

# save network
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
