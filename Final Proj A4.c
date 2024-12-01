#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX 200


typedef char string[MAX];


typedef struct node{
	char data;  
	struct node *next;
} Node, *NodePtr;

typedef struct {
	char data;
	NodePtr head;
} List, *ListPtr;

typedef struct {
	char data[MAX];
} Text;

Text populateFile(char filename[]);
void writeFile(char filename[],string data);

Text encodeAtbashCipher(string phrase);
Text decodeAtbashCipher(string phrase);

Text encodeShiftCipher(string phrase,int shift);
Text singleDecodeShiftCipher(string phrase,int shift);

Text encodeVignereCipher(string phrase, string key);
Text decodeVignereCipher(string phrase, string key);

Text encodeTranspositionCipher(string phrase, string key);
Text decodeTranspositionCipher(string phrase, string key,int phraseLength);

void generateKey(char *key,int length);
Text vernamCipher(string phrase, char *key,int length,int decrypt);

int gcd(int a, int b);
long long mod_exp(long long base, long long exp, long long mod);


int main(){

    srand(time(0));

	/* ---- */
	int count,length;
	char temp;
    Text message = {""};
	Text cipherText={""};
    string plaintext,key;

	/* ---- */
	int p = 2357;
    int q = 2551;
    int n = p * q; 
    int phi = (p - 1) * (q - 1); /* totient */
    
    long long encrypted_message[MAX];
    char decrypted_message[MAX+1];
    int i,itr;

    int e = 17; 
    
    int d = 0;
    int k = 2; 
    /* ---- */
	
	/* --- RSA --- */
	
    /* public key is greatest common divisor in relation to phi */
    while (gcd(e, phi) != 1) {
        e++;
    }
    
    /* private key   e x d == 1(% phi) */
    while (1) {
        if ((1 + (k * phi)) % e == 0) {
            d = (1 + (k * phi)) / e;
            break;
        }
        k++;
    }
    
    
	/* --- RSA --- */
	
	printf("\n *** Sabado's Cipher Final Project *** \n");
	printf("\n\n Type the message you'd like to encrypt and decrypt:\n ");
	
	fgets(plaintext,sizeof(string),stdin);
	if (plaintext[strlen(plaintext) - 1] == '\n') { 
		plaintext[strlen(plaintext) - 1] = '\0'; 
	}
	
	
	writeFile("Plaintext.txt",plaintext);
	message=populateFile("Plaintext.txt");
	
	printf("%s",message.data);
	
	message=encodeAtbashCipher(message.data);
	message=encodeShiftCipher(message.data,14);
	
	for(count=0;message.data[count+1]!='\0';count++){
		if((count+1)%3==0){
			temp=message.data[count];
			message.data[count]=message.data[count+1];
			message.data[count+1]=temp;
		}
	}
	
	printf("\n\nAfter Shift: \n%s",message.data);
	
	message=encodeVignereCipher(message.data,"VOLCANO");
	message=encodeTranspositionCipher(message.data,"JUNGLE");
	
	
	for(count=0;message.data[count+1]!='\0';count++){
		if((count+1)%2==0){
			message.data[count]=(char)((int)message.data[count]+7);
		}else{
			message.data[count]=(char)((int)message.data[count]-4);
		}
	}
	
	printf("\n\nAfter Loop: \n%s",message.data);
	
	generateKey(key,strlen(message.data));
	//printf("\n\n key %s \n\n",key);
	length=strlen(message.data);
	message=vernamCipher(message.data,key,length,0);
	
	/*--- RSA ---*/
	for (i = 0; message.data[i]!='\0'; i++) {
        encrypted_message[i] = mod_exp((int)message.data[i], e, n);
        //printf("Encrypted character '%c' to: %lld\n", message[i], encrypted_message[i]);
    }
    encrypted_message[i]='\0';
	
	printf("\n\nRSA Encrypted message:\n");
    for (i = 0; encrypted_message[i]!='\0'; i++) {
        //encrypted_message[i] = mod_exp((int)message[i], e, n);
        printf("%c", encrypted_message[i]);
        message.data[i]=(char)encrypted_message[i];
    }
    message.data[i]='\0';
	
    /*--- RSA ---*/
	
	printf("\n");
	writeFile("CipherText.txt",message.data);
	printf("\n");
	cipherText=populateFile("CipherText.txt");
	
	/*--- RSA ---*/
	printf("\n\nRSA Decrypted message:\n");
    for (i = 0; i<length; i++) {
        decrypted_message[i] = (char)mod_exp(encrypted_message[i], d, n);
        printf("%c",decrypted_message[i]);
    }
    decrypted_message[i] = '\0'; 
    
	strcpy(cipherText.data,decrypted_message);
	
    /*--- RSA ---*/
    
    
	cipherText=vernamCipher(cipherText.data,key,length,1);
	
	for(count=0;cipherText.data[count+1]!='\0';count++){
		if((count+1)%2==0){
			cipherText.data[count]=(char)((int)cipherText.data[count]-7);
		}else{
			cipherText.data[count]=(char)((int)cipherText.data[count]+4);
		}
	}
	printf("\n\nAfter Loop: \n%s",cipherText.data);
	
	
	cipherText=decodeTranspositionCipher(cipherText.data,"JUNGLE",length);
	cipherText=decodeVignereCipher(cipherText.data,"VOLCANO");
	for(count=0;cipherText.data[count+1]!='\0';count++){
		if((count+1)%3==0){
			temp=cipherText.data[count];
			cipherText.data[count]=cipherText.data[count+1];
			cipherText.data[count+1]=temp;
		}
	}
	
	printf("\n\nAfter Shift: \n%s",cipherText.data);
	
	cipherText=singleDecodeShiftCipher(cipherText.data,14);
	cipherText=decodeAtbashCipher(cipherText.data);
	
	printf("\n\n%s",cipherText.data);
	writeFile("Result.txt",cipherText.data);

/*
	
	encryption
	take file input
	read to "string" input
	atbash it
	shift by 14
	if %3 is 0, swap with letter before 
	vignere it by volcano
	transpose it by jungle
	loop, +7 if even spot, -4 if odd 
	vernam it with random key
		remember this key
	rsa it
	output cipher text
	
	decryption
	rsa reverse it
	vernam it with vernam key
		use with remembered key
	loop back that if even - 7, odd +4
	transpose back with jungle
	vignere it back by volcano
	if %3 is 0, swap with letter before 
	shift decrease by 14
	atbash


*/	

	
	return 0;
}

