#include "pch.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <functional>


using namespace std;

class Data
{
private:
	string time;
	string date;
	string location;
	double temp;
	long int moist;
	double MoldRisk;

	double Avaragetemp;
	long int Avaragemoist;

	///////////////////
	string Datum = date;
	double sumIntemp = 0;
	int sumInmoist = 0;
	double sumOuttemp = 0;
	int sumOutmoist = 0;
	double AntalIntemp = 0;
	long int AntalOuttemp = 0;
	double INAvarageTemp = 0;
	long int INAvarageMoist = 0;
	double OUTAvaragetemp = 0;
	long int OUTAvarageMoist = 0;
	double MoldLine = 0;
	double MoldRisk1 = 0;
	double MoldRisk2 = 0;
	int i = 0;
	int counterInne, counterUte = 0;
	bool myFlag;
	string keyin;

public:
	Data() {};
	Data(string date, string time, string location, string temp, string moist)
		: date(date), time(time), location(location), temp(stod(temp)), moist(stoi(moist)) {};

	Data(string date, double Avaragetemp, long int AvarageMoist, double MoldRisk)
		: date(date), Avaragetemp(Avaragetemp), Avaragemoist(AvarageMoist), MoldRisk(MoldRisk) {};

	void BringinFile();
	void SortAvarageTemp(string date, string location, double temp, long int moist);
	void Menu();

	vector<Data *>Inside;
	vector<Data *>Outside;

	string get_date() const { return this->date; }
	double get_Avaragetemp() const { return this->Avaragetemp; }
	long int get_Avaragemoist() const { return this->Avaragemoist; }
	double get_MoldRisk() const { return this->MoldRisk; }

	string datetofind;

}DataObj;

void Data::BringinFile()
{
	//Opens and brings in the data from the file
	ifstream inFile("Tempdata4.csv");

	string date;
	string time;
	string location;
	string temp;
	string moist;
	double temp1;
	long int moist1;
	myFlag = false;
	//Checks if File is open 
	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			getline(inFile, date, ' ');
			getline(inFile, time, ',');
			getline(inFile, location, ',');
			getline(inFile, temp, ',');
			getline(inFile, moist);
			temp1 = stod(temp);
			moist1 = stol(moist);
			SortAvarageTemp(date, location, temp1, moist1);

		}
		inFile.close();
	}
	else
	{
		cout << "Unable to open file" << endl;
	}
}

void Data::SortAvarageTemp(string date, string location, double temp, long int moist)
{
	if (Datum == date)
	{
		if (location == "Inne")
		{
			sumIntemp = sumIntemp + temp;
			sumInmoist = sumInmoist + moist;
			AntalIntemp = AntalIntemp + 1;

			counterInne++;


		}

		else if (location == "Ute")
		{
			sumOuttemp = sumOuttemp + temp;
			sumOutmoist = sumOutmoist + moist;
			AntalOuttemp = AntalOuttemp + 1;

			counterUte++;
		}


	}
	else if (myFlag == true)
	{
		INAvarageTemp = sumIntemp / counterInne;
		INAvarageMoist = sumInmoist / counterInne;

		MoldLine = -0.0015 * pow(INAvarageTemp, 3) + 0.1193 * pow(INAvarageTemp, 2) - (2.9878 * INAvarageTemp) + 102.96;
		MoldRisk1 = INAvarageMoist - MoldLine;

		Inside.push_back(new Data(Datum, INAvarageTemp, INAvarageMoist, MoldRisk1));

		OUTAvaragetemp = sumOuttemp / counterUte;
		OUTAvarageMoist = sumOutmoist / counterUte;

		MoldLine = -0.0015 * pow(OUTAvaragetemp, 3) + 0.1193 * pow(OUTAvaragetemp, 2) - (2.9878 * OUTAvaragetemp) + 102.96;
		MoldRisk2 = OUTAvarageMoist - MoldLine;

		Outside.push_back(new Data(Datum, OUTAvaragetemp, OUTAvarageMoist, MoldRisk2));

		Datum = date;

		sumIntemp = sumInmoist = sumOuttemp = sumOutmoist = AntalIntemp = AntalOuttemp = INAvarageTemp = INAvarageMoist = 0;
		OUTAvaragetemp = OUTAvarageMoist = MoldLine = MoldRisk1 = MoldRisk2 = i = counterUte = counterInne = 0;

	}
	else
	{
		Datum = date;
		myFlag = true;

	}

}

