#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
typedef unsigned int UINT;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
#define UINT_SIZE   sizeof(UINT)
#define UCHAR_SIZE  sizeof(UCHAR)
#define USHORT_SIZE sizeof(USHORT)

typedef struct node {
	long w;
	short p, l, r;
} htnode, *htnp;
typedef struct huffNode {
	UCHAR len;
	UCHAR *value;
} hNode;

long countFrequency(FILE *in, long frequency[]) {
	int i, read_len;
	UCHAR buf[256];
	long filesize;

	for (i = 0; i < 256; i++) {
		frequency[i] = 0;
	}
	fseek(in, 0L, SEEK_SET);
	read_len = 256;
	while (read_len == 256) {
		read_len = fread(buf, 1, 256, in);
		for (i = 0; i < read_len; i++) {
			frequency[*(buf + i)]++;
		}
	}
	for (i = 0, filesize = 0; i < 256; i++) {
		filesize += frequency[i];
	}
	return filesize;
}

int huffSearch(htnp ht, int n) {
	int i, x;
	for (x = 0; x < n; x++) {
		if (ht[x].p == -1)
			break;
	}
	for (i = x; i < n; i++) {
		if (ht[i].p == -1 && ht[i].w < ht[x].w) {
			x = i;
		}
	}
	ht[x].p = -2;
	return x;
}

void initHuffTree(long w[], int n, htnode ht[]) {
	int m, i, s1, s2;
	m = 2 * n - 1;

	for (i = 0; i < n; i++) {
		ht[i].w = w[i], ht[i].p = ht[i].l = ht[i].r = -1;
	}
	for (; i < m; i++) {
		ht[i].w = ht[i].p = ht[i].l = ht[i].r = -1;
	}
	for (i = n; i < m; i++) {
		s1 = huffSearch(ht, i);
		s2 = huffSearch(ht, i);
		ht[s1].p = ht[s2].p = i;
		ht[i].l = s1, ht[i].r = s2;
		ht[i].w = ht[s1].w + ht[s2].w;
	}
}

void encodeHuffTree(htnp htp, int n, hNode hc[]) {
	int i, j, p, codelen;
	UCHAR *code = (UCHAR*) malloc(n * UCHAR_SIZE);

	for (i = 0; i < n; i++) {
		for (p = i, codelen = 0; p != 2 * n - 2; p = htp[p].p, codelen++) {
			code[codelen] = (htp[htp[p].p].l == p ? 0 : 1);
		}
		hc[i].value = (UCHAR *) malloc((codelen) * UCHAR_SIZE);
		hc[i].len = codelen;
		for (j = 0; j < codelen; j++) {
			hc[i].value[j] = code[codelen - j - 1];
		}
	}
	free(code);
}

UCHAR char2bit(const UCHAR chars[8]) {
	int i;
	UCHAR bits = 0;

	bits |= chars[0];
	for (i = 1; i < 8; ++i) {
		bits <<= 1;
		bits |= chars[i];
	}
	return bits;
}

void huffZipFile(FILE* in, FILE* out, htnp ht, hNode hc[], long source_filesize) {
	UINT i, read_counter, write_counter;
	char head = 13;
	UCHAR write_char_counter, code_char_counter, copy_char_counter,
			read_buf[256], write_buf[256], write_chars[8];
	hNode *cur_hNode;

	fseek(in, 0L, SEEK_SET);
	fseek(out, 0L, SEEK_SET);
	putc(head, out);

	fwrite(&source_filesize, sizeof(long), 1, out);

	for (i = 256; i < 256 * 2 - 1; i++) {
		fwrite(&(ht[i].l), sizeof(ht[i].l), 1, out);
		fwrite(&(ht[i].r), sizeof(ht[i].r), 1, out);
	}

	write_counter = write_char_counter = 0;
	read_counter = 256;

	while (read_counter == 256) {
		read_counter = fread(read_buf, 1, 256, in);

		for (i = 0; i < read_counter; i++) {
			cur_hNode = &hc[read_buf[i]];
			code_char_counter = 0;

			while (code_char_counter != cur_hNode->len) {
				copy_char_counter = (
						8 - write_char_counter
								> cur_hNode->len - code_char_counter ?
								cur_hNode->len - code_char_counter :
								8 - write_char_counter);

				memcpy(write_chars + write_char_counter,
						cur_hNode->value + code_char_counter,
						copy_char_counter);
				write_char_counter += copy_char_counter;
				code_char_counter += copy_char_counter;

				if (write_char_counter == 8) {
					write_char_counter = 0;

					write_buf[write_counter++] = char2bit(write_chars);
					if (write_counter == 256) {
						fwrite(write_buf, 1, 256, out);
						write_counter = 0;
					}
				}
			}
		}

	}
	fwrite(write_buf, 1, write_counter, out);

	if (write_char_counter != 0) {
		write_char_counter = char2bit(write_chars);
		fwrite(&write_char_counter, 1, 1, out);
	}
}

