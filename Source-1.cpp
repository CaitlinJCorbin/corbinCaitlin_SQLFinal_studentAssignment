//////////////////////////////////////////////////////////////////////
//	Caitlin J. Corbin												//
//	Last Updated - December 20, 2020								//
//	CSCI210 Database Systems - Final Programming Project			//
//	"Connect to the Sqlite3 database, then allow the user to		//
//	add, update, and remove data from the database."				//
//////////////////////////////////////////////////////////////////////


#include <iostream>		// Input / Output file
#include <string>		// String file
#include <sqlite3.h>	// Sqlite3 file

// Not using namespace std library

// Prints Menu
void printMenu() {
	std::cout << "****************************************\n";
	std::cout << "* 1 - View / Search data               *\n";
	std::cout << "* 2 - Insert data                      *\n";
	std::cout << "* 3 - Delete data                      *\n";
	std::cout << "* 4 - Update data                      *\n";
	std::cout << "* 5 - Exit                             *\n";
	std::cout << "****************************************" << std::endl;
};
// Prints Selection Menu
void printViewMenu() {
	std::cout << "****************************************\n";
	std::cout << "* 1 - Employee                         *\n";
	std::cout << "* 2 - Funeral                          *\n";
	std::cout << "* 3 - Host                             *\n";
	std::cout << "* 4 - Items                            *\n";
	std::cout << "* 5 - Roles                            *\n";
	std::cout << "* 6 - Yard Patients                    *\n";
	std::cout << "* 7 - Yard Upkeep                      *\n";
	std::cout << "****************************************" << std::endl;
};

/*Callback function for insert and search*/
static int callback(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/*Main Menu Function*/
int mainMenu() {
	int choice = 0; printMenu(); std::cin >> choice; while ((!std::cin || choice < 1 || choice > 4) && choice != -1) {
		if (!std::cin) {
			std::cin.clear(); std::cin.ignore(1000, '/n');
		}std::cout << "That is not a valid choice." << std::endl << std::endl; printMenu(); std::cin >> choice;
	} return choice;
};

std::string insert(sqlite3* DB)
{
	char* messaggeError;
	int exit = sqlite3_open("funeral_home.db", &DB);
	std::string query = "SELECT * FROM Employees;";

	std::string eID;
	std::string jobTitle;

	std::cout << "Employee ID: "; std::cin >> eID;
	std::cout << "Job Title: "; std::cin >> jobTitle;

	std::string sql = "INSERT INTO Employee(Employee_ID, Job_Title) VALUES (" + eID + ", '" + jobTitle + "')";

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
		std::cout << "Records created Successfully!" << std::endl;

	sqlite3_close(DB);
	return (0);
};

//std::string delete(sqlite3* DB) {
//
//	char* messaggeError;
//	int exit = sqlite3_open("funeral_home.db", &DB);
//	std::string query = "SELECT * FROM Employees;";
//
//	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
//
//	sql = "";
//	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
//	if (exit != SQLITE_OK) {
//		std::cerr << "Error DELETE" << std::endl;
//		sqlite3_free(messaggeError);
//	}
//	else
//		std::cout << "Record deleted Successfully!" << std::endl;
//
//	std::cout << "STATE OF TABLE AFTER DELETE OF ELEMENT" << std::endl;
//	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
//
//	sqlite3_close(DB);
//	return (0);
//};


void viewEmployee(sqlite3* DB)
{
	std::string query = "SELECT * FROM Employee";
	sqlite3_stmt* pRes; 
	std::string m_strLastError;
	std::string employeeID;
	std::string jobTitle;

	if (sqlite3_prepare_v2(DB, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(DB);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		std::cout << "Please choose the employee you want to see:" << std::endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt* pRes2;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			std::cout << i << ". " << sqlite3_column_text(pRes, 0);
			std::cout << std::endl;
			i++;
		}
		do
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(1000, '/n');
			}
			std::cin >> choice;
			if (!std::cin || choice < 1 || choice > i)
				std::cout << "That is not a valid choice! Try Again!" << std::endl;
		} while (!std::cin);

		sqlite3_reset(pRes);
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);

		employeeID = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		jobTitle = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		sqlite3_finalize(pRes);

		std::cout << "Employee ID: " << employeeID << std::endl;
		std::cout << "Job Title: " << jobTitle << std::endl;
	};
};

