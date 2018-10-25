#undef UNICODE
#undef _UNICODE
#include<Windows.h>
#include<stdio.h>
void wrong()
{
	printf("Overwrites data in a file with the text \"Smile, Laugh, and Be Happy!\"\n\nUsage:\n\toverwrite [file_name] [q(optional)]\n\n\
\tfile_name\t- must be relative or absolute path\n\tq\t\t- quiet mode. processing without confirmation\n");
}
void write(char *name)
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	HANDLE hFile;
	DWORD dwTmp = 0;
	LARGE_INTEGER li;
	BOOL b = FALSE;
	char text[] = "Smile, Laugh, and Be Happy! ";//28
	char *buf;
	hFile = CreateFile(name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("FAILED;\nSystem error code: %u\nFile: %s\n", GetLastError(), name);
		return;
	}
	if (!GetFileSizeEx(hFile, &li))
	{
		printf("FAILED;\nSystem error code: %u\n", GetLastError());
		CloseHandle(hFile);
		return;
	}
	buf = malloc(29360128);
	for (DWORD c = 0; c < 1048576; c++)
	{
		for (UCHAR c2 = 0; c2 < 28; c2++)
		{
			buf[dwTmp] = text[c2];
			dwTmp++;
		}
	}
	while (li.QuadPart > 0)
	{
		if (!WriteFile(hFile, buf, li.QuadPart < 29360128 ? li.QuadPart : 29360128, &dwTmp, NULL))
		{
			printf("\nwriting failed with: %u\n", GetLastError());
			b = TRUE;
			break;
		}
		li.QuadPart -= dwTmp;
		printf("%u GB_", li.QuadPart / 1073741824);
		printf("%u MB left.\r", li.QuadPart / 1048576 % 1024);
	}
	if (!b)
		printf("\nSUCCESS.");
	free(buf);
	CloseHandle(hFile);
	cci.bVisible = TRUE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}
BOOL confirm(char *buf)
{
	char c;
	int tmp;
	again:
	printf("\nFile: %s\ndo you confirm erasing? (y or n)_", buf);
	c = getchar();
	if (tmp = getchar() != '\n')
	{
		printf("'y' for confirmation and 'n' for cancellation\n");
		c = 'a';
		while (tmp = getchar() != '\n');
	}
	if (c == 'y')
		return TRUE;
	else
		if (c == 'n')
			return FALSE;
		else
			goto again;
}
int main(int argc, char **argv)
{
	char buf[MAX_PATH];
	if (argc == 2)
		if (argv[1][1] == ':')
			if (argv[1][2] != '\\')
				wrong();
			else
				if (confirm(argv[1]))
					write(argv[1]);
				else;
		else
			if (!GetCurrentDirectory(MAX_PATH, buf))
				printf("System error code: %u\n", GetLastError());
			else
				if (strcat_s(buf, MAX_PATH, "\\"))
					printf("the path is larger than 255 characters\n");
				else
					if (strcat_s(buf, MAX_PATH, argv[1]))
						printf("the path is larger than 255 characters\n");
					else
						if (confirm(buf))
							write(buf);
						else;
	else
		if (argc == 3 && argv[2][2] == 0)
			if (argv[2][0] == '/'&&argv[2][1] == 'q')
				if (argv[1][1] == ':')
					if (argv[1][2] != '\\')
						wrong();
					else
						write(argv[1]);
				else
					if (!GetCurrentDirectory(MAX_PATH, buf))
						printf("System error code: %u\n", GetLastError());
					else
						if (strcat_s(buf, MAX_PATH, "\\"))
							printf("the path is larger than 255 characters\n");
						else
							if (strcat_s(buf, MAX_PATH, argv[1]))
								printf("the path is larger than 255 characters\n");
							else
								write(buf);
			else
				wrong();
		else
			wrong();
	return 0;
}