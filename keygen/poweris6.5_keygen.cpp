#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* transformStrip = "123456789ABCDEFGHIJKLMNPQRSTUVWXYZ";

char* myusername = "www.keygen.gq";

char randomValueFromStrip[5];

int TransformChar(char c)
{
	for(int i=0;i<0x22;i++)
	{
		if(transformStrip[i] == c)
			return i;
	}
	return 0;
}

void GenerateRandomFromStrip()
{
	for(int i=0;i<5;i++)
	{
		randomValueFromStrip[i] = transformStrip[rand() % 34];
	}
}

void RandomFillSerial(char* serial)
{
	for(int i=0;i<4;i++)
	{
		GenerateRandomFromStrip();
		memcpy(serial + (i * 5),  randomValueFromStrip, 5);
	}
}

char friendlySerialBuffer[30];

void PrintFriendlySerial(char* serial)
{
	for(int i=0,j=0;i<25;i+=5,j+=6)
	{
		friendlySerialBuffer[j] = serial[i];
		friendlySerialBuffer[j+1] = serial[i+1];
		friendlySerialBuffer[j+2] = serial[i+2];
		friendlySerialBuffer[j+3] = serial[i+3];
		friendlySerialBuffer[j+4] = serial[i+4];
		friendlySerialBuffer[j+5] = '-';
	}
	friendlySerialBuffer[29] = '\0';
	printf("[INFO]: New found: %s\n", friendlySerialBuffer);
}

bool MagicCalculator1(char* magicBuffer, int number)
{
	int a = 0;
	for (int i=0;i<0x20;i++)
	{
		int rez = (unsigned char) magicBuffer[i];
		rez *= number;
		rez += a;
		magicBuffer[i] = (unsigned char) rez;
		if(rez >= 0x100)
		{
			a = rez;
			if(a < 0)
				a += 0xFF;
			a >>= 8;
		}else
			a = 0;
	}
	return (bool)a;
}

bool MagicCalculator2(char* magicBuffer, int position)
{
	int a = 0;
	for (int i=0;i<0x20;i++)
	{
		int rez = (unsigned char) magicBuffer[i];
		a += rez;
		rez = position + a;
		magicBuffer[i] = (unsigned char) rez;
		position = 0;
		if(rez >= 0x100)
		{
			a = rez;
			if(a < 0)
				a += 0xFF;
			a >>= 8;
		}else
			a = 0;
		
	}
	return (bool)a;
}

int SerialMagicGenerator(char* serial, char* magicBuffer, int number)
{
	int serialLength = strlen(serial);
	if (serialLength <= 0)
		return 0;
	
	for(int i=0;i<serialLength;i++)
	{
		char c = serial[i];
		if (c != '\x20' && c != '\x2D')
		{
			int pos = TransformChar(c);
			if (MagicCalculator1(magicBuffer, number))
				return -1;
			if (MagicCalculator2(magicBuffer, pos))
				return -1;
		}
	}
	return 0;
}

unsigned short MagicValidator(char* combinedBuffer, int number)
{
	int c = 0;
	if(number)
	{
		for(int i=0;i<number;i++)
		{
			int rez = ((unsigned char) combinedBuffer[i]) << 8;
			c ^= rez;
			for(int j=0;j<8;j++)
			{
				if(c & 0x8000)
				{
					c += c;
					c ^= 0x1021;
				}else
					c <<= 1;
			}
		}
	}
	unsigned short s = (unsigned short) ( ((c & 0xFF) << 8) | ((c & 0xFF00) >> 8) );
	return s;
}

bool FinalVerifier(char* randomizedUsername, char* magicBuffer)
{
	char c;
	char d;
	//1
	c = *(magicBuffer + 7);
	d = *(randomizedUsername + 2);
	c >>= 1;
	c &= 2;
	d &= 2;
	c ^= d;
	if (c)
		return false;
	c = *randomizedUsername;
	d = *magicBuffer;
	c >>= 2;
	c &= 1;
	d &= 1;
	c ^= d;
	if (c)
		return false;
	return true;
}

bool FinalMagicChecker(char* magicBuffer, unsigned int array1[], unsigned int array2[])
{
	unsigned int arg5 = *(unsigned int*)magicBuffer;
	unsigned int arg6 = *(unsigned int*)(magicBuffer + 4);
	unsigned int arg7 = *(unsigned int*)(magicBuffer + 8);
	unsigned int arg8 = *(unsigned int*)(magicBuffer + 12);
	
	unsigned int value1 = (arg7 << 0x10) | (arg6 >> 0x10);
	unsigned int value2 = (arg8 << 0x10) | (arg7 >> 0x10);
	
	unsigned int c, a, di = 0;
	
	for(int i=0;i<0xA;i++)
	{
		c = array1[i];
		if(c < 0x20)
			a = value1;
		else
		{
			a = value2;
			c -= 0x20;
		}
		a >>= c & 0xFF; 
		c = di + di;
		a = (a & 1) | c;
		di = a;
	}
	
	//printf("\nDI = %X\n", di);
	for(int i=0;i<5;i++)
	{
		c = array2[i];
		if (! (di % c) )
			return false;
	}
	
	//printf("\nOK PASSED");
	return true;
}

