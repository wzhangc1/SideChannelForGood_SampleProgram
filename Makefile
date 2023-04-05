FILE1 = program
FILE2 = verifier
# FILE3 = generate_stream
# FILE4 = compare_streams

CC	= gcc
CFLAGS = -O0

# CFLAGS = -pthread -O0	-msse3	-mavx -mavx2

all: ${FILE1} ${FILE2}

${FILE1}: ${FILE1}.c
	$(CC) ${FILE1}.c $(CFLAGS) -o ${FILE1} -lssl -lcrypto

${FILE2}: ${FILE2}.c
	$(CC) ${FILE2}.c $(CFLAGS) -o ${FILE2}
# ${FILE2}: ${FILE2}.c ${FILE2}.asm
# 	$(CC) ${FILE2}.c $(CFLAGS) -o ${FILE2}
# ${FILE2}.asm: ${FILE2}.c
# 	$(CC)  $(CFLAGS) -S -o ${FILE2}.asm ${FILE2}.c
	
# ${FILE3}: ${FILE3}.c
# 	$(CC) ${FILE3}.c $(CFLAGS) -o ${FILE3}
	
# ${FILE4}: ${FILE4}.c
# 	$(CC) ${FILE4}.c $(CFLAGS) -o ${FILE4}

clean: 
	rm -f ${FILE1} ${FILE2}