Text populateFile(char filename[])
{
	FILE *fp;
	Text data={""};
	memset(&data, 0, sizeof(Text));
	fp=fopen(filename,"rb");
	
	if(fp!=NULL){
		//printf("\nFile '%s' exists!",filename);
		/*
		fread(data.data,sizeof(char),sizeof(data.data),fp);
		size_t bytesRead = fread(data.data, sizeof(char), sizeof(data.data) - 1, fp); 
		data.data[bytesRead] = '\0';
		
		printf("\nBytes read: %zu\n", bytesRead);*/
		/*fseek(fp, 0, SEEK_SET);
		if (fread(data.data,sizeof(data.data),1, fp) ==1) { 
			printf("\n Read line: %s\n", data.data); 
		} else { 
			printf("\n Error reading from file!"); 
		}*/
		size_t bytesRead = fread(data.data, sizeof(char), sizeof(data.data) - 1, fp); 
		if (bytesRead > 0) { 
			data.data[bytesRead] = '\0'; // Null-terminate the string 
			printf("\nRead line: %s\n", data.data); 
		} else { 
			printf("\nError reading from file! Bytes read: %zu", bytesRead); 
		}
		
		fclose(fp);
		
	}else{
		printf("\nError: File not found!");
	}
	
	return data;
}

void writeFile(char filename[],string data)
{
	FILE *fp;
	
	fp=fopen(filename,"w");
	
	if(fp!=NULL){
		//printf("\nFile '%s' exists!",filename);
		
		fwrite(data,sizeof(char),strlen(data),fp);
		
		fclose(fp);
		
		printf("\n Wrote to file '%s'",filename);
		
	}else{
		printf("\nError: File not found!");
	}
	
}


