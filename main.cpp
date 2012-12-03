#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void readString(string str); 
string findOpt(string &str);
int findKey(string &str);
string findSource(string &str);
int analysisOpt(string &str, string &alphabet);
void CaesarCipher(int Opt, string &alphabet, int Key, string &Source, string &Dest);
bool find(char c, char *Alp, int sizeAlp);
void help();
void main()
{
	string O;

	getline(cin, O);
	readString(O);
}
void readString(string str)
{
	string Crypt;
	string alphabet = "";
	int Opt;
	int pos = 0;
	pos = str.find(" ");
	Crypt = str.substr(0, pos);
	str.erase(0, pos+1);
	if(Crypt.compare("crypt"))
	{
		cout<<"Error!!!\nUnknown command\n";
		system("pause");
		return;
	}
	
	string Option = findOpt(str);
	Opt = analysisOpt(Option, alphabet);
	if(Opt == 0)
	{
		cout<<"Error!!!\nUnknown options\n";
		system("pause");
		return;
	}
	if(Opt == 1)
	{
		help();
		return;
	}
	int Key = findKey(str);
	string Source = findSource(str);
	string Dest = findSource(str);



	if(!Source.length())
	{
		cout<<"Error!!!\nPath is not\n";
		system("pause");
		return;
	}
	CaesarCipher(Opt, alphabet, Key, Source, Dest);

}

string findOpt(string &str)
{
	int pos = -1;
	string Opt;
	if( (str.at(0) >= 48) && (str.at(0) <= 57) )
	{
		return "";
	}
	else
	{
		for(int i = 1; i < str.length(); i++)
		{
			if( (str.at(i) >= 48) && (str.at(i) <= 57) )
			{
				if(str.at(i-1) == ' ') 
				{
					Opt = str.substr(0, i-1);
					str.erase(0, i);
					break;
				}
				else 
					return "";
			}
		}
		if( str.rfind("-h") != -1 )
			return str;
	}
	return Opt;
}

int findKey(string &str)
{
	int pos = str.find(" ");
	if(pos == -1)
		return -1;
	string num = str.substr(0, pos);
	str.erase(0, pos+1);
	if(!num.length())
		return -1;
	for(int i = 0; i < num.length(); i++)
		if( (((int)(num.at(i)) < 48) || ((int)(num.at(i)) > 57)) )
			return -1;
	char *numC = new char[num.length()];
	strcpy(numC, num.c_str());
	int numI = atoi(numC);
	return numI;
}

string findSource(string &str)
{
	string Source;
	if(!str.length())
		return str;
	int pos = str.find(" ");
	if(pos > 0)
	{
		Source = str.substr(0, pos);
		str.erase(0, pos+1);
		pos = 0;
	}
	else
	{
		Source = str;
		str.clear();
		pos = 0;
	}
	while(true)
	{
		pos = Source.rfind("/", pos);
		if(pos == -1)
			break;
		Source.insert(pos, "/");
		
		pos+=2;
	}
	if(Source.at(0) == '\"')
	{
		if(Source.at(Source.length()-1) == '\"')
		{
			Source.erase(0, 1);
			Source.erase(Source.length()-1, Source.length());
		}
		else
			return "";
	}
	if(Source.find(".") == -1)
	{
		return "";
	}
	return Source;
}

int analysisOpt(string &str, string &alphabet)
{
	int Opt = 0;
	int pos = 0;

	if(!str.length())
		return 21;

	if(str.rfind("--help") != -1)
	{
		pos = str.rfind("--help");
		str.erase(pos, pos+6);
		if(str.length())
			return 0;
		return 1;
	} else
	if(str.rfind("--h") != -1)
	{
		pos = str.rfind("-h");
		str.erase(pos, pos+2);
		if(str.length())
			return 0;
		return 1;
	}

	if( (str.rfind("-t encode") != -1) || (str.rfind("--type=encode") != -1) )
	{
		Opt = 21;
		pos = str.rfind("-t encode");
		if(pos>=0)
			str.erase(pos, pos+9);
		else
		{
			pos = str.rfind("--type=encode");
			str.erase(pos, pos+13);
		}
	} else
	if( (str.rfind("-t decode") != -1) || (str.rfind("--type=decode") != -1) )
	{
		Opt = 22;
		pos = str.rfind("-t decode");
		if(pos>=0)
			str.erase(pos, pos+9);
		else
		{
			pos = str.rfind("--type=decode");
			str.erase(pos, pos+13);
		}
	} 
	else
		Opt = 21;

	if(!str.length())
		return Opt;

	if(str.at(str.length()-1) == ' ')
		str.erase(str.length()-1);
	if(str.at(0) == ' ')
		str.erase(0, 1);

	pos = str.find(" ");
	if(pos < 0)
		pos = str.length()-1;
	string opt = str.substr(0, pos+1);
	str.erase(0, pos+1);

	if( ((opt.at(0)) == '-') && ((opt.at(1)) == '-') && ((opt.at(2)) == 'a'))
	{
		pos = opt.find("\"", 12);
		if(pos == -1)
			return 0;
		alphabet = opt.substr(12, pos-1);
		alphabet.erase(alphabet.length()-1);
		if(Opt)
			Opt = Opt + 1100;
		else
			Opt = 11;
	} else
	if( ((opt.at(0)) == '-') && ((opt.at(1)) == 'a') )
	{
		pos = str.find("\"", 1);
		if(pos == -1)
			return 0;
		alphabet = str.substr(1, pos-1);
		str.erase(0, pos+2);
		if(Opt)
			Opt = Opt + 1100;
		else
			Opt = 11;
	}

	if(str.length())
		return 0;

	return Opt;
}

