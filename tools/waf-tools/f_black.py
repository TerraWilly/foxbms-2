#!/usr/bin/env python
# -*- coding: utf-8 -*-

# @copyright &copy; 2010 - 2021, Fraunhofer-Gesellschaft zur Foerderung der
#   angewandten Forschung e.V. All rights reserved.
#
# BSD 3-Clause License
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1.  Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
# 3.  Neither the name of the copyright holder nor the names of its
#     contributors may be used to endorse or promote products derived from this
#     software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# We kindly request you to use one or more of the following phrases to refer to
# foxBMS in your hardware, software, documentation or advertising materials:
#
# &Prime;This product uses parts of foxBMS&reg;&Prime;
#
# &Prime;This product includes parts of foxBMS&reg;&Prime;
#
# &Prime;This product is derived from foxBMS&reg;&Prime;

r"""Implements a waf tool to run `black <https://github.com/psf/black>`_.

:numref:`f-black-usage` shows how to use this tool.

.. code-block:: python
    :caption: f_black.py
    :name: f-black-usage
    :linenos:

    def options(opt):
        opt.load("black")

    def configure(conf):
        conf.load("black")

    def build:
        files = bld.path.ant_glob("\*\*/\*.py")
        bld(features="black", files=files)

"""

from waflib import Task, TaskGen


class black(Task.Task):  # pylint: disable-msg=invalid-name
    """Class to implement running the black formatting tool on Python files"""

    #: str: color in which the command line is displayed in the terminal
    color = "BLUE"
    vars = ["BLACK_OPTIONS"]

    run_str = "${BLACK} ${BLACK_OPTIONS} ${SRC[0].abspath()}"

    def keyword(self):
        """displayed keyword when black is run"""
        return "Formatting"


@TaskGen.feature("black")
def process_black(self):
    """creates black tasks for each input file"""
    if not getattr(self, "files", None):
        self.bld.fatal("No files given.")
    for src in self.files:
        self.create_task("black", src, cwd=self.path)


def options(opt):
    """Passing options to black"""
    opt.add_option(
        "--black-option",
        action="append",
        default=[],
        dest="BLACK_OPTION",
        help="Options for black",
    )


def configure(conf):
    """configuration step of the black tool

    - searches for the program ``black``
    - applies configured options
    """
    conf.find_program("black", var="BLACK")
    conf.env.append_unique("BLACK_OPTIONS", conf.options.BLACK_OPTION)
