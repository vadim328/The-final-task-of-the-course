#include "iostream"
#include "string"
#include <exception>
#include <map>
#include <set>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>

using namespace std;

class Date {
public:
	Date(const int new_year, const int new_month, const int new_day) {
		year = new_year;
		if (new_month < 1 || new_month > 12) {
			throw logic_error("Month value is invalid: " + to_string(new_month));
		}
		else {
			month = new_month;
		}
		if (new_day < 1 || new_day > 31) {
			throw logic_error("Day value is invalid: " + to_string(new_day));
		}
		else {
			day = new_day;
		}
	}
	int GetDay() const {
		return day;
	}
	int GetMonth() const {
		return month;
	}
	int GetYear() const {
		return year;
	}
private:
	int day;
	int month;
	int year;
};

bool operator < (const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} < vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

ostream& operator << (ostream& stream, const Date& date) {
	stream << setfill('0');
	stream << setw(4) << date.GetYear() << "-"
		<< setw(2) << date.GetMonth() << "-"
		<< setw(2) << date.GetDay();
	return stream;
}

class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		database[date].insert(event);
	}
	bool DeleteEvent(const Date& date, const string& event) {
		if (database[date].count(event) > 0) {
			database[date].erase(event);
			return true;
		}
		return false;
	}
	int DeleteDate(const Date& date) {
		if (database.count(date) > 0) {
			int num = database[date].size();
			database.erase(date);
			return num;
		}
		return 0;
	}
	set<string> Find(const Date& date) {
		if (database.count(date) > 0) {
			return database.at(date);
		}
		return {};
	}
	void Print() const {
		for (const auto& element : database) {
			for (const string& event : element.second) {
				cout << element.first << " " << event << endl;
			}
		}
	}
private:
	map<Date, set<string>> database;
};

Date Date_extraction(const string& date) { // Extracting a date from a string
	stringstream date_stream(date);
	int year = 0;
	date_stream >> year;
	if (date_stream.peek() != '-' || date_stream.peek() == date[date.size() - 1]) {
		throw logic_error("Wrong date format: " + date);
	}
	date_stream.ignore(1);
	int month = 0;
	date_stream >> month;
	if (date_stream.peek() != '-') {
		throw logic_error("Wrong date format: " + date);
	}
	date_stream.ignore(1);
	int day = 0;
	date_stream >> day;
	if (!date_stream >> day) {
		throw logic_error("Wrong date format: " + date);
	}
	if (!date_stream.eof()) {
		throw logic_error("Wrong date format: " + date);
	}
	return Date(year, month, day);
}

int main() {
	try {
		Database db;
		string command_all;
		while (getline(cin, command_all)) {
			stringstream stream(command_all);
			string command;
			stream >> command;

			if (command == "Add") {
				string date, event;
				stream >> date >> event;
				db.AddEvent(Date_extraction(date), event);
			}
			else if (command == "Del") {
				string date;
				stream >> date;
				if (stream.eof()) {
					int n = db.DeleteDate(Date_extraction(date));
					cout << "Deleted " << n << " events" << endl;
				}
				else {
					string event;
					stream >> event;
					if (db.DeleteEvent(Date_extraction(date), event)) {
						cout << "Deleted successfully" << endl;
					}
					else {
						cout << "Event not found" << endl;
					}
				}
			}
			else if (command == "Find") {
				string date;
				stream >> date;
				set<string> events = db.Find(Date_extraction(date));
				for (const string& now : events) {
					cout << now << endl;
				}
			}
			else if (command == "Print") {
				db.Print();
			}
			else if (command.empty()) {
				continue;
			}
			else {
				throw logic_error("Unknown command: " + command);
			}
		}
	}
	catch (const exception& ex) {
		cout << ex.what() << endl;
	}
	return 0;
}