void CaesarCipher(int Opt, string &alphabet, int Key, string &Source, string &Dest)
{
	char *ALP;
	int sizeAlp;
	int shift = Key;
	if(Opt == 21 || Opt == 22)
	{
		ALP = new char[62];
		for(int i = 0, j = 65; i < 62; i+=2)
		{
			ALP[i] = j;
			ALP[i+1] = j + 32;
			j++;
		}
		for(int i = 52; i < 62; i++)
		{
			ALP[i] = i-4;
		}
		sizeAlp = 62;
	}
	else
	{
		ALP = new char[alphabet.length()];
		strcpy(ALP, alphabet.c_str());
		sizeAlp = alphabet.length();
		Opt = Opt - 1100;
	}

	if(Key < 0)
	{
		cout<<"Error!!!\nInvalid key\n";
		system("pause");
		return;
	}

	if (Key > 26) 
		Key = Key % 26;
	while(shift > 10)
		shift = shift % 10;

	if(!Dest.length())
		Dest = Source;

	string strIn;
	string strOut;
	ifstream in;
	in.open(Source);
	getline (in, strIn, '\0');
	in.close();

	switch (Opt) 
    {
	case 21:
		{
			for(int i = 0; i < strIn.length(); i++)
			{
				if(find(strIn.at(i), ALP, sizeAlp))
				{
					if( (((int)(strIn.at(i)) >= 65) && ((int)(strIn.at(i)) <= 90)) || (((int)(strIn.at(i)) >= 97) && ((int)(strIn.at(i)) <= 122)) )
					{
						if (((int)(strIn[i]) >= 97) && ((int)(strIn[i]) <= 122))
						{
							if ((int)(strIn[i]) + Key > 122)
								strOut += (char)((int)(strIn[i]) + Key - 26);
							else
								strOut += (char)((int)(strIn[i]) + Key);
						}
						else
						{
							if ((int)(strIn[i]) + Key > 90)
								strOut += (char)((int)(strIn[i]) + Key - 26);
							else
								strOut += (char)((int)(strIn[i]) + Key);
						}
					} else
					if( (((int)(strIn.at(i)) >= 48) && ((int)(strIn.at(i)) <= 57)) )
					{
						 if ((int)(strIn[i]) + shift > 57)
							strOut += (char)((int)(strIn[i]) + shift - 10);
						 else
							strOut += (char)((int)(strIn[i]) + shift);
					}
				}
				else
					strOut += (char)((int)(strIn[i]));
			}
			break;
		}
	case 22:
		{
			for(int i = 0; i < strIn.length(); i++)
			{
				if(find(strIn.at(i), ALP, sizeAlp))
				{
					if( (((int)(strIn.at(i)) >= 65) && ((int)(strIn.at(i)) <= 90)) || (((int)(strIn.at(i)) >= 97) && ((int)(strIn.at(i)) <= 122)) )
					{
						if (((int)(strIn[i]) >= 97) && ((int)(strIn[i]) <= 122))
						{
							if ((int)(strIn[i]) - Key < 97)
								strOut += (char)((int)(strIn[i]) - Key + 26);
							else
								strOut += (char)((int)(strIn[i]) - Key);
						}
						else
						{
							if ((int)(strIn[i]) - Key < 65)
								strOut += (char)((int)(strIn[i]) - Key + 26);
							else
								strOut += (char)((int)(strIn[i]) - Key);
						}
					} else
					if( (((int)(strIn.at(i)) >= 48) && ((int)(strIn.at(i)) <= 57)) )
					{
						 if ((int)(strIn[i]) - shift < 48)
							strOut += (char)((int)(strIn[i]) - shift + 10);
						 else
							strOut += (char)((int)(strIn[i]) - shift);
					}
				}
				else
					strOut += (char)((int)(strIn[i]));
			}
			break;
		}
	}
	ofstream out(Dest);
	out<<strOut;
	out.close();
}

bool find(char c, char *Alp, int sizeAlp)
{
	for(int i = 0; i < sizeAlp; i++)
		if(Alp[i] == c)
			return true;
	return false;
}

void help()
{
	setlocale(LC_ALL,"Russian");
	cout<<"$ crypt [options] <key> <source> [<dest>]"<<endl<<endl;
	cout<<"options:"<<endl<<endl;
	cout<<"\t-a, --alphabet=<alphabet> "
		<<"alphabet Ч алфавит дл€ работы алгоритма (по \n\tумолчанию "
		<<"содержит буквы из латниского алфавита и цифры: AaBbCc..Zz0..9)"<<endl;
	cout<<"\t-t, --type=<type>          type может быть 'encode' или 'decode', \n\tпо умолчанию Ч encode"<<endl;
	cout<<"\t-h, --help                 выводит эту справку"<<endl<<endl;
	cout<<"key:"<<endl<<"\tключ дл€ шифровани€/дешифровани€"<<endl<<endl;
	cout<<"source:"<<endl<<"\tисходный файл дл€ шифровани€/дешифровани€"<<endl<<endl;
	cout<<"dest:"<<endl<<"\tфайл, куда будет записан новый, зашифрованный текст. \n\t≈сли не указан, то переписывает source"<<endl<<endl;
	system("pause");
}