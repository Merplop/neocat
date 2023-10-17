#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
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
	char optionFlags[] = {'h', 'd', 'u'};
	int setFlags[] = {0,0,0};
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
				for (j=0;j<3;j++) {
					if (ptr[1] == optionFlags[j]) {
						setFlags[j] = 1;
					}
				}
			}
		}
		if (setFlags[0] == 1) {
			printf("Neocat - improved file concatenation and standard output printing utility, Miro Haapalainen\nUsage - cat [OPTION] [FILE]\n\n-h    display this and exit\n-b    decode hex bytes from binary file\n-u    use unbuffered I/O rather than buffered\n");
			return 0;
		}
		if (setFlags[2] == 1) {
			printf("got here");
			cp = (char*)calloc(SIZE, sizeof(char));
			fd = open(argv[1], O_RDONLY);
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
			printf("Could not open file");
			return 0;
		}
		c = fgetc(fp);
		while (c != EOF) {
			printf("%c", c);
			c = fgetc(fp);
		}
		fclose(fp);
		return 0;
	}
}


