#include <stdio.h>
#include <stdlib.h>

#define N1         2048
#define F           16

unsigned long int tSize = 0, cSize = 0;
unsigned char textBuf[N1 + F - 1];
int matchPosition, matchLength, lson[N1 + 1], rs[N1 + 257], ds[N1 + 1];

/**
 * Init the tree for lz1
 */
void initLz1Tree(void) {
	int i;

	for (i = N1 + 1; i <= N1 + 256; i++)
		rs[i] = N1;
	for (i = 0; i < N1; i++)
		ds[i] = N1;
}

/**
 * insert node to the tree for lz1
 */
void insertLz1Node(int r) {
	int i, p, cmp;
	unsigned char *key;

	cmp = 1;
	key = &textBuf[r];
	p = N1 + 1 + key[0];
	rs[r] = lson[r] = N1;
	matchLength = 0;
	for (;;) {
		if (cmp >= 0) {
			if (rs[p] != N1)
				p = rs[p];
			else {
				rs[p] = r;
				ds[r] = p;
				return;
			}
		} else {
			if (lson[p] != N1)
				p = lson[p];
			else {
				lson[p] = r;
				ds[r] = p;
				return;
			}
		}
		for (i = 1; i < F; i++)
			if ((cmp = key[i] - textBuf[p + i]) != 0)
				break;
		if (i > matchLength) {
			matchPosition = p;
			if ((matchLength = i) >= F)
				break;
		}
	}
	ds[r] = ds[p];
	lson[r] = lson[p];
	rs[r] = rs[p];
	ds[lson[p]] = r;
	ds[rs[p]] = r;
	if (rs[ds[p]] == p)
		rs[ds[p]] = r;
	else
		lson[ds[p]] = r;
	ds[p] = N1;
}

/**
 *
 */
void deleteLz1Node(int p) {
	int q;

	/* not in tree */
	if (ds[p] == N1)
		return;

	if (rs[p] == N1)
		q = lson[p];
	else if (lson[p] == N1)
		q = rs[p];
	else {
		q = lson[p];
		if (rs[q] != N1) {
			do {
				q = rs[q];
			} while (rs[q] != N1);
			rs[ds[q]] = lson[q];
			ds[lson[q]] = ds[q];
			lson[q] = lson[p];
			ds[lson[p]] = q;
		}
		rs[q] = rs[p];
		ds[rs[p]] = q;
	}
	ds[q] = ds[p];
	if (rs[ds[p]] == p)
		rs[ds[p]] = q;
	else
		lson[ds[p]] = q;
	ds[p] = N1;
}

/**
 * applies Lempel-Ziv variation 1 to the file
 */
void lz1(FILE* infile, FILE* outfile) {
	int i, c, len, r, s, last_matchLength, code_buf_ptr;
	unsigned char code_buf[17], mask;
	float compress_rate;

	char head;
	head = 17;
	putc(head, outfile);

	initLz1Tree();
	code_buf[0] = 0;
	code_buf_ptr = mask = 1;
	s = 0;
	r = N1 - F;
	for (i = s; i < r; i++)
		textBuf[i] = ' ';
	for (len = 0; len < F && (c = getc(infile)) != EOF; len++)
		textBuf[r + len] = c;
	if ((tSize = len) == 0)
		return;
	for (i = 1; i <= F; i++)
		insertLz1Node(r - i);
	insertLz1Node(r);
	do {
		if (matchLength > len)
			matchLength = len;
		if (matchLength <= 2) {
			matchLength = 1;
			code_buf[0] |= mask;
			code_buf[code_buf_ptr++] = textBuf[r];
		} else {
			code_buf[code_buf_ptr++] = (unsigned char) matchPosition;
			code_buf[code_buf_ptr++] = (unsigned char) (((matchPosition >> 4)
					& 0xf0) | (matchLength - (2 + 1)));
		}
		if ((mask <<= 1) == 0) {
			for (i = 0; i < code_buf_ptr; i++)
				putc(code_buf[i], outfile);
			cSize += code_buf_ptr;
			code_buf[0] = 0;
			code_buf_ptr = mask = 1;
		}
		last_matchLength = matchLength;
		for (i = 0; i < last_matchLength && (c = getc(infile)) != EOF; i++) {
			deleteLz1Node(s);
			textBuf[s] = c;
			if (s < F - 1)
				textBuf[s + N1] = c;
			s = (s + 1) & (N1 - 1);
			r = (r + 1) & (N1 - 1);

			insertLz1Node(r);
		}

		tSize += i;
		while (i++ < last_matchLength) {
			deleteLz1Node(s);
			s = (s + 1) & (N1 - 1);
			r = (r + 1) & (N1 - 1);
			if (--len)
				insertLz1Node(r);
		}
	} while (len > 0);
	if (code_buf_ptr > 1) {
		for (i = 0; i < code_buf_ptr; i++)
			putc(code_buf[i], outfile);
		cSize += code_buf_ptr;
	}

	compress_rate = 1 - (float)cSize / tSize;
	//printf("size %ld \n", tSize);
	//printf("LZ1siz:%ld\n", cSize);
	//printf("%%save:%.2lf%%\n", compress_rate * 100);

	fclose(infile);
	fclose(outfile);
}

/**
 * uncompresses the file with lz1
 */
void unlz1(FILE* infile, FILE* outfile) {
	int i, j, k, r, c;
	unsigned int flags;

	for (i = 0; i < N1 - F; i++)
		textBuf[i] = 0x0;
	r = N1 - F;
	flags = 0;
	for (;;) {
		if (((flags >>= 1) & 256) == 0) {
			if ((c = getc(infile)) == EOF)
				break;
			flags = c | 0xff00;
		}
		if (flags & 1) {
			if ((c = getc(infile)) == EOF)
				break;
			putc(c, outfile);
			textBuf[r++] = c;
			r &= (N1 - 1);
		} else {
			if ((i = getc(infile)) == EOF)
				break;
			if ((j = getc(infile)) == EOF)
				break;
			i |= ((j & 0xf0) << 4);
			j = (j & 0x0f) + 2;
			for (k = 0; k <= j; k++) {
				c = textBuf[(i + k) & (N1 - 1)];
				putc(c, outfile);
				textBuf[r++] = c;
				r &= (N1 - 1);
			}
		}
	}

	fclose(infile);
	fclose(outfile);
}