void Data::Menu()
{
	bool Menu = true;
	while (Menu)
	{
		cout << "Main Menu" << endl;
		cout << "1. Indoor Avarage Temp " << endl;
		cout << "2. Indoor Avarage Humidity " << endl;
		cout << "3. Indoor Mold Risk " << endl;
		cout << "4. Search for a specific date Indoor " << endl << endl;
		cout << "5. Outdoor Avarage Temp " << endl;
		cout << "6. Outdoor Avarage Humidity " << endl;
		cout << "7. Outdoor Mold Risk " << endl;
		cout << "8. Search for a specific date Outdoor " << endl;
		cout << "9. Date for Autumn and Winter " << endl;
		cout << "0. Exit " << endl;
		cin >> keyin;
		bool searching = true;

		switch (keyin[0])
		{
		case '1':
		{
			vector<Data>Ina;
			for (int i = 0; i < Inside.size(); i++)
			{
				Ina.push_back(*Inside[i]);
			}
			stable_sort(Ina.begin(), Ina.end(), [](const Data & i, const  Data& j) {return i.Avaragetemp < j.Avaragetemp; });

			for (auto & e : Ina)
				cout << e.date << ", " << e.Avaragetemp << '\n';

			system("pause");
			system("cls");
			Ina.clear();
			Ina.shrink_to_fit();
		}
		break;

		case '2':
		{
			vector<Data>Inb;
			for (int i = 0; i < Inside.size(); i++)
			{
				Inb.push_back(*Inside[i]);
			}
			stable_sort(Inb.begin(), Inb.end(), [](const Data& a, const Data& b) { return a.get_Avaragemoist() < b.get_Avaragemoist(); });

			for (auto & e : Inb)
			{
				cout << e.date << " , " << e.Avaragemoist << '\n';
			}
			system("pause");
			system("cls");
			Inb.clear();
			Inb.shrink_to_fit();
		}
		break;
		case '3':
		{
			vector<Data>Inc;
			for (int i = 0; i < Inside.size(); i++)
			{
				Inc.push_back(*Inside[i]);
			}
			stable_sort(Inc.begin(), Inc.end(), [](const Data& e, const Data& f) {return e.get_MoldRisk() < f.get_MoldRisk(); });

			for (auto & e : Inc)
			{
				cout << e.date << " , " << e.MoldRisk << '\n';
			}
			system("pause");
			system("cls");
			Inc.clear();
			Inc.shrink_to_fit();
		}
		break;
		case '4':
		{

			bool datefound;
			system("cls");
			cout << "Please enter the date you wish to find and do it like this: yyyy-mm-dd" << endl;
			cin >> datetofind;
			if (datefound = false)
			{
				cout << "Could not find the date" << endl;
				system("pause");
			}
			if (datetofind.size() == 10)
			{
				vector<Data *>::const_iterator itr = Inside.begin();
				while (itr != Inside.end() || (*itr)->get_date() != datetofind)
				{

					itr++;

					if (datetofind == (*itr)->get_date())
					{

						cout << "Date has been found: " << datetofind << endl;
						cout << "Date:" << (*itr)->get_date() << " the average temperature Inside: " << (*itr)->get_Avaragetemp() << endl;
						cout << "The avarage humidity: " << (*itr)->get_Avaragemoist() << "  And moldrisk: " << (*itr)->get_MoldRisk() << endl;
						// Sleep(2000);
						system("pause");
						datefound = true;
						break;


					}
					else { datefound = false; }
				}
			}
			else { datefound = false; }
		}
		break;
		case '5':
		{
			vector<Data>Outa;
			for (int i = 0; i < Outside.size(); i++)
			{
				Outa.push_back(*Outside[i]);
			}
			stable_sort(Outa.begin(), Outa.end(), [](const Data& k, const Data& l) {return k.get_Avaragetemp() < l.get_Avaragetemp(); });

			for (auto & e : Outa)
			{
				cout << e.date << ", " << e.Avaragetemp << '\n';
			}
			system("pause");
			system("cls");
			Outa.clear();
			Outa.shrink_to_fit();
		}
		break;
		case '6':
		{
			vector<Data>Outb;
			for (int i = 0; i < Outside.size(); i++)
			{
				Outb.push_back(*Outside[i]);
			}
			stable_sort(Outb.begin(), Outb.end(), [](const Data& c, const Data& d) {return c.get_Avaragemoist() < d.get_Avaragemoist(); });

			for (auto & e : Outb)
			{
				cout << e.date << " , " << e.Avaragemoist << '\n';
			}
			system("pause");
			system("cls");
			Outb.clear();
			Outb.shrink_to_fit();
		}
		break;
		case '7':
		{
			vector<Data>Outc;
			for (int i = 0; i < Outside.size(); i++)
			{
				Outc.push_back(*Outside[i]);
			}
			stable_sort(Outc.begin(), Outc.end(), [](const Data& g, const Data& h) {return g.get_MoldRisk() < h.get_MoldRisk(); });

			for (auto & e : Outc)
			{
				cout << e.date << " , " << e.MoldRisk << '\n';
			}
			system("pause");
			system("cls");
			Outc.clear();
			Outc.shrink_to_fit();
		}
		break;
		case '8':
		{

			bool datefound;
			system("cls");
			cout << "Please enter the date you wish to find in the database like this: yyyy-mm-dd" << endl;
			cin >> datetofind;
			if (datefound = false)
			{
				cout << "Could not find the date" << endl;
				system("pause");
			}

			if (datetofind.size() == 10)
			{
				vector<Data *>::const_iterator itr = Outside.begin();
				while (itr != Outside.end() || (*itr)->get_date() != datetofind)
				{

					itr++;

					if (datetofind == (*itr)->get_date())
					{

						cout << "Date has been found: " << datetofind << endl;
						cout << "Date:" << (*itr)->get_date() << " the average temperature outside:  " << (*itr)->get_Avaragetemp() << endl;
						cout << "The avarage humidity: " << (*itr)->get_Avaragemoist() << "  And moldrisk: " << (*itr)->get_MoldRisk() << endl;
						// Sleep(2000);
						system("pause");
						datefound = true;
						break;


					}
					else { datefound = false; }
				}
			}
			else { datefound = false; }
		}
		break;
		case '9':
			searching = true;
			do
			{
				if (Outside[i]->get_Avaragetemp() < 10 && Outside[i + 1]->get_Avaragetemp() < 10 && Outside[i + 2]->get_Avaragetemp() < 10 && Outside[i + 3]->get_Avaragetemp() < 10 && Outside[i + 4]->get_Avaragetemp() < 10)
				{

					cout << Outside[i]->get_date() << '\n';
					searching = false;
				}
				else
				{
					i++;
				}
				if (i == Outside.size() - 1)
				{
					cout << "No meteorological Autumn was found." << std::endl;

					searching = false;
				}
			} while (searching);
			searching = true;
			do
			{
				if (Outside[i]->get_Avaragetemp() < 0 && Outside[i + 1]->get_Avaragetemp() < 0 && Outside[i + 2]->get_Avaragetemp() < 0 && Outside[i + 3]->get_Avaragetemp() < 0 && Outside[i + 4]->get_Avaragetemp() < 0)
				{

					cout << Outside[i]->get_date() << '\n';
					searching = false;
				}
				else
				{
					i++;
				}
				if (i == Outside.size() - 1)
				{
					cout << "No meteorological Winter was found." << std::endl;

					searching = false;
				}
			} while (searching);
			system("pause");
			system("cls");
			break;
		case '0':
			cout << "Bye Bye!!" << endl;
			Menu = false;
		default:
			break;
		}
	}
}

int main()
{
	DataObj.BringinFile();
	DataObj.Menu();

	return 0;

}
