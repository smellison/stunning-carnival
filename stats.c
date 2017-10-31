#include "stats.h"

#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

#include <openssl/sha.h>
#include <time.h>

char *home_team;
char *away_team;
char home_score[4];
char away_score[4];

char *complete_response;

void error(const char *msg) { perror(msg); exit(0); }

void setHash( char outputBuffer[65] ) {
	/* the three elements that the hash is comprosed of */
	char *key = "8jfq98vqqbe7fdhc33tdqy7w";
	char *secret = "Jy33CCupgR";
	char timestamp[15];
	
	/* getting UTC timestamp in seconds and convert to char array */
	time_t now;
	time(&now);
    //printf("LOG: %ld\n", now);
    sprintf(timestamp, "%ld", now);
    //printf("LOG: %s\n", timestamp);
	
	/* calculating size of string to be hashed */
	int string_size = 0;
	string_size += strlen( key );
	string_size += strlen( secret );
	string_size += strlen( timestamp );
	
	/* creating string to be hashed */
	char *string = malloc(string_size);
	strcpy(string, key);
	strcat(string, secret);
	strcat(string, timestamp);
	
	/* calculating SHA256 hash */
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, string, strlen(string));
	SHA256_Final(hash, &sha256);
	
	/* converting hash to char array */
	int i = 0;
	for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
	}
	outputBuffer[64] = 0;
	
	free( string );
	
	//printf("LOG: %s\n", outputBuffer);
}

void modifyDone( char *response, int *done ) {
	int x = 0;
	int y = 0;
	for (int i = 0; response[i] != '\0'; i++) {
		// parse {}
		if ( *done == -1 ) {
			if ( response[i] == '{' ) {
				*done = 1;
				x = i;
			}
		}
		else {
			if ( response[i] == '{' ) {
				*done += 1;
			}
			else if ( response[i] == '}' ) {
				*done -= 1;
			}
		}
	}
	
	if (x != 0) {
		y = strlen(response);
		//printf("X: %d, Y: %d\n",x,y);
		char *temp = malloc( y - x );
		strncpy(temp, response+x, y-x);
		complete_response = temp;
		//printf("COMPLETE RESPONSE: %s\n\n",complete_response);
	}
}

void parse() {
	char *team_one = strstr(complete_response,"nickname");
	team_one = strstr(team_one,"abbreviation");
	char *score_one = strstr(team_one,"linescoreTotals");
	score_one = strstr(score_one,"runs");
	char *team_two = strstr(score_one,"nickname");
	team_two = strstr(team_two,"abbreviation");
	char *score_two = strstr(team_two,"linescoreTotals");
	score_two = strstr(score_two,"runs");
	
	team_one += 15;
	team_one[3] = 0;
	home_team = team_one;
	
	team_two += 15;
	team_two[3] = 0;
	away_team = team_two;
	
	score_one += 6;
	for (int i = 0; i < strlen(score_one); i++) {
		if (score_one[i] == ',') {
			score_one[i] = 0;
			break;
		}
	}
	
	score_two += 6;
	for (int i = 0; i < strlen(score_two); i++) {
		if (score_two[i] == ',') {
			score_two[i] = 0;
			break;
		}
	}
	
	if (strlen(score_one) == 1) {
		home_score[0] = '0';
		home_score[1] = '0';
		home_score[2] = score_one[0];
	}
	else {
		home_score[0] = '0';
		home_score[1] = score_one[0];
		home_score[2] = score_one[1];
	}
	
	if (strlen(score_two) == 1) {
		away_score[0] = '0';
		away_score[1] = '0';
		away_score[2] = score_two[0];
	}
	else {
		away_score[0] = '0';
		away_score[1] = score_two[0];
		away_score[2] = score_two[1];
	}
}

char *make_request(){
	complete_response = "";
	home_team = "XXX";
	away_team = "XXX";

	/* declaration of objects to be used later */
	struct hostent *server;
	struct sockaddr_in serv_addr;
	int sockfd, bytes, sent, received, total, message_size, done;
	char *message, response[1025];
	
    /* request settings and location and parameters */
    int portno = 80;
    char *host = "api.stats.com";
	char *method = "GET";
	char *path = "http://api.stats.com/v1/stats/baseball/mlb/scores/";
	char *query = "?linescore=true&accept=json&api_key=8jfq98vqqbe7fdhc33tdqy7w&sig=";
	char hash[65];
	setHash(hash);
	
    /* determining size of the message being sent */
    message_size = 0;
	message_size += strlen( "%s %s%s HTTP/1.1\r\n" );
	message_size += strlen( method );
	message_size += strlen( path );
	message_size += strlen( query );
	message_size += strlen( hash );

    /* allocate space for the message */
    message = malloc( message_size );

    /* fill in the parameters */
	sprintf( message,
		"%s %s%s%s HTTP/1.1\r\n",
		method,
		path,
		query,
		hash );
	strcat(message,"\r\n");

    /* LOG - what is being sent? */
    //printf( "Request:\n%s\n", message );

    /* create the socket */
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sockfd < 0 ) {
		error( "ERROR opening socket" );
	}

    /* lookup the ip address */
    server = gethostbyname( host );
    if ( server == NULL ) {
		error( "ERROR, no such host" );
	}

    /* fill in the structure */
    memset( &serv_addr, 0, sizeof( serv_addr ) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( portno );
    memcpy( &serv_addr.sin_addr.s_addr, server->h_addr, server->h_length );

    /* connect the socket */
    if ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0 ) {
        error("ERROR connecting");
	}

    /* send the request */
    total = strlen( message );
    sent = 0;
    do {
        bytes = write( sockfd, message+sent, total-sent );
        if ( bytes < 0 ) {
            error("ERROR writing message to socket");
		}
        if ( bytes == 0 ) {
            break;
		}
        sent += bytes;
    } while ( sent < total );

    /* receive and parse the response */
	done = -1;
    do {
    	memset( response, 0, sizeof( response ) );
    	bytes = recv( sockfd, response, 1024, 0 );
		//printf( "DONE: %d\n", done);
		if ( done != -1 ) {
			char *temp = malloc( strlen(complete_response) + strlen(response) );
			sprintf(temp,"%s%s",complete_response,response);
			complete_response = temp;
			free(temp);
		}
		modifyDone( response, &done );
		//printf("COMPLETE RESPONSE: %s\n\n",complete_response);
		//printf( "RESPONSE: %s\n\n", response );
    	if ( bytes < 0 ) {
        	error( "ERROR reading response from socket" );
		}
	} while ( done != 0 );

    /* close the socket */
    close( sockfd );

    free( message );
	
	//printf("COMPLETE RESPONSE:\n%s\n", complete_response);
	parse();
	
	//printf("%s - %s\n%s - %s\n", home_team, away_team, home_score, away_score);
	char *temp = malloc(13);
	sprintf(temp,"%s%s%s%s",home_team,away_team,home_score,away_score);
	return temp;
}