void viewFuneral(sqlite3* DB)
{
	std::string query = "SELECT * FROM Funeral";
	sqlite3_stmt* pRes;
	std::string m_strLastError;
	std::string ID;
	std::string Patient_Name;
	std::string Date_Of_Funeral;
	std::string Host_Name;
	std::string Cause_Of_Death;
	std::string Sex;

	if (sqlite3_prepare_v2(DB, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(DB);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		std::cout << "Please choose the ID you want to see:" << std::endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt* pRes2;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			std::cout << i << ". " << sqlite3_column_text(pRes, 0);
			std::cout << std::endl;
			i++;
		}
		do
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(1000, '/n');
			}
			std::cin >> choice;
			if (!std::cin || choice < 1 || choice > i)
				std::cout << "That is not a valid choice! Try Again!" << std::endl;
		} while (!std::cin);

		sqlite3_reset(pRes);
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);

		ID = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		Patient_Name = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		Date_Of_Funeral = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 2));
		Host_Name = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 3));
		Cause_Of_Death = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 4));
		Sex = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 5));
		sqlite3_finalize(pRes);

		std::cout << "ID: " << ID << std::endl;
		std::cout << "Patient Name: " << Patient_Name << std::endl;
		std::cout << "Date of Funeral: " << Date_Of_Funeral << std::endl;
		std::cout << "Host Name: " << Host_Name << std::endl;
		std::cout << "Cause of Death: " << Cause_Of_Death << std::endl;
		std::cout << "Sex: " << Sex << std::endl;
	};
};
void viewHost(sqlite3* DB)
{
	std::string query = "SELECT * FROM Host";
	sqlite3_stmt* pRes;
	std::string m_strLastError;
	std::string ID;
	std::string Host_Name;
	std::string Host_Phone_Number;
	std::string Relationship_To_Patient;

	if (sqlite3_prepare_v2(DB, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(DB);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		std::cout << "Please choose the ID you want to see:" << std::endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt* pRes2;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			std::cout << i << ". " << sqlite3_column_text(pRes, 0);
			std::cout << std::endl;
			i++;
		}
		do
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(1000, '/n');
			}
			std::cin >> choice;
			if (!std::cin || choice < 1 || choice > i)
				std::cout << "That is not a valid choice! Try Again!" << std::endl;
		} while (!std::cin);

		sqlite3_reset(pRes);
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);

		ID = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		Host_Name = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		Host_Phone_Number = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 2));
		Relationship_To_Patient = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 3));
		sqlite3_finalize(pRes);

		std::cout << "ID: " << ID << std::endl;
		std::cout << "Host Name: " << Host_Name << std::endl;
		std::cout << "Host Phone Number: " << Host_Phone_Number << std::endl;
		std::cout << "Relationship to Patient: " << Relationship_To_Patient << std::endl;
	};
};
void viewItems(sqlite3* DB)
{
	std::string query = "SELECT * FROM Items";
	sqlite3_stmt* pRes;
	std::string m_strLastError;
	std::string Order_Number;
	bool Flowers;
	bool Casket;
	bool Urn;

	if (sqlite3_prepare_v2(DB, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(DB);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		std::cout << "Please choose the ID you want to see:" << std::endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt* pRes2;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			std::cout << i << ". " << sqlite3_column_text(pRes, 0);
			std::cout << std::endl;
			i++;
		}
		do
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(1000, '/n');
			}
			std::cin >> choice;
			if (!std::cin || choice < 1 || choice > i)
				std::cout << "That is not a valid choice! Try Again!" << std::endl;
		} while (!std::cin);

		sqlite3_reset(pRes);
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);

		Order_Number = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		Flowers = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		Casket = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 2));
		Urn = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 3));
		sqlite3_finalize(pRes);

		std::cout << "Order Number: " << Order_Number << std::endl;
		std::cout << "Flowers: " << Flowers << std::endl;
		std::cout << "Casket: " << Casket << std::endl;
		std::cout << "Urn: " << Urn << std::endl;
	};
};
void viewRoles(sqlite3* DB)
{
	std::string query = "SELECT * FROM Roles";
	sqlite3_stmt* pRes;
	std::string m_strLastError;
	std::string Job_Title;
	std::string Salary;

	if (sqlite3_prepare_v2(DB, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(DB);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		std::cout << "Please choose the ID you want to see:" << std::endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt* pRes2;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			std::cout << i << ". " << sqlite3_column_text(pRes, 0);
			std::cout << std::endl;
			i++;
		}
		do
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(1000, '/n');
			}
			std::cin >> choice;
			if (!std::cin || choice < 1 || choice > i)
				std::cout << "That is not a valid choice! Try Again!" << std::endl;
		} while (!std::cin);

		sqlite3_reset(pRes);
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);

		Job_Title = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		Salary = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));

		
		sqlite3_finalize(pRes);

		std::cout << "Job Title: " << Job_Title << std::endl;
		std::cout << "Salary: " << Salary << std::endl;
	};
};
void viewYardPatients(sqlite3* DB)
{
	std::string query = "SELECT * FROM YardPatients";
	sqlite3_stmt* pRes;
	std::string m_strLastError;
	std::string ID;
	std::string Patient_Name;
	std::string Date_Of_Death;
	std::string Date_Of_Birth;
	std::string Stone_Type;

	if (sqlite3_prepare_v2(DB, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(DB);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		std::cout << "Please choose the ID you want to see:" << std::endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt* pRes2;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			std::cout << i << ". " << sqlite3_column_text(pRes, 0);
			std::cout << std::endl;
			i++;
		}
		do
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(1000, '/n');
			}
			std::cin >> choice;
			if (!std::cin || choice < 1 || choice > i)
				std::cout << "That is not a valid choice! Try Again!" << std::endl;
		} while (!std::cin);

		sqlite3_reset(pRes);
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);

		ID = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		Patient_Name = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		Date_Of_Death = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 2));
		Date_Of_Birth = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 3));
		Stone_Type = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 4));
		sqlite3_finalize(pRes);

		std::cout << "ID: " << ID << std::endl;
		std::cout << "Patient Name " << Patient_Name << std::endl;
		std::cout << "Date of Death: " << Date_Of_Death << std::endl;
		std::cout << "Date of Birth: " << Date_Of_Birth << std::endl;
		std::cout << "Stone Type " << Stone_Type << std::endl;
	};
};
void viewYardUpkeep(sqlite3* DB)
{
	std::string query = "SELECT * FROM YardUpkeep";
	sqlite3_stmt* pRes;
	std::string m_strLastError;
	std::string Employee_ID;
	std::string Date;
	std::string Task;
	std::string Employee_Hours;

	if (sqlite3_prepare_v2(DB, query.c_str(), -1, &pRes, NULL) != SQLITE_OK)
	{
		m_strLastError = sqlite3_errmsg(DB);
		sqlite3_finalize(pRes);
		std::cout << "There was an error: " << m_strLastError << std::endl;
		return;
	}
	else
	{
		std::cout << "Please choose the ID you want to see:" << std::endl;
		int columnCount = sqlite3_column_count(pRes);
		int i = 1, choice;
		sqlite3_stmt* pRes2;
		while (sqlite3_step(pRes) == SQLITE_ROW)
		{
			std::cout << i << ". " << sqlite3_column_text(pRes, 0);
			std::cout << std::endl;
			i++;
		}
		do
		{
			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(1000, '/n');
			}
			std::cin >> choice;
			if (!std::cin || choice < 1 || choice > i)
				std::cout << "That is not a valid choice! Try Again!" << std::endl;
		} while (!std::cin);

		sqlite3_reset(pRes);
		for (int i = 0; i < choice; i++)
			sqlite3_step(pRes);

		Employee_ID = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 0));
		Date = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 1));
		Task = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 2));
		Employee_Hours = reinterpret_cast<const char*>(sqlite3_column_text(pRes, 3));
		
		sqlite3_finalize(pRes);

		std::cout << "Employee ID: " << Employee_ID << std::endl;
		std::cout << "Date: " << Date << std::endl;
		std::cout << "Task: " << Task << std::endl;
		std::cout << "Employee Hours " << Employee_Hours << std::endl;
	};
};

/*Main function*/
int main(int argc, char** argv)
{

	/*Opens database*/
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open("funeral_home.db", &DB);

	if (exit) {
		std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl << std::endl;
		return (-1);
	}
	else
		std::cout << "Opened Database Successfully!" << std::endl << std::endl;
	/*End of code section*/

	/*User Selection*/
	int choice;
	std::cout << "NOTE: ONLY VIEW / SEARCH FUNCTIONING" << std::endl;
	std::cout << " ***** Database Management System ***** " << std::endl;
	choice = mainMenu();
	while (true) {
		switch (choice) {
		case 1: 
			int selection;
			printViewMenu();
			std::cin >> selection;
			switch (selection) {
			case 1: viewEmployee(DB); break;
			case 2: viewFuneral(DB); break;
			case 3: viewHost(DB); break;
			case 4: viewItems(DB); break;
			case 5: viewRoles(DB); break;
			case 6: viewYardPatients(DB); break;
			case 7: viewYardUpkeep(DB); break;
			};
			 break;
 		case 2: insert(DB); break;
// 		case 3: delete(DB); break;
// 		case 4: update(DB); break;
		case 5: return 0; break;
		}
		choice = mainMenu();
		/*End of user selection*/
	};
}