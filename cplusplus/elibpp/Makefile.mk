# Copyright (c) 2013 ASMlover. All rights reserved.
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

OUT	= elibpp.exe 
RM	= rm 
RD	= rd /s /q
CC	= cl -c -nologo 
MT	= mt -nologo
LINK	= link -nologo
CFLAGS	= -O2 -W3 -MD -GS -Zi -Fd"vc.pdb" -EHsc -DNDEBUG\
	-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS
LDFLAGS	= -INCREMENTAL -DEBUG -PDB:$(OUT).pdb -manifest\
	-manifestfile:$(OUT).manifest -manifestuac:no winmm.lib ws2_32.lib
OBJS	= el_test_main.obj el_test_mutex.obj el_test_spinlock.obj\
	el_test_condition.obj el_test_thread.obj el_test_thread_pool.obj\
	el_test_allocator.obj el_test_buffer.obj el_test_file.obj\
	el_test_time.obj el_test_logging.obj\
	\
	el_win_condition.obj el_win_io.obj el_win_file.obj el_win_time.obj\
	el_win_net.obj el_win_socket.obj\
	\
	el_allocator.obj el_thread_pool.obj el_buffer.obj el_time.obj\
	el_logging.obj el_net.obj el_net_buffer.obj el_address.obj el_socket.obj



all: $(OUT)

rebuild: clean all 

clean:
	$(RM) $(OUT) $(OBJS) *.pdb *.manifest *.ilk

clean_all:
	$(RM) $(OUT) $(OBJS) *.pdb *.manifest *.ilk *.log 

clean_log:
	if exist logging $(RD) logging




$(OUT): $(OBJS)
	$(LINK) -out:$(OUT) $(OBJS) $(LDFLAGS)
	$(MT) -manifest $(OUT).manifest -outputresource:$(OUT);1

.cc.obj:
	$(CC) $(CFLAGS) $<

{.\win}.cc{}.obj:
	$(CC) $(CFLAGS) $<

{.\test}.cc{}.obj:
	$(CC) $(CFLAGS) $<