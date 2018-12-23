

#include "Device.h"
#include "Disc.h"



void PrintHelp()
{
	printf("HELP\n");
	printf("create new disc: disc_name -n size\n");
	printf("delete disc: disc_name -r\n");
	printf("copy file to disc: disc_name -v file_name\n");
	printf("copy file from disc: disc_name -c file_name\n");
	printf("delete file from disc: disc_name -d file_name\n");
	printf("list disc dir: -l\n");
	printf("print disc map: disc_name -m\n");
}

int DeviceIoControl(int argc, char** argv)
{
	if (argc < 2 || !strcmp(argv[1], "-h"))
	{
		PrintHelp();
		return 0;
	}

	switch (argv[2][1])
	{
	case 'n':
		if (argc < 4)
		{
			printf("ERROR: -n require additional arg: size\n");
			return -1;
		}
		disc = NewDiscInstance(atoi(argv[3]));
		if (CreateDiscFile(argv[1]))
		{
			printf("successfully created disc %s\n", argv[1]);
		}
		else
		{
			printf("cannot create disc %s\n", argv[1]);
		}
		CloseDiscFile();
		break;
	case 'l':
		OpenDiscFile(argv[1]);
		ListDisc();
		CloseDiscFile();
		break;
	case 'd':
		if (argc < 4)
		{
			printf("ERROR: -d require additional arg: filename\n");
			return -1;
		}
		OpenDiscFile(argv[1]);
		DeleteFileFromDisc(argv[3]);
		CloseDiscFile();
		break;
	case 'r':
		DeleteDisc(argv[1]);
		break;
	case 'v':
		if (argc < 4)
		{
			printf("ERROR: -c require additional arg: filename\n");
			return -1;
		}
		OpenDiscFile(argv[1]);
		CopyToDisc(argv[3]);
		CloseDiscFile();
		break;
	case 'c':
		if (argc < 4)
		{
			printf("ERROR: -c require additional arg: filename\n");
			return -1;
		}
		OpenDiscFile(argv[1]);
		CopyFromDisc(argv[3]);
		CloseDiscFile();
		break;
	case 'm':
		OpenDiscFile(argv[1]);
		PrintDiscMap();
		CloseDiscFile();
		break;
	}
	return 0;
}