void huff(FILE* in, FILE* out) {
	int i;
	float compress_rate;
	clock_t begin, duration;
	begin = clock();

	hNode hc[256];
	htnode ht[256 * 2 - 1];
	long frequency[256], source_filesize, obj_filesize = 0;
	source_filesize = countFrequency(in, frequency);
	initHuffTree(frequency, 256, ht);
	encodeHuffTree(ht, 256, hc);

	for (i = 0; i < 256; i++)
		obj_filesize += frequency[i] * hc[i].len;
	obj_filesize =
			obj_filesize % 8 == 0 ? obj_filesize / 8 : obj_filesize / 8 + 1;
	for (i = 0; i < 256 - 1; i++)
		obj_filesize += 2 * sizeof(short);
	obj_filesize += sizeof(long);
	obj_filesize += UINT_SIZE;
	compress_rate = 1 - (float) obj_filesize / source_filesize;
	huffZipFile(in, out, ht, hc, source_filesize);

	for (i = 0; i < 256; i++) {
		free(hc[i].value);
	}

	//printf("size %ld \n", source_filesize);
	//printf("HUFFsiz:%ld\n", obj_filesize);
	//printf("%%save:%.2lf%%\n", compress_rate * 100);
	fclose(in);
	fclose(out);
}

void huffUnzipFile(FILE* in, FILE* out, short mini_ht[][2], long bits_pos,
		long obj_filesize) {
	long cur_size;
	UCHAR read_buf[256], write_buf[256], convert_bit;
	UINT read_counter, write_counter, cur_pos;

	fseek(in, bits_pos, SEEK_SET);
	fseek(out, 0L, SEEK_SET);
	read_counter = 256 - 1;
	cur_size = write_counter = 0;
	cur_pos = 256 * 2 - 2;
	while (cur_size != obj_filesize) {
		if (++read_counter == 256) {
			fread(read_buf, 1, 256, in);
			read_counter = 0;
		}
		for (convert_bit = 128; convert_bit != 0; convert_bit >>= 1) {
			cur_pos = (
					(read_buf[read_counter] & convert_bit) == 0 ?
							mini_ht[cur_pos][0] : mini_ht[cur_pos][1]);
			if (cur_pos < 256) {
				write_buf[write_counter] = (UCHAR) cur_pos;
				if (++write_counter == 256) {
					fwrite(write_buf, 1, 256, out);
					write_counter = 0;
				}
				cur_pos = 256 * 2 - 2;
				if (++cur_size == obj_filesize) {
					break;
				}
			}
		}
	}
	fwrite(write_buf, 1, write_counter, out);

	fclose(in);
	fclose(out);
}

void miniHuffTree(FILE* in, short mini_ht[][2]) {
	int i;
	for (i = 0; i < 256; i++) {
		mini_ht[i][0] = mini_ht[i][1] = -1;
	}
	fread(mini_ht[i], sizeof(short), 2 * (256 - 1), in);
}

void unhuff(FILE* in, FILE* out) {
	short mini_ht[256 * 2 - 1][2];
	long obj_filesize;
	fread(&obj_filesize, sizeof(long), 1, in);
	miniHuffTree(in, mini_ht);
	huffUnzipFile(in, out, mini_ht, ftell(in), obj_filesize);
}

