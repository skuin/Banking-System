// 17.12.27
// Banking System ver.0.6

// [진행상황]
// 17.12.04 Banking System Ver 0.1 시작
// 17.12.08 - 복사 생성자 추가
// 17.12.14 - const 선언 추가
// 17.12.20 - 클래스 정의 선언 구분, 컨트롤 클래스 정의
// 17.12.27 - 

#include <iostream>
#include <cstring>

using namespace std;

enum { MAKE = 1, DEPOSIT, WITHDRAW, SHOWINFO, EXIT };

enum { GRADE_A = 7, GRADE_B = 4, GRADE_C = 2 };

class Account {
private:
	int accID;		// 계좌번호
	int balance;	// 잔액
	char* cusName;	// 고객이름
public:
	Account(int accID, int money, char* Name);
	Account(const Account& copy);
	int getID(void) const;
	virtual void Deposit(int money) = 0;
	int WithDraw(int money);
	void showAccInfo(void) const;
	~Account();
};

Account::Account(int accID, int money, char* Name) : accID(accID), balance(money) {
	cusName = new char[strlen(Name) + 1];
	strcpy_s(cusName, strlen(Name) + 1, Name);
}

Account::Account(const Account& copy) : accID(copy.accID), balance(copy.balance) {
	cusName = new char[strlen(copy.cusName) + 1];
	strcpy_s(cusName, strlen(copy.cusName) + 1, copy.cusName);
}

void Account::Deposit(int money)
{
	balance += money;
}

int Account::getID(void) const
{
	return accID;
}

int Account::WithDraw(int money) {
	if (balance < money)
		return 0;
	balance -= money;
	return money;
}

void Account::showAccInfo(void) const
{
	cout << "=====================" << endl;
	cout << "계좌ID: " << accID << endl;
	cout << "고객이름: " << cusName << endl;
	cout << "잔액: " << balance << endl;
	cout << "=====================" << endl;
}

Account::~Account() {
	delete[] cusName;
}


/*
* 클래스 이름 : NormalAccount
* 클래스 설명 : 보통예금계좌(Account 상속)
*/
class NormalAccount : public Account {
private:
	int NAratio;
public:
	NormalAccount(int ID, int money, char* name, int ratio)
		: Account(ID, money, name), NAratio(ratio) {}

	int getNAratio(int money)
	{
		return (int)money*NAratio / 100;
	}

	void Deposit(int money)
	{
		Account::Deposit(money);
		Account::Deposit((int)money*NAratio / 100);
	}
};


/*
* 클래스 이름 : HighCreditAccount
* 클래스 설명 : 신용신뢰계좌(Account 상속)
*/
class HighCreditAccount :public NormalAccount {
private:
	int HCAGrade;
public:
	HighCreditAccount(int ID, int money, char* name, int ratio, int grade)
		: NormalAccount(ID, money, name, ratio), HCAGrade(grade) {}

	int HCAPlusInter(int money)
	{
		switch (HCAGrade)
		{
		case 1:
			return (int)money*GRADE_A / 100;
		case 2:
			return (int)money*GRADE_B / 100;
		case 3:
			return (int)money*GRADE_C / 100;
		default:
			break;
		}
	}

	void Deposit(int money)
	{
		Account::Deposit(money + NormalAccount::getNAratio(money) + HighCreditAccount::HCAPlusInter(money));
	}
};


/*
* 클래스 이름 : AccountHandler
* 클래스 유형 : control class
*/
class AccountHandler {
private:
	Account * Acc[100];
	int accCount = 0;	// 계좌 개수
public:
	AccountHandler();
	void showMenu(void) const;
	void createAcc(void);
	void deposit(void);
	void withDraw(void);
	void showAllInfo(void) const;
	int getAccCount(void);
	~AccountHandler();
};

AccountHandler::~AccountHandler() {
	for (int i = 0; i < accCount; ++i)
		delete Acc[i];
}

