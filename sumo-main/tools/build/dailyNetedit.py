#!/usr/bin/env python
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2008-2021 German Aerospace Center (DLR) and others.
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# https://www.eclipse.org/legal/epl-2.0/
# This Source Code may also be made available under the following Secondary
# Licenses when the conditions for such availability set forth in the Eclipse
# Public License 2.0 are satisfied: GNU General Public License, version 2
# or later which is available at
# https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later

# @file    dailyNetedit.py
# @author  Michael Behrisch
# @author  Jakob Erdmann
# @author  Laura Bieker
# @date    2008

"""
Does the nightly git pull on the windows server and the visual
studio build. The script is also used for the meso build.
Some paths especially for the names of the texttest output dirs are
hard coded into this script.
"""
from __future__ import absolute_import
from __future__ import print_function
import datetime
import optparse
import os
import glob
import subprocess
import sys
import logging
import logging.handlers

import status

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import sumolib  # noqa

BINARIES = ("netedit",)


def runTests(options, env, gitrev, debugSuffix=""):
    prefix = env["FILEPREFIX"] + debugSuffix
    env["SUMO_BATCH_RESULT"] = os.path.join(options.rootDir, prefix + "batch_result")
    env["SUMO_REPORT"] = os.path.join(options.remoteDir, prefix + "report")
    env["TEXTTEST_TMP"] = os.path.join(options.rootDir, prefix + "texttesttmp")
    env["TEXTTEST_HOME"] = os.path.join(options.rootDir, options.testsDir)
    if not os.path.exists(env["SUMO_REPORT"]):
        os.makedirs(env["SUMO_REPORT"])
    status.killall(debugSuffix, BINARIES)
    for name in BINARIES:
        binary = os.path.join(options.rootDir, options.binDir, name + debugSuffix + ".exe")
        if os.path.exists(binary):
            env[name.upper() + "_BINARY"] = binary
    ttBin = "texttest"
    today = datetime.date.today()
    tasks = sorted(glob.glob(os.path.join(env["TEXTTEST_HOME"], "netedit", "testsuite.netedit.daily.*")))
    taskID = os.path.basename(tasks[today.toordinal() % len(tasks)])[10:]
    cmd = [ttBin, "-b", prefix, "-a", taskID, "-name", "%sr%s" % (today.strftime("%d%b%y"), gitrev)]
    for call in (cmd, [ttBin, "-b", env["FILEPREFIX"], "-coll"]):
        status.log_subprocess_output(subprocess.Popen(
            call, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True))
    status.killall(debugSuffix, BINARIES)


optParser = optparse.OptionParser()
optParser.add_option("-r", "--root-dir", dest="rootDir",
                     default=r"D:\Sumo", help="root for git and log output")
optParser.add_option("-s", "--suffix", default="", help="suffix to the fileprefix")
optParser.add_option("-b", "--bin-dir", dest="binDir", default=r"git\bin",
                     help="directory containg the binaries, relative to the root dir")
optParser.add_option("-t", "--tests-dir", dest="testsDir", default=r"git\tests",
                     help="directory containg the tests, relative to the root dir")
optParser.add_option("-m", "--remote-dir", dest="remoteDir", default="S:\\daily",
                     help="directory to move the results to")
optParser.add_option("-p", "--python", help="path to python interpreter to use")
(options, args) = optParser.parse_args()

env = os.environ
if "SUMO_HOME" not in env:
    env["SUMO_HOME"] = os.path.dirname(
        os.path.dirname(os.path.dirname(__file__)))
env["PYTHON"] = "python"
env["SMTP_SERVER"] = "smtprelay.dlr.de"
msvcVersion = "msvc16"

platform = "x64"
env["FILEPREFIX"] = msvcVersion + options.suffix + platform
prefix = os.path.join(options.remoteDir, env["FILEPREFIX"])
testLog = prefix + "NeteditTest.log"
gitrev = sumolib.version.gitDescribe()
logger = logging.getLogger()
logger.setLevel(logging.INFO)
try:
    handler = logging.handlers.TimedRotatingFileHandler(testLog, when="d", interval=1, backupCount=5)
    logger.addHandler(handler)
except Exception as e:
    logger.error(e)
logger.info(u"%s: %s" % (datetime.datetime.now(), "Running tests."))
runTests(options, env, gitrev)