bool FinalMagicCheckerAll(char* magicBuffer)
{
	unsigned int array11[] = 
	{
		3,
		8,
		0x16,
		0x1F,
		0x27,
		0x2B,
		0x32,
		0x33,
		0x39,
		0x3F
	};

	unsigned int array21[] =
	{
		0x09,
		0x0B,
		0x0F,
		0x14,
		0x19,
		0x1C,
		0x33,
		0x37,
		0x38,
		0x3C
	};
	
	unsigned int array31[] =
	{
		0x00,
		0x03,
		0x04,
		0x07,
		0x09,
		0x0E,
		0x13,
		0x1B,
		0x1E,
		0x36
	};
	
	unsigned int array41[] = 
	{
		0x0F,
		0x11,
		0x13,
		0x15,
		0x17,
		0x27,
		0x28,
		0x2F,
		0x32,
		0x38
	};
	
	unsigned int array12[] =
	{
		0x29,
		0x49,
		0xB3,
		0xBF,
		0xE3
	};
	
	unsigned int array22[] =
	{
		0x1F,
		0x65,
		0x7F,
		0x8B,
		0xFB
	};
	
	unsigned int array32[] =
	{
		0x03,
		0x05,
		0x0D,
		0x13,
		0x17
	};
	
	unsigned int array42[] =
	{
		0x0B,
		0x17,
		0x29,
		0xB5,
		0xF1
	};
	
	if(!FinalMagicChecker(magicBuffer, array11, array12))
		return false;
	if(!FinalMagicChecker(magicBuffer, array21, array22))
		return false;
	if(!FinalMagicChecker(magicBuffer, array31, array32))
		return false;
	if(!FinalMagicChecker(magicBuffer, array41, array42))
		return false;
	
	return true;
}

bool UsernameMagicVerifier(char* randomizedUsername, char* magicBuffer)
{
	if(!FinalVerifier(randomizedUsername, magicBuffer))
		return false;
	if(!FinalMagicCheckerAll(magicBuffer))
		return false;
	char combinedBuffer[0x20];
	memset(combinedBuffer, 0, 0x20);
	memcpy(combinedBuffer, randomizedUsername, 0x10);
	memcpy(combinedBuffer + 0x10, magicBuffer, 0x10);
	unsigned short mustHaveValue = (unsigned short) (MagicValidator(combinedBuffer, 0x1E) & 0x7FFF);
	unsigned short foundValue =    (unsigned short) ((*(unsigned short*) (magicBuffer + 0xE)) & 0x7FFF);
	return (mustHaveValue == foundValue);
}

char RandomizedUsernameBuffer[0x11];

char* RandomizeUsername(char* username)
{
	memset(RandomizedUsernameBuffer, 0, 0x11);
	int usernameLength = strlen(username);
	if(usernameLength >= 1)
	{
		if(usernameLength < 0x10)
		{
			int size = (usernameLength >> 2) * 4;
			memcpy(RandomizedUsernameBuffer, username, size);
			memcpy(RandomizedUsernameBuffer + size, username + size, usernameLength & 3);
			if( usernameLength + 1 < 0x10 )
			{
				char* ptr;
				ptr = RandomizedUsernameBuffer + size + (usernameLength & 3) + 1;
				for(int i=0;ptr<RandomizedUsernameBuffer+0x10;ptr++)
				{
					if( username[i] != '\0')
					{
						*ptr = username[i];
						i++;
					}else
						i = 0;
				}
			}
		}
		else
		{
			memcpy(RandomizedUsernameBuffer, username, 0x10);
		}
	}
	return RandomizedUsernameBuffer;
}

bool TrySerial(char* serial)
{
	char magicBuffer[0x20];
	memset(magicBuffer, 0, 0x20);
	SerialMagicGenerator(serial, magicBuffer, 0x22);
	return  UsernameMagicVerifier(RandomizedUsernameBuffer, magicBuffer);
}

int main()
{
	char newserial[] 	 = "1111111111111111111111111"; //25 chars without '-'
	
	RandomizeUsername(myusername);
	
	char* c1;
	char* c2;
	char* c3;
	char* c4;
	char* c5;
	
	for(c1 = transformStrip;c1<transformStrip+34;c1++ )
	{
		*(newserial+20) = *c1;
		for(c2 = transformStrip;c2<transformStrip+34;c2++ )
		{
			*(newserial+21) = *c2;
			for(c3 = transformStrip;c3<transformStrip+34;c3++ )
			{
				*(newserial+22) = *c3;
				for(c4 = transformStrip;c4<transformStrip+34;c4++ )
				{
					*(newserial+23) = *c4;
					for(c5 = transformStrip;c5<transformStrip+34;c5++ )
					{
						*(newserial+24) = *c5;
						RandomFillSerial(newserial);
						if(TrySerial(newserial))
						{
							PrintFriendlySerial(newserial);
							getchar();
						}
					}
				}
			}
		}
	}
	
	return 0;
}