AccountHandler::AccountHandler() : accCount(0) {}

int AccountHandler::getAccCount(void)
{
	return accCount;
}

void AccountHandler::showMenu(void) const
{
	cout << "======================" << endl;
	cout << "Banking System ver 0.6" << endl;
	cout << "======================" << endl;
	cout << "         메뉴" << endl;
	cout << "     1. 계좌 생성" << endl;
	cout << "     2. 입금" << endl;
	cout << "     3. 출금" << endl;
	cout << "     4. 계좌 조회" << endl;
	cout << "     5. 종료" << endl;
	cout << " #선택====> ";
}
void AccountHandler::createAcc(void)
{
	int ch;

	int id;
	int balance;
	char name[20];
	int ratio;

	int creditGrade;

	cout << "[계좌종류선택]" << endl;
	cout << "1. 보통예금계좌 2. 신용신뢰계좌 " << endl;
	cout << "선택: ";
	cin >> ch;

	switch (ch)
	{
	case 1:
		cout << "[보통예금계좌 개설]" << endl;
		cout << "이름: "; cin >> name;
		cout << "계좌번호: "; cin >> id;
		cout << "입금액: "; cin >> balance;
		cout << "이자율: "; cin >> ratio;
		Acc[accCount++] = new NormalAccount(id, balance, name, ratio);
		cout << "---계좌개설완료!" << endl;
		cout << endl;
		break;
	case 2:
		cout << "[신용신뢰계좌 개설]" << endl;
		cout << "이름: "; cin >> name;
		cout << "계좌번호: "; cin >> id;
		cout << "입금액: "; cin >> balance;
		cout << "이자율: "; cin >> ratio;
		cout << "신용등급(Ato1,Bto2,Cto3): "; cin >> creditGrade;
		Acc[accCount++] = new HighCreditAccount(id, balance, name, ratio, creditGrade);
		cout << "---계좌개설완료!" << endl;
		cout << endl;
		break;

	default:
		break;
	}

}
void AccountHandler::deposit()
{
	int id;
	int money;
	cout << "[입 금]" << endl;
	cout << "계좌번호를 입력하세요: "; cin >> id;
	cout << "입금할 금액을 입력하세요: "; cin >> money;

	for (int i = 0; i < accCount; ++i) {
		if (Acc[i]->getID() == id) {
			Acc[i]->Deposit(money);
			cout << "입금 완료!" << endl;
			return;
		}
	}
	cout << "유효하지 않은 ID입니다." << endl;

}
void AccountHandler::withDraw()
{
	int id;
	int money;
	cout << "[출 금]" << endl;
	cout << "계좌번호를 입력하세요: "; cin >> id;
	cout << "출금할 금액을 입력하세요: "; cin >> money;

	for (int i = 0; i < accCount; ++i) {
		if (Acc[i]->getID() == id) {
			if (Acc[i]->WithDraw(money) == 0) {
				cout << "잔액이 부족합니다" << endl;
				return;
			}
			else {
				cout << "출금 완료!" << endl;
				return;
			}
		}
	}
	cout << "유효하지 않은 ID 입니다." << endl;
}

void AccountHandler::showAllInfo(void) const
{
	for (int i = 0; i < accCount; ++i) {
		cout << "[" << i + 1 << "]" << endl;
		Acc[i]->showAccInfo();
	}
}



/* --------------------
* main()
* ------------------- */
int main()
{
	AccountHandler manager;
	int choice;

	while (1) {
		manager.showMenu();
		cin >> choice;

		switch (choice)
		{
		case MAKE:
			manager.createAcc();
			break;
		case DEPOSIT:
			manager.deposit();
			break;
		case WITHDRAW:
			manager.withDraw();
			break;
		case SHOWINFO:
			manager.showAllInfo();
			break;
		case EXIT:
			return 0;
		default:
			break;
		}
		system("pause");
		system("cls");

	}
	return 0;
}