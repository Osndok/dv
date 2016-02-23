/*
 * dv - encoder/decoder for "Dvorak encoding"
 * 
 * https://en.wikipedia.org/wiki/Dvorak_encoding
 *
 * Given a plain-text (human-readable) string, "encoding" that string will result in a string
 * that "is easy for a dvorak typist to type on a qwerty keyboard" (by attempting to type the
 * plaintext).
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef char (codec)(char);

void usage(char* execName);
int for_each_char(int argc, char* argv[], codec* codec);

char dv_encode_char(char c);
char dv_decode_char(char c);

int main(int argc, char*argv[])
{
	if (argc==2 || argc==3)
	{
		char* modeFlag=argv[1];
		bool encode;

		if (strcmp(modeFlag, "-e")==0)
		{
			return for_each_char(argc, argv, dv_encode_char);
		}
		else
		if (strcmp(modeFlag, "-d")==0)
		{
			return for_each_char(argc, argv, dv_decode_char);
		}
		else
		if (modeFlag[0]=='-')
		{
			fprintf(stderr, "unknown mode flag: '%s'\n", modeFlag);
			fflush(stderr);
			return 2;
			
		}
		else
		{
			fprintf(stderr, "missing mode flag <-e|-d>, got: '%s'\n", modeFlag);
			fflush(stderr);
			return 3;
		}
	}
	else
	{
		usage(argv[0]);
		return 4;
	}
}

void usage(char* execName)
{
	fprintf(stderr, "usage: %s <-e|-d> [file]\n", execName);
	fflush(stderr);
}

int for_each_char(int argc, char* argv[], codec* codec)
{
	FILE* out=stdout;
	FILE* in;
	{
		if (argc==2 || strcmp(argv[2],"-")==0)
		{
			in=stdin;
		}
		else
		{
			in=fopen(argv[2], "r");

			if (in==NULL)
			{
				perror(argv[2]);
				return 5;
			}
		}
	}

	char c=fgetc(in);

	while (c!=EOF)
	{
		c=codec(c);
		fputc(c, out);
		c=fgetc(in);
	}

	fclose(in);
	fclose(out);
	return 0;
}

char *Qwerty="][abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ;',./_+{}:\"<>?=-";
char *Dvorak="=/axje.uidchtnmbrl'poygk,qf;AXJE>UIDCHTNMBRL\"POYGK<QF:s-wvz{}?+S_WVZ][";

char dv_encode_char(char c)
{
	char* haystack=Dvorak;
	char* p=strchr(haystack, c);

	if (p)
	{
		int i=p-haystack;
		return Qwerty[i];
	}
	else
	{
		return c;
	}
}

char dv_decode_char(char c)
{
	char* haystack=Qwerty;
	char* p=strchr(haystack, c);

	if (p)
	{
		int i=p-haystack;
		return Dvorak[i];
	}
	else
	{
		return c;
	}
}

