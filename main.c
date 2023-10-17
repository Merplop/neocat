#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#define SIZE 4096

int main(int argc, char* argv[]) {

	size_t bytesRead;
	char buffer[SIZE];
	FILE *fp;
	char* cp;
	char c;
	char *ptr;
	int fd, sz;
	int i, j;
	int decodeCount = 0;
	int set_pos;
	long size;
	char optionFlags[] = {'h', 'b', 'u', 's', 'd'};
	int setFlags[] = {0,0,0,0,0};
	int optionValid;
	DIR *d;
	struct dirent *dir;

	if (argc == 1) {
		while(1) {
			bytesRead = read(STDIN_FILENO, buffer, SIZE);
			if (bytesRead < 0) {
				perror("Read error");
				return 1;
			}
			if (bytesRead == 0) {
				break;
			}
			if (write(STDOUT_FILENO, buffer, bytesRead)) {
			} else { perror("Write error");}
		} 
	} else {
		for (i=0;i<argc;i++) {
			ptr = argv[i];
			if (ptr[0] == '-') {
				optionValid = 0;
				for (j=0;j<5;j++) {
					if (ptr[1] == optionFlags[j]) {
						setFlags[j] = 1;
						optionValid = 1;
						break;
					}
				}
				if (optionValid == 0) {
					printf("Error - invalid option '");
					printf(argv[i]);
					printf("'\nType 'ncat -h' to view available options\n");
					return 1;
				}
			}
		}
		if (setFlags[0] == 1) {
			printf("Neocat - improved file concatenation and standard output printing utility, Miro Haapalainen\nUsage - cat [OPTION] [FILE]\n\n-h    display this and exit\n-b    decode hex bytes from binary file\n-u    use unbuffered I/O rather than buffered\n-s    display size of file\n-d    display contents of working directory\n");
			return 0;
		}
		if (setFlags[4] == 1) {
			d = opendir(".");
			if (d) {
				while ((dir = readdir(d)) != NULL) {
					if (dir->d_type == DT_REG) {
						fp =fopen(dir->d_name, "r");
						set_pos = fseek(fp, 0, SEEK_END);
						size = ftell(fp);
						printf("\033[1;35m");
						printf("%s", dir->d_name);
						printf("\033[0m");
						printf("   %lu bytes\n", size);
						fclose(fp);

					} else if (dir->d_type == DT_DIR) {
						printf("\033[0;35m");
						printf("%s\n", dir->d_name);
						printf("\033[0m");
					} else {
						printf("%s\n", dir->d_name);
					}
				}
				closedir(d);
			}
			return 0;
		}
		if (setFlags[2] == 1) {
			cp = (char*)calloc(SIZE, sizeof(char));
			fd = open(argv[argc-1], O_RDONLY);
			if (fd < 0) {
				perror("Error opening file");
				exit(1);
			}
			sz = read(fd, cp, SIZE);
			cp[sz] = '\0';
			printf("%s\n", cp);
		       return 0;	
		}
		fp = fopen(argv[argc-1], "r");
		if (fp == NULL) {
			perror("Error opening file");
			exit(1);
		}
		if (setFlags[3] == 1) {
			set_pos = fseek(fp, 0, SEEK_END);
			size = ftell(fp);
			printf("\033[1m");
			printf(argv[argc-1]);
			printf("\033[1;35m");
			printf(" - %lu bytes\n\n", size);
			printf("\033[0m");
		        set_pos = fseek(fp, 0, SEEK_SET);	
		}
		c = fgetc(fp);
		while (c != EOF) {
			if (setFlags[1] == 1) {
				printf("%02X:", c); 
				decodeCount++;
				if (decodeCount%16==0) {printf("\n");}
			} else {
				printf("%c", c);
			} 
			c = fgetc(fp);
		}
		printf("\n");
		fclose(fp);
		return 0;
	}
}


