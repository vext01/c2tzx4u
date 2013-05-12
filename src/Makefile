all: c2tzx

C2TZX_SRCS=C2TZX.cpp stdafx.cpp

c2tzx: ${C2TZX_SRCS} stdafx.h
	${CXX} ${CFLAGS} ${C2TZX_SRCS} -o $@

.PHONY: clean
clean:
	rm -f c2tzx
