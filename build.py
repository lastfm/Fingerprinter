#!/usr/bin/env python
#-----------------------------------------------------------------------------
# This file is part of the Last FM build system and can be safely ignored.
#-----------------------------------------------------------------------------

from mirbuild import *

project = CMakeProject('libfplib')
project.version('include/fplib/version.h')
project.run()

#-----------------------------------------------------------------------------