Text encodeAtbashCipher(string phrase)
{
	
	Text data = {""};
	int count,itr,resCount;
	char encode;
	string result;
	
	char atbashMatrix[2][52] = {
								{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'},
								{'Z','Y','X','W','V','U','T','S','R','Q','P','O','N','M','L','K','J','I','H','G','F','E','D','C','B','A','z','y','x','w','v','u','t','s','r','q','p','o','n','m','l','k','j','i','h','g','f','e','d','c','b','a'}
								};
			
	
	
	for(count=0,resCount=0;phrase[count] != '\0'&& count<MAX;count++){					
		
		//encode = towupper(phrase[count]);
		encode = phrase[count];
			
		if(isspace(phrase[count])||ispunct(phrase[count])){
			
			result[resCount]=encode;
			resCount++;
			
		}else{	
			
			for(itr = 0; itr < 52; itr++){
				if(encode == atbashMatrix[0][itr]){
					encode = atbashMatrix[1][itr];
					break;
				}
			}
			
			result[resCount]=encode;
			resCount++;
		}
	}
	result[resCount] = '\0';
	
	printf("\n\nAtbash Encoded Message: \n%s",result);
	
	strcpy(data.data,result);
	return	data;
	
}
Text decodeAtbashCipher(string phrase)
{
	
	Text data = {""};
	int count,itr,resCount;
	char decode;
	string result;
	
	char atbashMatrix[2][52] = {
								{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'},
								{'Z','Y','X','W','V','U','T','S','R','Q','P','O','N','M','L','K','J','I','H','G','F','E','D','C','B','A','z','y','x','w','v','u','t','s','r','q','p','o','n','m','l','k','j','i','h','g','f','e','d','c','b','a'}
								};
			
	
	for(count=0,resCount=0;phrase[count] != '\0'&& count<MAX;count++){					
		
		//decode = towupper(phrase[count]);
		decode = phrase[count];
		
		if(isspace(phrase[count])||ispunct(phrase[count])){
			
			
			result[resCount]=decode;
			resCount++;
			
		}else{
		
			for(itr = 0; itr < 52; itr++){
				if(decode == atbashMatrix[1][itr]){
					decode = atbashMatrix[0][itr];
					break;
				}
			}
			
			result[resCount]=decode;
			resCount++;
		}
	}
	result[resCount] = '\0';

	printf("\n\nAtbash Decoded Message: \n%s",result);
	
	strcpy(data.data,result);
	return	data;
}

Text encodeShiftCipher(string phrase,int shift)
{
	Text data={""};
	int lower;
	int count, itr,resCount;
	int tempNum;
	char encode;
	string result;
	
	char alphabet[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	
	for(count=0,resCount=0;phrase[count] != '\0'&& count<MAX;count++){					
		lower=0;
		if(islower(phrase[count])){
			lower=1;
		}
		encode = towupper(phrase[count]);
		//encode = phrase[count];
			
		if(isspace(phrase[count])||ispunct(phrase[count])){
			
			
			result[resCount]=encode;
			resCount++;
			
		}else{
		
			for(itr = 0; itr < 26; itr++){
				if(encode == alphabet[itr]){
					tempNum=itr+shift;
					if(tempNum>=26){
						encode = alphabet[tempNum-26];
					}else{
						encode = alphabet[tempNum];
					}
					break;
				}
			}
			
			result[resCount]=(lower==1)? tolower(encode):encode;
			resCount++;
		}
	}
	result[resCount] = '\0';
	
	printf("\n\nShift Encoded Message: \n%s",result);
	strcpy(data.data,result);
	return	data;
}
Text singleDecodeShiftCipher(string phrase,int shift)
{
	Text data = {""};
	int lower;
	int count, itr,resCount;
	int tempNum;
	char decode;
	string result;
	
	char alphabet[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	
	for(count=0,resCount=0;phrase[count] != '\0'&& count<MAX;count++){					
		lower=0;
		if(islower(phrase[count])){
			lower=1;
		}
		
		decode = towupper(phrase[count]);
		//decode = phrase[count];
			
		if(isspace(phrase[count])||ispunct(phrase[count])){
			
			result[resCount]=decode;
			resCount++;
			
		}else{
		
			for(itr = 25; itr > -1; itr--){
				
				if(decode == alphabet[itr]){
					tempNum=itr-shift;
					
					if(tempNum <= -1){
						decode = alphabet[tempNum+26];
					}else{
						decode = alphabet[tempNum];
					}
					break;
					
				}
			}
			
			
			result[resCount]=(lower==1)? tolower(decode):decode;
			resCount++;
		}
	}
	result[resCount] = '\0';
	
	printf("\n\nShift Decoded Message: \n%s",result);
	strcpy(data.data,result);
	return	data;
	
}


Text encodeVignereCipher(string phrase, string key)
{
	Text data = {""};
	int lower;
	int count,itr,flag,keyCount,resCount;
	int rowNdx,colNdx;
	char encode;
	string result;
	
	char vignereMatrix[26][26]={
							{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'},
							{'B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A'},
							{'C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B'},
							{'D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C'},
							{'E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D'},
							{'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E'},
							{'G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F'},
							{'H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G'},
							{'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H'},
							{'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I'},
							{'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J'},
							{'L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K'},
							{'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L'},
							{'N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M'},
							{'O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N'},
							{'P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O'},
							{'Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'},
							{'R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q'},
							{'S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R'},
							{'T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S'},
							{'U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T'},
							{'V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U'},
							{'W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V'},
							{'X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W'},
							{'Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X'},
							{'Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y'}
						};
	
	keyCount=0;		
	count=0;
	resCount=0;
	
	while(phrase[count] != '\0'&& count<MAX){	
		
		lower=0;
		if(islower(phrase[count])){
			lower=1;
		}
		
		encode = towupper(phrase[count]);
		
		if(isspace(phrase[count])||ispunct(phrase[count])||isdigit(phrase[count])){
			
			result[resCount]=encode;
			count++;
			resCount++;
			
		}else{
		
			
			itr=0;
			flag=0;
			rowNdx=-1;
			colNdx=-1;
				
			do{/*per row*/
				if(encode==vignereMatrix[0][itr]){
					rowNdx=itr;
				}
				itr++;
			}while(itr<26&&rowNdx==-1);
			
			//printf("\n%c = %c :",phrase[count],vignereMatrix[0][rowNdx]);
			
				
			do{ /*per column*/
				if(key[keyCount]==vignereMatrix[flag][0]){
					colNdx=flag;
				}
				flag++;
			}while(flag<26&&colNdx==-1);
			
			//printf("%c=%c ||",key[keyCount],vignereMatrix[colNdx][0]);
			
			encode = vignereMatrix[colNdx][rowNdx];
			//printf("%c \n",encode);
				
			if(key[keyCount+1]=='\0'){
				keyCount=-1;
			}
			result[resCount]=(lower==1)? tolower(encode):encode;
				
			keyCount++;
			
			count++;
			resCount++;
		}
	
	
	}
	
	result[resCount] = '\0';
	
	printf("\n\nVignere Encoded Message: \n%s",result);
	
	
	
	strcpy(data.data,result);
	return	data;
}
Text decodeVignereCipher(string phrase, string key)
{
	Text data = {""};
	int lower;
	
	int count,itr,flag,keyCount,resCount;
	int rowNdx,colNdx;
	char decode;
	string result;
	
	char vignereMatrix[26][26]={
							{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'},
							{'B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A'},
							{'C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B'},
							{'D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C'},
							{'E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D'},
							{'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E'},
							{'G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F'},
							{'H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G'},
							{'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H'},
							{'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I'},
							{'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J'},
							{'L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K'},
							{'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L'},
							{'N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M'},
							{'O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N'},
							{'P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O'},
							{'Q','R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'},
							{'R','S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q'},
							{'S','T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R'},
							{'T','U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S'},
							{'U','V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T'},
							{'V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U'},
							{'W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V'},
							{'X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W'},
							{'Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X'},
							{'Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y'}
						};
	
	keyCount=0;		
	resCount=0;
	count=0;
	while(phrase[count] != '\0'&& count<MAX){		
		
		lower=0;
		if(islower(phrase[count])){
			lower=1;
		}
		
		decode = towupper(phrase[count]);
		
		if(isspace(phrase[count])||ispunct(phrase[count])||isdigit(phrase[count])){
			
			result[resCount]=decode;
			resCount++;
			count++;
			
		}else{
		
			
			itr=0;
			flag=0;
			rowNdx=-1;
			colNdx=-1;
			
			
				
			do{ /*per row*/
				if(key[keyCount]==vignereMatrix[flag][0]){
					rowNdx=flag;
				}
				flag++;
			}while(flag<26&&rowNdx==-1);
			//printf("\n%c=%c :",phrase[count],vignereMatrix[0][rowNdx]);
			
			
			do{
				if(decode==vignereMatrix[rowNdx][itr]){
					colNdx=itr;
				}
				itr++;
				
			}while(itr<26&&colNdx==-1); 
			
			//printf("%c=%c ||",key[keyCount],vignereMatrix[colNdx][0]);
			
			decode = vignereMatrix[colNdx][0];
			//printf("%c \n",decode);
				
			if(key[keyCount+1]=='\0'){
				keyCount=-1;
			}
		
			result[resCount]=(lower==1)? tolower(decode):decode;
				
			keyCount++;
			count++;
			resCount++;
		}
	
	
	}
	
	result[resCount] = '\0';
	
	printf("\n\nVignere Decoded Message:\n%s",result);
	
	strcpy(data.data,result);
	return	data;
}


Text encodeTranspositionCipher(string phrase, string key)
{
	Text data= {""};
	int count,itr,flag,keyCount, phraseCount;
	int keyLength,phraseLength,total;
	int curPos, smllrNdx, movingPos, max;
	string result;
	ListPtr *listArray, swpHolder;
	NodePtr temp, *holder;
	
	
	phraseCount = 0;
	keyLength = strlen(key);
	phraseLength = strlen(phrase);
	
	//printf("\n");
	
	listArray = (ListPtr*)malloc (keyLength * sizeof(List));
	
	if(listArray == NULL){
        printf("Memory not allocated.\n");
		
	}else{
        //printf("Memory successfully allocated.\n");
        
		/*adds keyCharacters to listArray*/
        for(count=0;count<keyLength;count++){
        	//initialize each head
        	
        	listArray[count] = (ListPtr)malloc(sizeof(List));
        	if(listArray[count]==NULL){
				//printf("Memory not allocated.\n");
			}else{
        		//printf("Memory successfully allocated.\n");
	        	listArray[count]->data=key[count];
	        	listArray[count]->head=NULL;
        	}
        	//printf(" %c", listArray[count]->data,key[count]);
        	
		}
		
		
		/*adds parts of phrase to listArray*/
		keyCount=0;
		for(count=0;phrase[count]!='\0';count++){
			
			temp= (NodePtr)malloc (sizeof(Node));
			if(temp==NULL){
				//printf("Memory not allocated.\n");
			}else{
        		//printf("Memory successfully allocated.\n");
        		
        		temp->data=phrase[count];
        		temp->next=NULL;
        		
        		
				if(listArray[keyCount]->head != NULL){
					for(holder=&(listArray[keyCount]->head) ; *holder!= NULL ; holder=&(*holder)->next){}
					(*holder)=temp;
							
				}else{
					listArray[keyCount]->head=temp;
					
				}
			}
			
			
			if(key[keyCount+1]=='\0'){
				keyCount=-1;
			}
			
			keyCount++;
			
		}
		/*
		for(count=0;count<keyLength;count++){
        	
			printf("\n %c: ",listArray[count]->data);
			
			for(holder=&(listArray[count]->head) ; *holder!= NULL ; holder=&(*holder)->next){
				printf("%c",(*holder)->data);
			}
			
        	
		}*/
		
		/*checker if all are equal*/
		
		total=(phraseLength/keyLength)+1;
		
		if(phraseLength%keyLength!=0){	
		
			
			for(keyCount=0;keyCount<keyLength;keyCount++){
				//printf("keyCount %d ",keyCount);
				
				for(holder=&(listArray[keyCount]->head),itr=0 ; *holder!= NULL ; holder=&(*holder)->next,itr++){}
				
				if(itr<total){
					
					temp= (NodePtr)malloc (sizeof(Node));
	        		temp->data='*';
	        		temp->next=NULL;
					(*holder)=temp;
				}
				//printf("result[count] %c\n",result[count]);
				
				
			
			}
				
		}
		/*
		for(count=0;count<keyLength;count++){
        	
			printf("\n %c: ",listArray[count]->data);
			
			for(holder=&(listArray[count]->head) ; *holder!= NULL ; holder=&(*holder)->next){
				printf("%c",(*holder)->data);
			}
			
        	
		}
		*/
		
		/*sorts the listArray*/
		max = keyLength-1;
	
		for(curPos=0;curPos<max;curPos++){
			
			smllrNdx=curPos;
			
			for(movingPos=curPos+1;movingPos<keyLength;movingPos++){
				
				if(listArray[smllrNdx]->data > listArray[movingPos]->data){
					smllrNdx=movingPos;
				}
				
			}
			
			if(smllrNdx!=curPos){
				swpHolder=listArray[smllrNdx];
				listArray[smllrNdx]=listArray[curPos];
				listArray[curPos]=swpHolder;
				
			}
			
			
		}
		
			
		
	
		
	
		/*making into word*/
		count=0;
		
		for(keyCount=0;keyCount<keyLength;keyCount++){
				
			for(holder=&(listArray[keyCount]->head) ; *holder!= NULL ; holder=&(*holder)->next){
				
				result[count]=(*holder)->data;
				count++;
				
			}
			
		/*	for(holder=&(listArray[keyCount]->head) ; *holder!= NULL ; holder=&(*holder)->next){
				
				if(*holder!=NULL){
				
					temp=(*holder);
					*holder=(*holder)->next;
					free(temp);
					
					//printf("\nDeletion Success!");
					
				}else{
					
				}
			}	*/
		}
		result[count]='\0';
		
		/*
		for(count=0;count<keyLength;count++){
        	
			printf("\n %c: ",listArray[count]->data);
			
			for(holder=&(listArray[count]->head) ; *holder!= NULL ; holder=&(*holder)->next){
				printf("%c",(*holder)->data);
			}
			
        	
		}*/
			
	
		
	}
	
	
	printf("\n\nTransposition Encoded Message:\n%s",result);
	
	strcpy(data.data,result);
	return	data;
}
Text decodeTranspositionCipher(string phrase, string key,int phraseLength)
{
	
	Text data= {""};
	int count,itr,flag,keyCount,pos;
	int keyLength,total;
	int curPos, smllrNdx, movingPos, max;
	string result;
	ListPtr *listArray, swpHolder;
	NodePtr temp, *holder;
	
	
	//phraseLength = strlen(phrase);
	keyLength = strlen(key);
	
	listArray = (ListPtr*)malloc (keyLength * sizeof(List));
	
	if(listArray == NULL){
        printf("Memory not allocated.\n");
		
	}else{
        //printf("Memory successfully allocated.\n");
        /*adds keyCharacters to listArray*/
        for(count=0;count<keyLength;count++){
        	//initialize each head
        	
        	listArray[count] = (ListPtr)malloc(sizeof(List));
        	if(listArray[count]==NULL){
				//printf("Memory not allocated.\n");
			}else{
        		//printf("Memory successfully allocated.\n");
	        	listArray[count]->data=key[count];
	        	listArray[count]->head=NULL;
        	}
        	//printf(" %c", listArray[count]->data);
        	
		}
		
		/*sorts the listArray*/
		max = keyLength-1;
	
		for(curPos=0;curPos<max;curPos++){
			
			smllrNdx=curPos;
			
			for(movingPos=curPos+1;movingPos<keyLength;movingPos++){
				
				if(listArray[smllrNdx]->data > listArray[movingPos]->data){
					smllrNdx=movingPos;
				}
				
			}
			
			if(smllrNdx!=curPos){
				swpHolder=listArray[smllrNdx];
				listArray[smllrNdx]=listArray[curPos];
				listArray[curPos]=swpHolder;
				
			}
			
			
		}
		
	
		
		
		/*adds parts of phrase to listArray*/
		
		
		total=phraseLength/keyLength;
		keyCount=0;
		count=0;
		itr=0;
		for(keyCount=0;keyCount<keyLength;keyCount++){
			itr=0;
			while(itr<total){
				
				temp= (NodePtr)malloc (sizeof(Node));
				if(temp==NULL){
					printf("Memory not allocated.\n");
				}else{
		    		//printf("Memory successfully allocated.\n");
		    		
		    		temp->data=phrase[count];
		    		temp->next=NULL;
		    		
		    		
					if(listArray[keyCount]->head != NULL){
						for(holder=&(listArray[keyCount]->head) ; *holder!= NULL ; holder=&(*holder)->next){}
						(*holder)=temp;
								
					}else{
						listArray[keyCount]->head=temp;
						
					}
				}
				count++;
				itr++;
			}
			
		}
		
		
		/*put keyword from listArray into original position*/
		
		for(keyCount=0;keyCount<keyLength;keyCount++){
			
			curPos=keyCount;
			
			for(itr=curPos;itr<keyLength;itr++){
				if(key[keyCount]==listArray[itr]->data){
					curPos=itr;
				} 
			}
			
			if(curPos!=keyCount){				
				swpHolder=listArray[curPos];
				listArray[curPos]=listArray[keyCount];
				listArray[keyCount]=swpHolder;
			}
		}
		
	
		/*making into word*/
		
		
		count=0;
		
		for(flag=0;flag<total;flag++){
			pos=flag;
			//printf("flag %d pos %d\n",flag, pos);
			for(keyCount=0;keyCount<keyLength;keyCount++){
				//printf("keyCount %d ",keyCount);
				
				for(holder=&(listArray[keyCount]->head),itr=0 ; itr!=pos ; holder=&(*holder)->next,itr++){}
				
				if((*holder)->data!='*'){
				
					result[count]=(*holder)->data;
				count++;
					//printf("result[count] %c\n",result[count]);
				}
				
			
				
			}
			
			
				
		}
		result[count]='\0';

		/*free up memory*/
		/*
		for(keyCount=0;keyCount<keyLength;keyCount++){
			for(holder=&(listArray[keyCount]->head) ; *holder!= NULL ; holder=&(*holder)->next){
				
				if(*holder!=NULL){
				
					temp=(*holder);
					*holder=(*holder)->next;
					free(temp);
					
					//printf("\nDeletion Success!");
					
				}else{
					
				}
			}
		}*/
/*
		for(count=0;count<keyLength;count++){
        	
			printf("\n %c: ",listArray[count]->data);
			
			for(holder=&(listArray[count]->head) ; *holder!= NULL ; holder=&(*holder)->next){
				printf("%c",(*holder)->data);
			}
			
        	
		}
		*/
		
	}
	printf("\n\nTransposition Decoded Message: \n%s",result);
	
	
	strcpy(data.data,result);
	return	data;
}


void generateKey(char *key,int length){
	int i;
	//printf("\n key generation:\n");
	for( i = 0; i < length; i++) {
		//do{
			
		key[i] = (rand() % 94) + 33;
	//	printf("| %c %d ",key[i],key[i]);
		//}while(key[i]>32);
        
    }
    key[length] = '\0';
}
Text vernamCipher(string phrase, string key, int length, int decrypt)
{
	Text data = {""};
	string result = {0};
	int count;
	
//	printf("\n\n");
	
	for(count = 0; count < length; count++) {
		//result[count] = (char)(phrase[count] ^ key[count]);
		//do{
		if(decrypt==1){
	    	result[count] = phrase[count] - key[count];
	    	
		}else{
			result[count] = (phrase[count] + key[count]);
			
		}
	       //printf("| %c, %d ",result[count],result[count]);
	   
		
	    /*printf("actual res = %c",result[count]);*/
    }
    result[count] = '\0'; 
	
	strcpy(data.data,result);
	printf("\n\nVernam Message: \n%s",data.data);
	
	return data;
}

// greatest common divisor
int gcd(int a, int b) {
    int temp;
	int retVal=-1;
    while (retVal== -1) {
        temp = a % b;
        if (temp == 0){
        	retVal=b;
		}
        a = b;
        b = temp;
    }
    
    return retVal;
}

// modular exponentiation
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}








