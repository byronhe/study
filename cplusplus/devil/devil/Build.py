#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#
# Copyright (c) 2016 ASMlover. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list ofconditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materialsprovided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
from __future__ import print_function

import argparse
import os
import platform
import shutil
import subprocess
import sys

WINDOWS_MAKEFILE = """
OUTDIR	= {outdir}
OBJDIR	= obj
BINDIR	= bin
TARGET	= $(OUTDIR)\$(BINDIR)\{target}.exe
RM	= del /s /f /q
CC	= cl -c -nologo
MT	= mt -nologo
LINK	= link -nologo
CFLAGS	= -GS -Zi -Fd"$(OUTDIR)\$(BINDIR)\\vc.pdb" -O2 -W2 -MDd -EHsc -D_DEBUG -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS
LDFLAGS	= -INCREMENTAL -DEBUG -PDB:$(TARGET).pdb -manifest -manifestfile:$(TARGET).manifest -manifestuac:no winmm.lib
OBJS	= {objs}

all: $(TARGET)
rebuild: clean all
clean:
	$(RM) $(OUTDIR)\$(BINDIR)\*.* $(OUTDIR)\$(OBJDIR)\*.*

$(TARGET): $(OBJS)
	$(LINK) -out:$(TARGET) $(OBJS) $(LDFLAGS)
	$(MT) -manifest $(TARGET).manifest -outputresource:$(TARGET);1

{make_objs}
"""
WINDOWS_CCOBJ = """$(OUTDIR)\$(OBJDIR)\{make_obj}: {make_src}
    $(CC) -Fo:$(OUTDIR)\$(OBJDIR)\{make_obj} $(CFLAGS) {make_src}
"""

POSIX_MAKEFILE = """
OUTDIR	= {outdir}
OBJDIR	= obj
BINDIR	= bin
TARGET	= $(OUTDIR)/$(BINDIR)/{target}
RM	= rm -rf
CC	= {cc}
CFLAGS	= -g -O2 -Wall -std=c++0x
LDFLAGS	= {ldflags}
OBJS	= {objs}

all: $(TARGET)
rebuild: clean all
clean:
	$(RM) $(OUTDIR)/$(BINDIR)/*.* $(OUTDIR)/$(OBJDIR)/*.*

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

{make_objs}
"""
POSIX_CCOBJ = """$(OUTDIR)/$(OBJDIR)/{make_obj}: ${make_src}
	$(CC) -o $(OUTDIR)/$(OBJDIR)/{make_obj} -c $(CFLAGS) {make_src}
"""

LINUX_MKDIR = {'cc': 'g++', 'ldflags': '-lpthread'}
DARWIN_MKDIR = {'cc': 'clang++', 'ldflags': '-lc -lpthread'}
TARGET = 'devil'
OUTDIR = 'build'
DIRS = ['utility', 'net', 'test']

if sys.version_info.major < 3:
    import codecs
    def do_open(fname, mode='rb', encoding=None):
        return codecs.open(fname, mode, encoding)
else:
    do_open = open

def get_platform():
    return platform.system().lower()

def safe_mkdir(dirpath):
    if not os.path.exists(dirpath):
        os.mkdir(dirpath)

def safe_rmdir(dirpath):
    if os.path.exists(dirpath):
        shutil.rmtree(dirpath)

def safe_rm(path):
    if os.path.exists(path):
        if os.path.isdir(path):
            safe_mkdir(path)
        else:
            os.remove(path)

def get_sources_from_dir(dirpath):
    cur_sources = os.listdir(dirpath)
    all_sources = []
    for source_fname in cur_sources:
        source_fullpath = os.path.join(dirpath, source_fname)
        if os.path.isdir(source_fullpath):
            all_sources.extend(get_sources_from_dir(source_fullpath))
        else:
            if os.path.splitext(source_fname)[1][1:] in ('cpp', 'cc'):
                all_sources.append(source_fullpath)
    return all_sources

def get_all_sources(dirs=['./']):
    all_sources = []
    for path in dirs:
        all_sources.extend(get_sources_from_dir(path))
    return all_sources

def gen_windows_obj(source_fname):
    s = source_fname.split('\\')[-1]
    return '$(OUTDIR)\$(OBJDIR)\{objname}.obj '.format(objname=os.path.splitext(s)[0])

def gen_posix_obj(source_fname, outdir):
    s = source_fname.split('/')[-1]
    return '$(OUTDIR)/$(OBJDIR)/{objname}.o '.format(objname=os.path.splitext(s)[0])

def gen_windows_make_obj(out, src):
    out = out.split('\\')[-1]
    return WINDOWS_CCOBJ.format(make_obj=out.strip(), make_src=src)

def gen_posix_make_obj(out, src):
    out = out.split('/')[-1]
    return POSIX_CCOBJ.format(make_obj=out.strip(), make_src=src)

def gen_makefile(platform='linux', target='a.out', outdir='build', sources=[]):
    mname = sys.modules['__main__']
    gen_obj = getattr(mname, 'gen_{pt}_obj'.format(pt=platform), gen_posix_obj)
    gen_make_obj = getattr(mname, 'gen_{pt}_make_obj'.format(pt=platform), gen_posix_make_obj)

    objs_list = []
    make_objs_list = []
    for s in sources:
        objstr = gen_obj(s)
        objs_list.append(objstr)
        make_objs_list.append(gen_make_obj(objstr, s))
    make_dict = dict(
        outdir = outdir,
        target = target,
        objs = ''.join(objs_list)[:-1],
        make_objs = ''.join(make_objs_list)[:-1]
    )
    ext_dict = getattr(mname, '{pt}_MKDIR'.format(pt=platform.upper()), None)
    if ext_dict:
        make_dict.update(ext_dict)

    makefile_temp = getattr(mname, '{pt}_MAKEFILE'.format(pt=platform.upper()), 'POSIX_MAKEFILE')
    with do_open('Makefile', 'w', encoding='utf-8') as fp:
        fp.write(makefile_temp.format(**make_dict))

def gen_outdir(outdir='build'):
    safe_mkdir(outdir)
    safe_mkdir('{outdir}/bin'.format(outdir=outdir))
    safe_mkdir('{outdir}/obj'.format(outdir=outdir))

def get_options():
    parser = argparse.ArgumentParser(description='Devil building tool')
    parser.add_argument('option', help='build|rebuild|clean|remove the project')
    args = parser.parse_args()
    return args.option

def do_remove():
    safe_rmdir(OUTDIR)
    safe_rm('Makefile')

def main():
    option = get_options()
    if option == 'remove':
        do_remove()
        return

    platform = get_platform()
    gen_outdir(outdir=OUTDIR)
    gen_makefile(platform=platform, target=TARGET, outdir=OUTDIR, sources=get_all_sources(dirs=DIRS))

    make = platform == 'windows' and 'nmake' or 'make'
    make_flags = {
        'build': '',
        'rebuild': 'rebuild',
        'clean': 'clean'
    }
    flag = make_flags.get(option, None)
    if flag is None:
        do_remove()
    else:
        subprocess.check_call('{make} {flag}'.format(make=make, flag=flag), shell=True)

if __name__ == '__main__':
    main()
