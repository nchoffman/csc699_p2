all: cpan specCompress

cpan: cpan.c header.c anread.c cpan
	gcc -O cpan.c header.c anread.c -o cpan -lm

specCompress: specCompress.c header.c anread.c getMaxError.c specCompress
	gcc -O -g specCompress.c header.c anread.c getMaxError.c -o specCompress -lm

clean:
	rm cpan specCompress
